//
// Created by thallock on 9/28/21.
//

#ifndef IDEA_TARGET_CALCULATIONS_H
#define IDEA_TARGET_CALCULATIONS_H

#include "common/shots/CueingInfo.h"
#include "common/config/Pocket.h"
#include "shot_calculations.h"
#include "basic_calculations.h"

namespace billiards::shots {

	inline
	geometry::MaybeDouble get_precision(
		const geometry::MaybePoint& cue,
		const geometry::MaybePoint& target1,
		const geometry::MaybePoint& target2
	) {
		// a . b = |a| * |b| cos theta
		auto p1 = target1 - cue;
		auto p2 = target2 - cue;
		auto cos_theta = p1.dot(p2) / p1.norm() / p2.norm();
		return cos_theta.acos();
	}

	inline
	geometry::MaybeDouble get_target_cut(
		const geometry::MaybePoint& cue,
		const geometry::MaybePoint& obj,
		const geometry::MaybeDouble& obj_r,
		const geometry::MaybePoint& target) {
		const auto direct_line = through(cue, obj);
		const auto orthog_line = orthogonal_at(direct_line, obj);

		if (((cue - obj).norm() < TOLERANCE).get_or(true)) {
			return {};
		}

		// for each target...
		const auto aim_line = through(cue, target);
		const auto inter = intersection(aim_line, orthog_line);
		const auto dist = (inter - obj).norm();
		const auto to_the_right = is_to_the_right_of(cue - obj, target - cue);
		const auto sign = ternary(to_the_right, -1, 1);
		return sign * dist / obj_r;
	}

	void orient_pocket(
		const geometry::MaybePoint& source,
		const geometry::MaybeDouble radius,
		const config::Pocket& pocket,
		const std::shared_ptr<GoalPostTarget>& dest
	) {
		dest->goal_post_1 = geometry::get_edge_point(
			source, pocket.outer_segment_1, radius, pocket.outer_segment_2);
		dest->goal_post_2 = geometry::get_edge_point(
			source, pocket.outer_segment_2, radius, pocket.outer_segment_1);
		dest->goal_post_center = (dest->goal_post_1 + dest->goal_post_2) / 2;
	}

	inline
	void construct_ghost_ball_target(
		const geometry::MaybePoint& object_location,
		const std::shared_ptr<GoalPostTarget>& target,
		const geometry::MaybeDouble& cue_radius,
		const geometry::MaybeDouble& obj_radius,
		const std::shared_ptr<GoalPostTarget>& source
	) {
		source->goal_post_1 = geometry::extend_line(target->goal_post_1, object_location, obj_radius + cue_radius);
		source->goal_post_2 = geometry::extend_line(target->goal_post_2, object_location, obj_radius + cue_radius);
		source->goal_post_center = geometry::extend_line(target->goal_post_center, object_location, obj_radius + cue_radius);
	}

	void calculate_rail_target(
		const geometry::MaybePoint& source,
		const std::shared_ptr<GoalPostTarget>& dest,
		const geometry::MaybeDouble radius,
		const config::Rail& rail,
		const std::shared_ptr<GoalPostTarget>& out
	) {
		out->goal_post_1 = geometry::calculate_bank(
			source, dest->goal_post_1,
			rail.segment1, rail.segment2, rail.in, radius);
		out->goal_post_2 = geometry::calculate_bank(
			source, dest->goal_post_2,
			rail.segment1, rail.segment2, rail.in, radius);
		out->goal_post_center = geometry::calculate_bank(
			source, dest->goal_post_center,
			rail.segment1, rail.segment2, rail.in, radius);
	}
}
#endif //IDEA_TARGET_CALCULATIONS_H
