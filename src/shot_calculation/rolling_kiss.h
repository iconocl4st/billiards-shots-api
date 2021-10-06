//
// Created by thallock on 10/6/21.
//

#ifndef IDEA_ROLLING_GLANCE_H
#define IDEA_ROLLING_GLANCE_H

#include "shot_calculation/AssignmentStatus.h"
#include "math/rolling_glance.h"
#include "math/rolling_glance_to_pocket.h"
#include "billiards_common/shots/shot_helpers.h"

#include "shot_calculation/pocket.h"

namespace billiards::shots {

	[[nodiscard]] inline
	bool maybe_assign_rolling_kiss_locations(AssignmentStatus& status) {
		if (status.already_has_source() && status.already_has_target()) {
			return false;
		}
		const auto& prev = get_previous_status(status);
		if (!prev.already_has_source()) {
			return false;
		}
		const auto& cue_loc = prev.exiting_location;
		const auto& cue_radius = prev.exiting_radius;
		const auto& kiss_step = status.info.get_typed_step<KissStep>(status.index);
		const auto& obj_loc = status.params.locations.balls[kiss_step->kissed_ball];
		const auto& obj_radius = get_ball_type(status.params.table, status.params.locations, kiss_step->kissed_ball)->radius;
		const auto a = cue_loc - obj_loc;

		if (!a.is_valid() || !obj_radius.is_valid() || !cue_radius.is_valid()) {
			status.info.valid_calculations = false;
			// assign the locations anyway...
			return;
		}

		const auto& next_status = get_next_status(status);
		if (next_status.already_has_target()) {
			const auto& next_target = next_status.get_target();
			const auto target = std::make_shared<GoalPostTarget>();
			const auto r = cue_radius + obj_radius;
			for (int i = 0; i < 3; i++) {
				const auto d = next_target.posts[i] - obj_loc;
				if (!d.is_valid()) {
					status.info.valid_calculations = false;
					// assign the locations anyway...
					return;
				}
				bool assigned = false;
				get_glance_location(
					a.x.get(), a.y.get(),
					d.x.get(), d.y.get(),
					r.get(), M_PI,
					[&](const double x, const double y) {
						assigned = true;
						target->posts[i] = geometry::MaybePoint{x, y};
					}
				);

				if (!assigned) {
					status.info.valid_calculations = false;
					return;
				}
			}
			status.assign_target(target);
			// TODO:
			status.assign_exiting_location(target->posts[1]);
			return;
		}

		if (next_status.info.get_shot_type(status.index) != step_type::POCKET) {
			return false;
		}
		const auto& pocket_step = status.info.get_typed_step<PocketStep>(status.index + 1);
		const auto& pocket = status.params.table.get_pocket(pocket_step->pocket);

		const auto next_target = std::make_shared<GoalPostTarget>();
		const auto target = std::make_shared<GoalPostTarget>();
		const auto r = cue_radius + obj_radius;

		bool assigned = false;
		solve_glance_and_pocket(
			a.x.get(), a.y.get(),
			pocket.outer_segment_1.x, pocket.outer_segment_1.y,
			pocket.outer_segment_2.x, pocket.outer_segment_2.y,
			cue_radius.get(), obj_radius.get(),
			[&](const GlanceSolution& sol) {
				target->posts[0] = geometry::MaybePoint{sol.x, sol.y};
				next_target->posts[0] = geometry::MaybePoint{sol.dx, sol.dy};
			}
		);
		if (!assigned) {
			status.info.valid_calculations = false;
		}
		assigned = false;
		solve_glance_and_pocket(
			a.x.get(), a.y.get(),
			pocket.outer_segment_1.x, pocket.outer_segment_1.y,
			pocket.outer_segment_2.x, pocket.outer_segment_2.y,
			cue_radius.get(), obj_radius.get(),
			[&](const GlanceSolution& sol) {
				target->posts[0] = geometry::MaybePoint{sol.x, sol.y};
				next_target->posts[0] = geometry::MaybePoint{sol.dx, sol.dy};
			}
		);
		if (!assigned) {
			status.info.valid_calculations = false;
		}
		status.assign_target(target);
		tidy_pocket_posts(status, target);
		// TODO:
		status.assign_exiting_location(target->posts[1]);
		return true;
	}
}

#endif //IDEA_ROLLING_GLANCE_H
