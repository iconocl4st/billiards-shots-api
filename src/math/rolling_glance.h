//
// Created by thallock on 10/3/21.
//

#ifndef IDEA_ROLLING_GLANCE_H
#define IDEA_ROLLING_GLANCE_H

#include "billiards_common/math/polynomial.h"
#include "billiards_common/shots/RollingGlanceCalculation.h"

namespace billiards::shots::math {

	inline
	void get_glance_location(
		const double ax, const double ay,
		const double dx, const double dy,
		const double r,
		const std::function<void(const billiards::shots::RollingGlanceCalculation&)>& receiver
	) {
		const double ax2 = ax * ax;
		const double ax3 = ax2 * ax;
		const double ax4 = ax3 * ax;
		const double ay2 = ay * ay;
		const double ay3 = ay2 * ay;
		const double ay4 = ay3 * ay;
		const double dx2 = dx * dx;
		const double dy2 = dy * dy;
		const double r2 = r * r;
		const double na2 = ax * ax + ay * ay;
		if (std::abs(na2) < TOL) {
			// cue is distance 0 away...
			return;
		}
		if (std::abs(dx) < TOL && std::abs(dy) < TOL) {
			// cue is at destination...
			return;
		}

		const auto checker = [&](const RollingGlanceCalculation& rgc) {
//			const double tx = rgc.tan_dir.x;
//			const double ty = rgc.tan_dir.y;
//			const double aim_x = rgc.aim_dir.x;
//			const double aim_y = rgc.aim_dir.y;
//
//			const double s = (aim_x * tx + aim_y * ty) / (aim_x * aim_x + aim_y * aim_y);
//			const double alpha = 2 / 7.0;
//			double t;
//			if (std::abs(dx) >= TOL) {
//				t = (alpha * aim_x + (1 - alpha) * s * aim_x) / dx;
//			} else if (std::abs(dy) >= TOL) {
//				t = (alpha * aim_y + (1 - alpha) * s * aim_y) / dy;
//			} else {
//				throw std::runtime_error{"We already checked whether dx and dy are zero..."};
//			}
//
//			const double orthogonality_at_s = rgc.aim_dir.x * (s * rgc.aim_dir.x - tx) + rgc.aim_dir.y * (s * rgc.aim_dir.y - ty);
//			if (std::abs(orthogonality_at_s) > LARGER_TOL) {
//				return;
//			}
//
//			const double rolling_x = alpha * tx + (1 - alpha) * s * aim_x - t * dx;
//			if (std::abs(rolling_x) > LARGER_TOL) {
//				return;
//			}
//			const double rolling_y = alpha * ty + (1 - alpha) * s * aim_y - t * dy);
//			if (std::abs(rolling_y) > LARGER_TOL) {
//				return;
//			}
//			receiver(rgc);
		};

		do {
			const double c0 = -25/ (double) 49*ax2*ay2*dx2*r2 + 10/ (double) 7*ax3*ay*dx*dy*r2 + 20/ (double) 49*ax*ay3*dx*dy*r2 - ax4*dy2*r2 - 4/ (double) 7*ax2*ay2*dy2*r2 - 4/ (double) 49*ay4*dy2*r2;
			const double c2 = 4/ (double) 49*ax4*dx2 + 53/ (double) 49*ax2*ay2*dx2 + ay4*dx2 - 90/ (double) 49*ax3*ay*dx*dy - 90/ (double) 49*ax*ay3*dx*dy + ax4*dy2 + 53/ (double) 49*ax2*ay2*dy2 + 4/ (double) 49*ay4*dy2;
			if (std::abs(c2) < TOL) {
				break;
			}
			const double x2 = -c0 / c2;
			if (x2 < 0) {
				break;
			}
			const double x = std::sqrt(x2);
			const double rad = r * r - x * x;
			double y;
			if (rad > TOL) {
				y = std::sqrt(rad);
			} else if (rad > -TOL) {
				y = 0;
			} else {
				break;
			}
			const double tx = y;
			const double ty = -x;
			checker(RollingGlanceCalculation{x, y, tx, ty, x - ax, y - ay});
		} while (false);

		do {
			const double c0 = -25/ (double) 49*ax2*ay2*dx2*r2 + 10/ (double) 7*ax3*ay*dx*dy*r2 + 20/ (double) 49*ax*ay3*dx*dy*r2 - ax4*dy2*r2 - 4/ (double) 7*ax2*ay2*dy2*r2 - 4/ (double) 49*ay4*dy2*r2;
			const double c2 = 4/ (double) 49*ax4*dx2 + 53/ (double) 49*ax2*ay2*dx2 + ay4*dx2 - 90/ (double) 49*ax3*ay*dx*dy - 90/ (double) 49*ax*ay3*dx*dy + ax4*dy2 + 53/ (double) 49*ax2*ay2*dy2 + 4/ (double) 49*ay4*dy2;
			if (std::abs(c2) < TOL) {
				break;
			}
			const double x2 = -c0 / c2;
			if (x2 < 0) {
				break;
			}
			const double x = std::sqrt(x2);
			const double rad = r * r - x * x;
			double y;
			if (rad > TOL) {
				y = std::sqrt(rad);
			} else if (rad > -TOL) {
				y = 0;
			} else {
				break;
			}
			const double tx = -y;
			const double ty = x;
			checker(RollingGlanceCalculation{x, y, tx, ty, x - ax, y - ay});
		} while (false);
	}


