//
// Created by thallock on 9/28/21.
//

#ifndef IDEA_CALCULATE_SHOT_H
#define IDEA_CALCULATE_SHOT_H

#include "common/shots/ShotInformation.h"
#include "common/shots/Locations.h"
#include "common/shots/shot_helpers.h"
#include "common/config/Table.h"
#include "target_calculations.h"

namespace billiards::shots {

	inline
	int determine_exiting_ball_index(ShotInformation& info, size_t index) {
		auto step = info.shot.steps[index];
		switch (step->type) {
			case step_type::STRIKE: {
				auto strike = std::dynamic_pointer_cast<StrikeStep>(step);
				return strike->object_ball;
			}
			case step_type::CUE: {
				auto cue = std::dynamic_pointer_cast<CueStep>(step);
				return cue->cue_ball;
			}
			case step_type::KISS:
			case step_type::RAIL:
				if (index <= 0) {
					throw std::runtime_error{"A kiss or rail cannot be first"};
				}
				return determine_exiting_ball_index(info, index - 1);
			case step_type::POCKET:
				throw std::runtime_error{"Pockets must be at the end"};
			case step_type::UNKNOWN:
			default:
				throw std::runtime_error{"Unknown shot type"};
		}
	}

	inline
	geometry::Point determine_source(
		const config::Table& table,
		const layout::Locations& locations,
		ShotInformation& info, size_t index) {
		if (index < 0) {
			throw std::runtime_error{"A ball must be in motion..."};
		}
		auto previous = info.shot.steps[index];
		switch (previous->type) {
			case step_type::STRIKE: {
				auto prev = std::dynamic_pointer_cast<StrikeStep>(previous);
				return locations.get_ball_location(prev->object_ball);
			}
			case step_type::CUE: {
				auto prev = std::dynamic_pointer_cast<CueStep>(previous);
				return locations.get_ball_location(prev->cue_ball);
			}
			case step_type::RAIL: {
				auto prev = std::dynamic_pointer_cast<RailStep>(previous);
				int rail_index = prev->rail;
				int bouncing_ball = determine_exiting_ball_index(info, index - 1);
				const auto rail = table.rail(rail_index);
				const geometry::Point source = determine_source(table, locations, info, index - 1);
				const auto radius = get_ball_type(table, locations, bouncing_ball)->radius;

				const auto rail_line = geometry::through(rail.segment1, rail.segment2);
				const auto reflection = geometry::reflect(source, rail_line);
				// Could use reflection...
				const auto travel_line = geometry::through(source, reflection);
				const auto bank_line = geometry::parallel_at(rail_line, rail.segment1 + rail.in * radius);
				const auto intersection = geometry::intersection(bank_line, travel_line);
				return intersection.get();
			}
			case step_type::KISS:
				throw std::runtime_error{"Not implemented"};
			case step_type::POCKET:
				throw std::runtime_error{"Pockets must be at the end"};
			case step_type::UNKNOWN:
			default:
				throw std::runtime_error{"Unknown shot type"};
		}
	}

//	template<class StepType>
//	std::shared_ptr<StepType> get_step(ShotInformation& info, const Destination& dest) {
//		const int step_index = dest.step_index;
//		if (step_index < 0 || step_index >= info.shot.steps.size()) {
//			throw std::runtime_error{"Invalid index"};
//		}
//		return std::dynamic_pointer_cast<StepType>(info.shot.steps[step_index]);
//	}

	inline
	void assign_target(
		const config::Table& table,
		const layout::Locations& locations,
		ShotInformation& info,
		Destination& dest,
		size_t step_index,
		const Destination* next_dest
	) {
		if (step_index <= 0) {
			throw std::runtime_error{"The first step does not have a target."};
		}

		switch (info.get_shot_type(dest)) {
			case step_type::POCKET: {
				const std::shared_ptr<PocketStep>& pocket_step = info.get_typed_step<PocketStep>(dest);
				int pocket_index = pocket_step->pocket;
				int sunk_ball = determine_exiting_ball_index(info, step_index - 1);
				const geometry::Point source = determine_source(table, locations, info, step_index - 1);
				std::shared_ptr<GoalPostTarget> target = std::make_shared<GoalPostTarget>();
				dest.target = std::dynamic_pointer_cast<Target>(target);
				orient_pocket(
					source,
					get_ball_type(table, locations, sunk_ball)->radius,
					table.get_pocket(pocket_index),
					target
				);
				break;
			}
			case step_type::STRIKE: {
				const std::shared_ptr<StrikeStep>& strike_step = info.get_typed_step<StrikeStep>(dest);
				int object_index = strike_step->object_ball;
				int cue_index = determine_exiting_ball_index(info, step_index - 1);

				std::shared_ptr<GoalPostTarget> target = std::make_shared<GoalPostTarget>();
				dest.target = std::dynamic_pointer_cast<Target>(target);

				const std::shared_ptr<Target> next_target = next_dest->target;
				switch (next_target->type) {
					case target_type::GOAL_POST: {
						const auto& next_goal_post = std::dynamic_pointer_cast<GoalPostTarget>(next_target);
						construct_ghost_ball_target(
							locations.get_ball_location(object_index),
							next_goal_post,
							get_ball_type(table, locations, cue_index)->radius,
							get_ball_type(table, locations, object_index)->radius,
							target
						);
						break;
					}
					case target_type::UNKNOWN:
					default:
						throw std::runtime_error{"Not implemented"};
				}
				break;
			}
			case step_type::RAIL: {
				const std::shared_ptr<RailStep>& rail_step = info.get_typed_step<RailStep>(dest);
				int rail_index = rail_step->rail;
				int bouncing_ball = determine_exiting_ball_index(info, step_index - 1);
				const geometry::Point source = determine_source(table, locations, info, step_index - 1);
				const auto rail = table.rail(rail_index);
				const auto radius = get_ball_type(table, locations, bouncing_ball)->radius;
				std::shared_ptr<GoalPostTarget> target = std::make_shared<GoalPostTarget>();
				dest.target = std::dynamic_pointer_cast<Target>(target);
				const std::shared_ptr<Target> next_target = next_dest->target;
				switch (next_target->type) {
					case target_type::GOAL_POST: {
						const std::shared_ptr<GoalPostTarget> next_gp_target = std::dynamic_pointer_cast<GoalPostTarget>(next_target);
						calculate_rail_target(
							source,
							next_gp_target,
							radius,
							rail,
							target
						);
						break;
					}
					case target_type::UNKNOWN:
						throw std::runtime_error{"Unknown target type"};
				}
				break;
			}
			case step_type::UNKNOWN:
			default:
				throw std::runtime_error{"Not implemented"};
		}
	}

