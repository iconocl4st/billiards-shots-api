//
// Created by thallock on 10/5/21.
//

#ifndef IDEA_STRIKE_H
#define IDEA_STRIKE_H

#include "shot_calculation/AssignmentStatus.h"
#include "billiards_common/geometry/geometry.h"

namespace billiards::shots {

	[[nodiscard]] inline
	bool maybe_assign_strike_source(AssignmentStatus& status) {
		if (status.already_has_source()) {
			return false;
		}

		const auto& strike_step = status.info.get_typed_step<Strike>(status.index);
		status.assign_source(strike_step->struck_ball);
		return true;
	}

	[[nodiscard]] inline
	bool maybe_assign_strike_target(AssignmentStatus& status) {
		if (status.already_has_target()) {
			return false;
		}
		if (!get_previous_status(status).assigned_exiting_radius || !get_next_status(status).already_has_target()) {
			return false;
		}
		if (!status.already_has_source()) {
			return false;
		}

		const auto& strike_step = info.get_typed_step<StrikeStep>(dest);
		const auto& cue_radius = ass.statuses[status.index - 1].exiting_radius;
		const auto& obj_loc = status.exiting_source;
		const auto& object_radius = status.exiting_radius;

		auto target = std::make_shared<GoalPostTarget>();
		auto& next_target = ass.info.destinations[index + 1].target;
		for (int i = 0; i < 3; i++) {
			target->posts[i] = geometry::extend_line(next_target->posts[i], object_loc, obj_radius + cue_radius);
		}
		status.assign_target(target);
		return true;
	}


	[[nodiscard]] inline
	bool maybe_assign_strike(AssignmentStatus& status) {
		const b1 = maybe_assign_cue_source(status);
		const b2 = maybe_assign_cue_target(status);
		return b1 || b2;
	}
}

#endif //IDEA_STRIKE_H
