//
// Created by thallock on 9/28/21.
//

#ifndef IDEA_SHOT_CALCULATIONS_H
#define IDEA_SHOT_CALCULATIONS_H

#include "basic_calculations.h"
#include "linear_system_2x2.h"

namespace billiards::geometry {

	[[nodiscard]] inline
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


	// Solves:
	// norm(gp - x) = radius
	// (x - obj) @ (x - gp) == 0
	[[nodiscard]] inline
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

	[[nodiscard]] inline
	MaybePoint calculate_bank(
		const MaybePoint& src, const MaybePoint& dst,
		const MaybePoint& s1, const MaybePoint& s2,
		const MaybePoint& in,
		const MaybeDouble radius
	) {
		const auto rail_line = geometry::through(s1, s2);
		const auto reflection = geometry::reflect(dst, rail_line);
		const auto travel_line = geometry::through(src, reflection);
		const auto bank_line = geometry::parallel_at(rail_line, s1 + in * radius);
		const auto intersection = geometry::intersection(bank_line, travel_line);
		return intersection;
	}


	[[nodiscard]] inline
	MaybePoint calculate_bank_2(
		const MaybePoint& src, const MaybePoint& dst,
		const MaybePoint& s1, const MaybePoint& s2,
		const MaybePoint& in,
		const MaybeDouble radius
	) {
		const auto rail_line = through(s1, s2);
		const auto bank_line = parallel_at(rail_line, s1 + in * radius);

		// Solve:
		// (x - src.x) / (y - src.y) == (x - dst.x) / (y - dst.y)
		// a * x + b * y + c == 0

		// (y - dst.y) * (x - src.x) == alpha * (x - dst.x) * (y - src.y) is nonlinear

		// (y - dst.y) * (x - src.x) == (x - dst.x) * (y - src.y)
		// y * (x - src.x) - dst.y * (x - src.x) == x * (y - src.y) - dst.x * (y - src.y)
		// y * x - y * src.x - dst.y * x + dst.y * src.x == x * y - x * src.y - dst.x * y + dst.x * src.y
		// y * x - y * src.x - dst.y * x + dst.y * src.x - x * y + x * src.y + dst.x * y - dst.x * src.y == 0
		// -y * src.x - dst.y * x + dst.y * src.x + x * src.y + dst.x * y - dst.x * src.y == 0
		// [src.y - dst.y] * x - [src.x - dst.x] * y + [dst.y * src.x - dst.x * src.y] == 0

		return solve_2x2(
			rail_line.a, rail_line.b, rail_line.c,
			src.y - dst.y, src.x - dst.x, dst.y * src.x - dst.x * src.y,
			(src.x + dst.x) / 2, (src.y + dst.y) / 2
		);
	}

	[[nodiscard]] inline
	MaybeDouble determinant(const MaybePoint& p1, const MaybePoint& p2) {
		return p1.x * p2.y - p1.y * p2.x;
	}

	[[nodiscard]] inline
	MaybeBool is_to_the_right_of(const MaybePoint& p1, const MaybePoint& p2) {
		return determinant(p1, p2) < 0;
	}

	[[nodiscard]] inline
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

	[[nodiscard]] inline
	MaybePoint calculate_rolling_glance(
		const MaybePoint& src,
		const MaybeDouble& src_r,
		const MaybePoint& obj,
		const MaybeDouble& obj_r,
		const MaybePoint& dst
	) {
		// Solve:
		// (x - obj.x) ** 2 + (y - obj.y) ** 2 == (obj_r + src_r) ** 2
		// p = {x, y}

		// aim = p - src
		// dir = dst - p
		// tan = orthogonal aim at p
		// tan @ tan == 1
		//
		// dir == 2.0 / 7 * tan + 5.0 / 7 * t * aim
		// (t * aim - tan) @ tan == 0
		// t == 1 / (aim @ tan)

		// dir == 2.0 / 7 * tan + 5.0 / 7 * aim / (aim @ tan)
		// dir == 2.0 / 7 * tan + 5.0 / 7 * aim / (aim @ tan)

		// x, y
		// aim = {src.y - y, x - src.x, y * (src.x - x) + x * (y - src.y)} // runs through x,y
		// tan = {x - src.x, y - src.y, x * (src.x - x) + y * (src.y - y)} // runs though x, y; orthogonal to aim


		// d = 2/7 * n + 5/7 * t * a
		// (t * a - n) @ n == 0
		// t * a @ n == n @ n


		// a = p - src
		// n @ a == 0
		// n @ [p; 1] == 0
		// d = 5/7 * n @ n + 1 / (n @ a) * a + 2/7 * n
		// d = dst - p

		// nx * (x - sx) + ny * (y - sy) == 0
		// dst.x - p == 2/7 * nx + 5/7 * (nx ** 2 + ny ** 2 +  1 / (nx * (x - sx) + ny * (y - sy)) *
		return MaybePoint{};
	}

	[[nodiscard]] inline
	MaybePoint calculate_stun_glance(
		const MaybePoint& src,
		const MaybeDouble& src_r,
		const MaybePoint& obj,
		const MaybeDouble& obj_r,
		const MaybePoint& dst
	) {
		// Solve:
		// (x - obj.x) ** 2 + (y - obj.y) ** 2 == (obj_r + src_r) ** 2
		// (x - dst.x) * (x - src.x) + (y - dst.y) * (y - src.y) == 0
		return get_edge_point(src, obj, src_r + obj_r, dst);
	}
}


#endif //IDEA_SHOT_CALCULATIONS_H