	/*
	inline
	std::shared_ptr<Destination> construct_destination(const std::shared_ptr<ShotStep>& step, int index) {
		switch (step->type) {
			case step_type::POCKET: {
				auto pocket_dest = std::make_shared<PocketDestination>(index);
				return std::dynamic_pointer_cast<Destination>(pocket_dest);
			}
			case step_type::STRIKE: {
				auto strike_dest = std::make_shared<GhostBallDestination>(index);
				return std::dynamic_pointer_cast<Destination>(strike_dest);
			}
			case step_type::RAIL: {
				auto strike_dest = std::make_shared<GhostBallDestination>(index);
				return std::dynamic_pointer_cast<Destination>(strike_dest);
			}
			case step_type::KISS:
				throw std::runtime_error{"Implement me"};
			case step_type::CUE:
			case step_type::UNKNOWN:
			default:
				throw std::runtime_error{"Invalid destination"};
		}
	}*/

	inline
	void assign_cueing(
		const config::Table& table,
		const layout::Locations& locations,
		ShotInformation& info,
		const std::shared_ptr<CueStep>& cue_step,
		const Destination& next_dest,
		CueingInfo& cueing
	) {
		int cue_index = cue_step->cue_ball;

		// assign location
		cueing.cue_location = locations.get_ball_location(cue_index);

		// assign precision
		switch (next_dest.target->type) {
			case target_type::GOAL_POST: {
				const auto& goal_posts = std::dynamic_pointer_cast<GoalPostTarget>(next_dest.target);
				cueing.precision = get_precision(
					locations.get_ball_location(cue_index),
					goal_posts->goal_post_1,
					goal_posts->goal_post_2
				);
				break;
			}
			case target_type::UNKNOWN:
			default:
				throw std::runtime_error{"Unknown target type"};
		}

		// Assign cuts...
		switch (info.get_shot_type(next_dest)) {
			case step_type::STRIKE: {
				const auto& strike_step = info.get_typed_step<StrikeStep>(next_dest);
				const auto& goal_posts = std::dynamic_pointer_cast<GoalPostTarget>(next_dest.target);
				int object_index = strike_step->object_ball;
				auto cut_angle_1 = get_target_cut(
					locations.get_ball_location(cue_index),
					locations.get_ball_location(object_index),
					get_ball_type(table, locations, object_index)->radius,
					goal_posts->goal_post_1);
				auto cut_angle_2 = get_target_cut(
					locations.get_ball_location(cue_index),
					locations.get_ball_location(object_index),
					get_ball_type(table, locations, object_index)->radius,
					goal_posts->goal_post_2);

				cueing.lower_cut_angle = cut_angle_1.min(cut_angle_2);
				cueing.upper_cut_angle = cut_angle_1.max(cut_angle_2);
				break;
			}
			case step_type::RAIL:
			case step_type::POCKET:
				// No cuts available...
				break;
			case step_type::KISS:
				throw std::runtime_error{"Implement me"};
			case step_type::CUE:
				throw std::runtime_error{"Two cues in a row"};
			default:
			case step_type::UNKNOWN:
				throw std::runtime_error{"Unknown step type"};
		}
	}

	inline
	void calculate_shot(
		const config::Table& table,
		const layout::Locations& locations,
		ShotInformation& info
	) {
		// TODO: Fit the Locations to the Table...
		const auto num_steps = info.shot.steps.size();
		// when we have more? (I think bank shots will have more...)
		info.destinations.reserve(num_steps - 1);


		Destination *next_dest = nullptr;
		for (int i = (int) num_steps - 1; i > 0; i--) {
			info.destinations.emplace_back(i);
			assign_target(table, locations, info, info.destinations.back(), i, next_dest);
			next_dest = &info.destinations.back();
		}
		std::reverse(info.destinations.begin(), info.destinations.end());

		switch (info.shot.steps[0]->type) {
			case step_type::CUE: {
				auto cue_step = std::dynamic_pointer_cast<CueStep>(info.shot.steps[0]);
				assign_cueing(table, locations, info, cue_step, info.destinations[0], info.cueing);
				break;
			}
			default:
				throw std::runtime_error{"First step must be a cue"};
		}
	}
}

#endif //IDEA_CALCULATE_SHOT_H
