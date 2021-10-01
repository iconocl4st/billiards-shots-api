//
// Created by thallock on 9/30/21.
//

#ifndef IDEA_SOLVE_QUADRATIC_2X2_H
#define IDEA_SOLVE_QUADRATIC_2X2_H

#include "common/geometry/Maybe.h"

namespace billiards::shots {

	// Solve:
	// a == 0
	void solve_0(
		double a, double default_val,
		const std::function<void(const double)>& sol
	) {
		const bool a_is_small = std::abs(a) < TOLERANCE;
		if (a_is_small) {
			sol(default_val);
		}
	}

	// Solve:
	// a0 + x == 0
	void solve_1n(
		double a0
		const std::function<void(const double)>& sol
	) {
		sol(-a0);
	}

	// Solve:
	// a0 + a1 * x
	void solve_1(
		double a0, double a1, double default_val,
		const std::function<void(const double)>& sol
	) {
		const bool a1_is_small = std::abs(a1) < TOLERANCE;
		if (a1_is_small) {
			solve_0(a0, default_val, sol);
			return;
		}
		return sol_1n(a0 / a1, sol);
	}

	// Solve:
	// a0 + a1 * x + x ** 2 == 0
	void solve_2n(
		double a0, double a1, double default_val,
		const std::function<void(const double)>& sol
	) {
		const double discriminant = a1 * a1 - 4 * a0;
		if (discriminant < -TOLERANCE) {
			return;
		}
		if (discriminant < TOLERANCE) {
			sol(-a1 / 2);
			return;
		}
		const double s = std::sqrt(discriminant);
		sol((-a1 + s) / 2);
		sol((-a1 - s) / 2);
	}

	// Solve:
	// a0 + a1 * x + a2 * x ** 2 == 0
	void solve_2(
		double a0, double a1, double a2, double default_val,
		const std::function<void(const double)>& sol
	) {
		const bool a2_is_small = std::abs(a2) < TOLERANCE;
		if (a2_is_small) {
			solve_l(a0, a1, default_val, sol);
			return;
		}
		solve_2n(a0 / a2, a1 / a2, default_val, sol);
	}

	// Solve:
	// a0 + a1 * x + a2 * x ** 2 + x ** 3 == 0
	void solve_3n(
		const double a0, const double a1, const double a2
		const double default_val,
		const std::function<void(const double)>& sol
	) {
		// https://sciencing.com/how-to-factor-polynomials-of-degree-3-12751796.html
		// https://math.vanderbilt.edu/schectex/courses/cubic/
		const double p = -a2 / 3;
		const double q = p * p * p + (a2 * a1 - 3 * a0) / 6;
		const double r = a1 / 3;
		const double p2 = p * p;
		const double rmp2 = r - p2;

		const double d = q * q + rmp * rmp * rmp;
		if (d < 0) {
			// TODO
			return;
		}
		const double sd = std::sqrt(d);
		sol(std::pow(q + sd, 1.0 / 3) + std::pow(q - sd, 1.0 / 3) + p)
	}

	// Solve:
	// a0 + a1 * x + a2 * x ** 2 + a3 * x ** 3 == 0
	void solve_3(
		const double a0, const double a1, const double a2, const double a3, const double a4,
		const double default_val,
		const std::function<void(const double)>& sol
	) {
		const bool a4_is_small = std::abs(a4) < TOLERANCE;
		if (a4_is_small) {
			solve_3(a0, a1, a2, a3, default_val, sol);
			return;
		}
		solve_4n(a0 / a4, a1 / a4, a2 / a4, a3 / a4, default_val, sol);
	}


//	// Solve:
//	// a0 + a1 * x + a2 * x ** 2 + a4 * x ** 4 == 0
//	void solve_4_s(
//		const double a0, const double a1, const double a2, const double a4,
//		const double default_val,
//		const std::function<void(const double)>& sol
//	) {
//
//	}

	// Solve:
	// a0 + a1 * x + a2 * x ** 2 + a3 * x ** 3 + x ** 4 == 0
	void solve_4n(
		const double a0, const double a1, const double a2, const double a3,
		const double default_val,
		const std::function<void(const double)>& sol
	) {
		// https://www.maa.org/sites/default/files/pdf/upload_library/22/Ford/auckly29.pdf
		const double p = b - 3 * a3 / 8;
		const double q = a1 - a3 * a2 / 2 - a3 * a3 * a3 / 8;
		const double r = a0 - a3 * a1 / 4 + a3 * a3 * a2 / 16 - 3 * a3 * a3 * a3 * a3 / 256;
		solve_3n(
			-q * q, p * p - 4 * r, 2 * p,
			default_val,
			[p, q, a3sol&](const double lam){
				if (lam < -TOLERANCE) {
					// TODO
					return;
				}
				if (lam < TOLERANCE) {
					// TODO
					return;
				}

				const double slam = std::sqrt(lam);
				sol((-slam + 2 * (p + lam + q / slam)) / 2 - a3 / 4);
				sol((-slam - 2 * (p + lam + q / slam)) / 2 - a3 / 4);
				sol((-slam + 2 * (p + lam - q / slam)) / 2 - a3 / 4);
				sol((-slam - 2 * (p + lam - q / slam)) / 2 - a3 / 4);
			}
		);
	}

	// Solve:
	// a0 + a1 * x + a2 * x ** 2 + a3 * x ** 3 + a4 * x ** 4 == 0
	void solve_4(
		const double a0, const double a1, const double a2, const double a3, const double a4,
		const double default_val,
		const std::function<void(const double)>& sol
	) {
		const bool a4_is_small = std::abs(a4) < TOLERANCE;
		if (a4_is_small) {
			solve_3(a0, a1, a2, a3, default_val, sol);
			return;
		}
		solve_4n(a0 / a4, a1 / a4, a2 / a4, a3 / a4, default_val, sol);
	}



