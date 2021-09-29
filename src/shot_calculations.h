//
// Created by thallock on 9/28/21.
//

#ifndef IDEA_SHOT_CALCULATIONS_H
#define IDEA_SHOT_CALCULATIONS_H

#include "./basic_calculations.h"

namespace billiards::geometry {

	inline
	bool solve_edge_system(
		const MaybePoint& w, const MaybeDouble& r,
		MaybePoint& sol0, MaybePoint& sol1
	) {
		const auto r2 = r * r;
		const auto wx2 = w.x * w.x;
		const auto wy2 = w.y * w.y;

		const auto wx2pwy2 = wx2 + wy2;
		const auto rad = (wx2pwy2 - r2).sqrt();
		const auto x_den = wx2pwy2;
		const auto xp = w.x * r2;
		const auto y_den = w.y * wx2pwy2;
		const auto yp = wy2 * r2;

		const auto y_is_small = y_den.cannot_divide()
			|| w.y.cannot_divide()
			|| wy2.cannot_divide();
		const auto x_is_small = x_den.cannot_divide()
			|| w.x.cannot_divide()
			|| wx2.cannot_divide();

		if (y_is_small) {
			if (x_is_small) {
				// Infinitely man solutions if r == 0
				// No solutions otherwise
				return false;
			}

			sol0.x = r2 / w.x;
			sol0.y = (r2 - (-r2 / wx2 + 1)).sqrt();
			sol1.x = sol0.x;
			sol1.y = -sol0.y;
			return true;
		}
		if (x_is_small) {
			sol0.x = (r2 - (-r2 / wy2 + 1)).sqrt();
			sol0.y = r2 / w.y;
			sol1.x = -sol0.x;
			sol1.y = sol0.y;
			return true;
		}

		sol0.x = (xp -       w.y * r * rad) / x_den;
		sol0.y = (yp + w.x * w.y * r * rad) / y_den;
		sol1.x = (xp +       w.y * r * rad) / x_den;
		sol1.y = (yp - w.x * w.y * r * rad) / y_den;
		return true;
	}

	MaybePoint get_edge_point(
		const MaybePoint& obj_location, const MaybePoint& goal_post, const MaybeDouble radius,
		const MaybePoint& inner
	) {
		const auto line = through(obj_location, goal_post);
		const auto orientation = line.at(inner).signum();
		MaybePoint trial1; MaybePoint trial2;
		bool success = solve_edge_system(obj_location - goal_post, radius, trial1, trial2);
		if (!success) {
			// TODO: What do we do in this case...
			throw std::runtime_error{"Unhandled case..."};
		}
		{
			const auto ret = goal_post + trial1;
			if ((line.at(ret).signum() == orientation).get_or(false)) {
				return ret;
			}
		}
		{
			const auto ret = goal_post + trial2;
			if ((line.at(ret).signum() == orientation).get_or(false)) {
				return ret;
			}
		}
		return MaybePoint{};
	}


	inline
	MaybePoint project_onto_segment(
		const MaybePoint& l1,
		const MaybePoint& l2,
		const MaybePoint& p
	) {
		const auto diff = l2 - l1;
		const auto t1 = (p - l1).dot(diff) / diff.norm2();
		const auto t2 = t1.max(0).min(1);
		return l1 + diff * t2;
	}

	inline
	MaybeDouble distance_to_segment(
		const MaybePoint& l1,
		const MaybePoint& l2,
		const MaybePoint& p
	) {
		return (p - project_onto_segment(l1, l2, p)).norm();
	}

	inline
	MaybeDouble determinant(const MaybePoint& p1, const MaybePoint& p2) {
		return p1.x * p2.y - p1.y * p2.x;
	}

	inline
	MaybeBool is_to_the_right_of(const MaybePoint& p1, const MaybePoint& p2) {
		return determinant(p1, p2) < 0;
	}

	inline
	MaybeBool triangle_contains2(
		const MaybePoint& v1,
		const MaybePoint& v2,
		const MaybePoint& v3,
		const MaybePoint& p
	) {
		return (
			on_same_side(v1, v2, v3, p) &&
			on_same_side(v1, v3, v2, p) &&
			on_same_side(v2, v3, v1, p)
		);
	}

}


#endif //IDEA_SHOT_CALCULATIONS_H