	inline
	void get_glance_location(
		const geometry::MaybePoint& cue_loc,
		const geometry::MaybePoint& destination_loc,
		const double r,
		const std::function<void(const billiards::shots::RollingGlanceCalculation&)>& receiver
	) {
		if (!cue_loc.is_valid() || !destination_loc.is_valid()) {
			return;
		}
		get_glance_location(
			cue_loc.x.get(), cue_loc.y.get(),
			destination_loc.x.get(), destination_loc.y.get(),
			r,
			receiver
		);
	}


#if 0
	void old_code() {

		const double a2 = a * a;
		const double b2 = b * b;
		const double r2 = r * r;

		a * ((p0 - c0) * b - p1 + c1)
		+ 1 / r ** 2 * (1 - a) * (
			p0 ** 2 * (c0 - p0) * b - p1 ** 2 * (c1 - p1) + p0 * p1 * ((c1 - p1) * b - (c0 - p0))
		)
	}
#endif



/*
 * Sage code no longer needed...


eq_r
poly = eq_06.left()



for ass, tan_sol in enumerate([{'tx': y, 'ty': -x}, {'tx': -y, 'ty': x}]):
	print('\t// tangent assignment ' + str(ass))
	print('\t// ' + str(poly.substitute(**tan_sol)))
	solve(eq_06.substitute(**tan_sol), y)
	for name, coeff in zip(
		['c00', 'c10', 'c01', 'c20', 'c11', 'c02'],
		get_coefficients(poly.substitute(**tan_sol).expand())
	):
		print('\tconst double ' + name + ' = ' + convert_powers(coeff).replace('(std::pow(ax, 2) + std::pow(ay, 2))', 'na2'))


eq_11 = eq_dy / eq_dx
eq_12 = eq_11 * ((alpha - 1)*ax*s - alpha*tx)
eq_13 = eq_12 - ( (alpha - 1)*ay*s - alpha*ty )
eq_14 = eq_13.substitute(s=s_sol)
eq_15 = eq_14.substitute(alpha=2/7)
poly1 = eq_15.left()

for denom, poly in [('dy', poly0), ('dx', poly1)]:
	print('// ' + denum + ' cannot be 0')
	for ass, tan_sol in enumerate([{'tx': y, 'ty': -x}, {'tx': -y, 'ty': x}]):
		print('// tangent assignment ' + str(ass))
		for name, coeff in zip(
			['c00', 'c10', 'c01', 'c20', 'c11', 'c02'],
			get_coefficients(poly.substitute(**tan_sol).expand())
		):
			print('\tconst double ' + name + ' = ' + convert_powers(coeff).replace('(std::pow(ax, 2) + std::pow(ay, 2))', 'na2'))






#R = PolynomialRing(QQ, names='x, y')
# assume(ax, ay, dx, dy, r, 'real')

(eq_dx / eq_dy * ((alpha - 1)*ay*s - alpha*ty)).substitute(s=s_sol)
(eq_dx / eq_dy * ((alpha - 1)*ay*s - alpha*ty) - (ax*tx + ay*ty)*(alpha - 1)*ax/(ax^2 + ay^2) - alpha*tx).substitute(s=s_sol).simplify()


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



.replace('(std::pow(ax, 2) + std::pow(ay, 2))', 'na2')


 */
}






#endif //IDEA_ROLLING_GLANCE_H