	// Solve
	// b00 + a01 * x + b10 * y + b20 * x ** 2 + b02 * y ** 2 == 0
	// with x = x_val
	void subs_22(
		const double x_val,
		const double a00, const double a01, const double a10, const double a20, const double a02,
		const double default_y,
		const std::function<void(const geometry::Point&)>& sol
	) {
		// Could use default_y
		solve_q(
			a00 + a01 * x_val + b20 * x_val * x_val, b10, b02,
			default_y,
			[sol&](const double y) { sol(x_val, y); }
		);
	}
	// Any solution to :
	// b00 + a01 * x + b10 * y + b20 * x ** 2 + b02 * y ** 2 == 0
	void solve_qq(
		double a00, double a01, double a10, double a20, double a02,
		double default_x, double default_y,
		const std::function<void(const geometry::Point&)>& sol
	) {
		subs_qq(default_x, a00, a01, a10, a20, a02, default_y, sol);
	}

	// Solve:
	// a0 == 0
	// b00 + a01 * x + b10 * y + b20 * x ** 2 + b02 * y ** 2 == 0
	void solve_00_22(
		double a0,
		double b00, double b01, double b10, double b20, double b02,
		double default_x, double default_y,
		const std::function<void(const geometry::Point&)>& sol
	) {
		const bool a0_is_small = std::abs(a0) < TOLERANCE;
		if (a0_is_small) {
			return solve_qq(b00, b01, b10, b20, b02, default_x, default_y, sol);
		}
	}

	// Solve:
	// a0 + a1 * x == 0
	// b00 + b01 * x + b10 * y + b20 * x ** 2 + b02 * y ** 2 == 0
	void solve_01_22(
		double a0, double a1
		double b00, double b01, double b10, double b20, double b02,
		double default_x, double default_y,
		const std::function<void(const geometry::Point&)>& sol
	) {
		const bool a1_is_small = std::abs(a1) < TOLERANCE;
		if (a1_is_small) {
			solve_cqlq(a0, b00, b01, b10, b20, b02, default_x, default_y, sol);
			return;
		}

		subs_qq(-a0 / a1, a1, b00, b01, b20, b02, default_y, sol);
	}

	// Solve:
	// a0 + a1 * x + a2 * x ** 2 == 0
	// b00 + b01 * x + b10 * y + b20 * x ** 2 + b02 * y ** 2 == 0
	void solve_02_22(
		double a0, double a1, double a2,
		double b00, double b01, double b10, double b20, double b02,
		double default_x, double default_y,
		const std::function<void(const geometry::Point&)>& sol
	) {
//		const bool a2_is_small = std::abs(a2) < TOLERANCE;
//		if (a2_is_small) {
//			solve_cllq(a0, a1,
//				b00, b01, b10, b20, b02,
//				default_x, default_y,
//				sol);
//			return;
//		}

		solve_q(
			a0, a1, a2,
			default_x,
			[sol&](const double x) {
				subs_qq(
					x,
					b00, b01, b10, b20, b02,
					default_y,
					sol);
			}
		);
	}

	// Solve:
	// a00 + a01 * x + a10 * y + a20 * x ** 2 == 0
	// b00 + a01 * x + b10 * y + b20 * x ** 2 + b02 * y ** 2 == 0
	void solve_12_22(
		double a00, double a01, double a10, double a20,
		double b00, double b01, double b10, double b20, double b02,
		double default_x, double default_y,
		const std::function<void(const geometry::Point&)>& sol
	) {
		const bool a10_is_small = std::abs(a10) < TOLERANCE;
		if (a10_is_small) {
			void solve_cqqq(
				a00, a10, a20,
				b00, b01, b10, b20, b02,
				default_x, default_y,
				sol
			);
		}

		// y = (a00 + a01 * x + a20 * x ** 2) / a10
		// Solve:
		// b00 + a01 * x + b10 * (a00 + a01 * x + a20 * x ** 2) / a10 + b20 * x ** 2
		// 			+ b02 * (a00 + a01 * x + a20 * x ** 2) ** 2 / a10 ** 2 == 0


		const double y =
	}

	// Solve:
	// a00 + a01 * x + a10 * y + a20 * x ** 2 + a02 * y ** 2 == 0
	// b00 + a01 * x + b10 * y + b20 * x ** 2 + b02 * y ** 2 == 0
	void solve_22_22(
		double a00, double a01, double a10, double a20, double a02,
		double b00, double b01, double b10, double b20, double b02,
		double default_x, double default_y
	) {
		const bool a02_is_small = std::abs(a02) < TOLERANCE;
		if (a02_is_small) {

		}

	}


//	/*
//	 * a * (x - cx) ** 2 + b * (y - cy) ** 2 + c
//	 * = a * x ** 2 = 2 * a
//	 */
//
//	// Solve:
//	// a00 + a01 * x + a10 * y + a20 * x ** 2 + a11 * x * y + a02 * y ** 2 == 0
//	// b00 + a01 * x + b10 * y + b20 * x ** 2 + b11 * x * y + b02 * y ** 2 == 0
//	geometry::MaybePoint solve(
//		double a00, double a01, double a10, double a11, double a20, double a02,
//		double b00, double b01, double b10, double b11, double b20, double b02,
//		double default_x, double default_y
//	) {
//
//
//	}

}


#endif //IDEA_SOLVE_QUADRATIC_2X2_H
