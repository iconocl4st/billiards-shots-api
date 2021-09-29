//
// Created by thallock on 9/28/21.
//

#ifndef IDEA_CALCULATE_SHOT_H
#define IDEA_CALCULATE_SHOT_H

#include "common/shots/ShotInformation.h"
#include "common/shots/Locations.h"
#include "common/config/Table.h"
#include "target_calculations.h"

namespace billiards::shots {

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

	geometry::Point determine_source(const layout::Locations& locations, ShotInformation& info, size_t index) {
		if (index < 0) {
			throw std::runtime_error{"A ball must be in motion..."};
		}
		auto previous = info.shot.steps[index];
		switch (previous->type) {
			case step_type::STRIKE: {
				auto prev = std::dynamic_pointer_cast<StrikeStep>(previous);
				auto ball_index = prev->object_ball;
				if (ball_index < 0 || ball_index >= locations.balls.size()) {
					throw std::runtime_error{"Invalid index"};
				}
				return locations.balls[ball_index].location;
			}
			case step_type::CUE: {
				auto prev = std::dynamic_pointer_cast<CueStep>(previous);
				auto ball_index = prev->cue_ball;
				if (ball_index < 0 || ball_index >= locations.balls.size()) {
					throw std::runtime_error{"Invalid index"};
				}
				return locations.balls[ball_index].location;
			}
			case step_type::KISS:
			case step_type::RAIL:
				// Probably need to return an object...
				throw std::runtime_error{"Not implemented"};
			case step_type::POCKET:
				throw std::runtime_error{"Pockets must be at the end"};
			case step_type::UNKNOWN:
			default:
				throw std::runtime_error{"Unknown shot type"};
		}
	}

	template<class DestType, class StepType>
	std::shared_ptr<StepType> get_step(ShotInformation& info, DestType dest) {
		const int step_index = dest.step_index;
		if (step_index < 0 || step_index >= info.shot.steps.size()) {
			throw std::runtime_error{"Invalid index"};
		}
		return std::dynamic_pointer_cast<StepType>(info.shot.steps[step_index]);
	}

	void assign_target(
		const config::Table& table,
		const layout::Locations& locations,
		ShotInformation& info,
		std::shared_ptr<Destination> dest,
		size_t step_index,
		std::shared_ptr<Destination> next_dest
	) {
		switch (dest->type) {
			case dest_type::POCKET: {
				auto pocket_dest = std::dynamic_pointer_cast<PocketDestination>(dest);
				int pocket_index = get_step<PocketDestination, PocketStep>(info, *pocket_dest)->pocket;
				if (pocket_index < 0 || pocket_index >= table.pockets.size()) {
					throw std::runtime_error{"Invalid index"};
				}
				if (step_index <= 0) {
					throw std::runtime_error{"A pocket cannot be first."};
				}
				int sunk_ball = determine_exiting_ball_index(info, step_index - 1);
				const geometry::Point source = determine_source(locations, info, step_index - 1);
				orient_pocket(
					geometry::MaybePoint{source},
					geometry::MaybeDouble{table.balls[sunk_ball].radius},
					table.pockets[pocket_index],
					pocket_dest->target
				);
				break;
			}
			case dest_type::GHOST_BALL: {
				auto ghost_dest = std::dynamic_pointer_cast<GhostBallDestination>(dest);
				int object_index = get_step<GhostBallDestination, StrikeStep>(info, *ghost_dest)->object_ball;
				if (object_index < 0 || object_index >= table.pockets.size() || object_index >= locations.balls.size()) {
					throw std::runtime_error{"Invalid index"};
				}
				if (step_index <= 0) {
					throw std::runtime_error{"A ghost ball cannot be first."};
				}
				int cue_index = determine_exiting_ball_index(info, step_index - 1);
				auto *target = next_dest->get_target();
				switch (target->type) {
					case target_type::GOAL_POST: {
						auto *gp_target = (GoalPostTarget *) target;
						construct_ghost_ball_target(
							geometry::MaybePoint{locations.balls[object_index].location},
							(GoalPostTarget *) target,
							geometry::MaybeDouble{table.balls[cue_index].radius},
							geometry::MaybeDouble{table.balls[object_index].radius},
							&ghost_dest->target
						);
						break;
					}
					case target_type::UNKNOWN:
					default:
						throw std::runtime_error{"Not implemented"};
				}
				break;
			}
			case dest_type::UNKNOWN:
			default:
				throw std::runtime_error{"Not implemented"};
		}
	}

	std::shared_ptr<Destination> construct_destination(std::shared_ptr<ShotStep> step, int index) {
		switch (step->type) {
			case step_type::POCKET: {
//				auto pocket_step = std::dynamic_pointer_cast<PocketStep>(step);
				auto pocket_dest = std::make_shared<PocketDestination>(index);
				return std::dynamic_pointer_cast<Destination>(pocket_dest);
			}
			case step_type::STRIKE: {
//				auto strike_step = std::dynamic_pointer_cast<StrikeStep>(step);
				auto strike_dest = std::make_shared<GhostBallDestination>(index);
				return std::dynamic_pointer_cast<Destination>(strike_dest);
			}
			case step_type::KISS:
			case step_type::RAIL:
			case step_type::CUE:
			case step_type::UNKNOWN:
			default:
				throw std::runtime_error{"Implement me"};
		}
	}

	void assign_cueing(
		const config::Table& table,
		const layout::Locations& locations,
		ShotInformation& info,
		std::shared_ptr<CueStep> cue_step,
		std::shared_ptr<Destination> next_dest,
		CueingInfo& cueing
	) {
		int cue_index = cue_step->cue_ball;
		if (cue_index < 0 || cue_index >= table.balls.size() || cue_index >= locations.balls.size()) {
			throw std::runtime_error{"Invalid index"};
		}

		switch (next_dest->type) {
			case dest_type::POCKET: {
				auto p_dest = std::dynamic_pointer_cast<PocketDestination>(next_dest);

				throw std::runtime_error{"Implement me!"};
			};
			case dest_type::GHOST_BALL: {
				auto p_dest = std::dynamic_pointer_cast<GhostBallDestination>(next_dest);

				int object_index = get_step<GhostBallDestination, StrikeStep>(info, *p_dest)->object_ball;
				if (object_index < 0 || object_index >= table.balls.size() || object_index >= locations.balls.size()) {
					throw std::runtime_error{"Invalid index"};
				}

				auto cut_angle_1 = get_target_cut(
					locations.balls[cue_index].location,
					locations.balls[object_index].location,
					table.balls[object_index].radius,
					p_dest->target.goal_post_1);
				auto cut_angle_2 = get_target_cut(
					locations.balls[cue_index].location,
					locations.balls[object_index].location,
					table.balls[object_index].radius,
					p_dest->target.goal_post_2);

				cueing.lower_cut_angle = cut_angle_1.min(cut_angle_2);
				cueing.upper_cut_angle = cut_angle_1.max(cut_angle_2);
				cueing.precision = get_precision(
					locations.balls[cue_index].location,
					p_dest->target.goal_post_1,
					p_dest->target.goal_post_2
				);
				break;
			}
			case dest_type::UNKNOWN:
			default:
				throw std::runtime_error{"Unknown type"};
		}
	}

	void calculate_shot(
		const config::Table& table,
		const layout::Locations& locations,
		ShotInformation& info
	) {
		// TODO: Fit the Locations to the Table...
		const auto num_steps = info.shot.steps.size();
		// when we have more? (I think bank shots will have more...)
		info.destinations.reserve(num_steps - 1);

		std::shared_ptr<Destination> next_dest{nullptr};
		for (int i = num_steps - 1, d_index = 0; i > 0; i--, d_index++) {
			auto current_step = info.shot.steps[i];
			std::shared_ptr<Destination> current_dest = construct_destination(current_step, i);
			assign_target(table, locations, info, current_dest, i,next_dest);
			next_dest = current_dest;

			info.destinations[d_index] = current_dest;
		}

		switch (info.shot.steps[0]->type) {
			case step_type::CUE: {
				auto cue_step = std::dynamic_pointer_cast<CueStep>(info.shot.steps[0]);
				assign_cueing(table, locations, info, cue_step, next_dest, info.cueing);
				break;
			}
			default:
				throw std::runtime_error{"First step must be a cue"};
		}
	}















//	std::shared_ptr<Destination> construct_strike_destination(
//		const config::Table& table,
//		const layout::Locations& locations,
//		ShotInformation& info,
//		std::shared_ptr<StrikeStep> step,
//		size_t step_index,
//		std::shared_ptr<Destination> target
//	) {
//		int struck_index = step->object_ball;
//		if (struck_index < 0 || struck_index >= table.balls.size() || struck_index >= locations.balls.size()) {
//			throw std::runtime_error{"Invalid index"};
//		}
//
//		/////////////////////////////////////////////////////////////////////////
//		/// Get Target...
//		/////////////////////////////////////////////////////////////////////////
//		Target *target = nullptr;
//		switch (target->type) {
//			case dest_type::GHOST_BALL: {
//				auto next_dest = std::dynamic_pointer_cast<GhostBallDestination>(target);
//				int object_index = next_dest->step->object_ball;
//				if (object_index < 0 || object_index >= table.balls.size() || object_index >= locations.balls.size()) {
//					throw std::runtime_error{"Invalid index"};
//				}
//
//				const Point& object_location,
//				const GoalPostTarget& target,
//				const geometry::MaybeDouble cue_radius,
//				const geometry::MaybeDouble obj_radius,
//				GoalPostTarget& source
//
//				construct_ghost_ball_target(
//					locations.balls[0].location,
//					next_dest->target,
//					geometry::MaybeDouble{table.balls[struck_index].radius},
//					geometry::MaybeDouble{table.balls[object_index].radius},
//					next_dest->target
//				)
//
//
//				target = (Target *)&next_dest->target;
//				break;
//			}
//			case dest_type::POCKET: {
//			}
//			case dest_type::UNKNOWN:
//			default:
//				throw std::runtime_error{"Not implemented"};
//		}
//
//		if (step_index <= 0) {
//			throw std::runtime_error{"Strike steps cannot be first"};
//		}
//		auto previous_step = info.shot.steps[step_index - 1];
//		/////////////////////////////////////////////////////////////////////////
//		/// Get Destination...
//		/////////////////////////////////////////////////////////////////////////
//		switch (previous_step->type) {
//			case dest_type::GHOST_BALL: {
//				auto next_dest = std::dynamic_pointer_cast<GhostBallDestination>(target);
//				auto dest = std::make_shared<GhostBallDestination>(step);
//
//				int cue_index = step->object_ball;
//				if (cue_index < 0 || cue_index >= table.balls.size() || cue_index >= locations.balls.size()) {
//					throw std::runtime_error{"Invalid index"};
//				}
//
//				construct_ghost_ball_target(
//					locations.balls[0].location,
//					next_dest->target,
//					geometry::MaybeDouble{table.balls[cue_index].radius},
//					geometry::MaybeDouble{table.balls[obj_index].radius},
//					dest->target
//				)
//			}
//			case dest_type::POCKET: {
//
//			}
//			case dest_type::UNKNOWN:
//			default:
//				throw std::runtime_error{"Not implemented"};
//		}
//	}
}

#endif //IDEA_CALCULATE_SHOT_H
