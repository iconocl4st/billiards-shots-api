//
// Created by thallock on 10/3/21.
//

#ifndef IDEA_ROLLING_GLANCE_H
#define IDEA_ROLLING_GLANCE_H

#include "billiards_common/math/polynomial.h"

namespace billiards::shots::math {
/*
docker run -it sagemath/sagemath:latest

import re
# TODO: should use std::sqrt and std::cbrt...
def convert_powers(expr):
	return re.sub(
	 	r'([a-zA-Z0-9]*)\^([0-9]+)', r'std::pow(\g<1>, \g<2>)', str(expr)
	).replace('/', '/ (double) ')

# Rolling glance direction.
# Using the model in https://billiards.colostate.edu/technical_proofs/new/TP_A-4.pdf


x, y = var('x y') # Location of cue at glance
# The object ball is centered at the origin
dx, dy = var('dx dy') # Location of the desired cue destination
r = var('r') # The sum of radaii of the cue and object balls
alpha = var('alpha') # This will be set to 2/7

# FOR TEST
# ax = -1; ay = 1; dx = 2; dy = -1; r = 0.5

# The tangent direction must be one of the following. Note, it will have magnitude 1.
tx, ty = var('tx ty')

# tx = y; ty = -x;
# t1x = -y; t0y = x;

# We need the location of the glance to be at radius r from the object ball
eq_r = x^2 + y^2 == r^2

# Let s represent the distance travelled along the aim line such it is
# orthogonal to the tangent line:
s = var('s')
eq_s = (s * ax) * (s * ax - tx) + (s * ay) * (s * ay - ty) == 0

# The rolling glance means that the direction from the glance point satisfies
t = var('t')
eq_dx = t * dx == (1 - alpha) * tx + alpha * s * ax
eq_dy = t * dy == (1 - alpha) * ty + alpha * s * ay

# We need to solve for x and y
# First, we solve for s. One solution is s = 0, so we divide it out.
s_sol = solve(eq_s / s, s)[0].right(); s_sol

expr = eq_dx / eq_dy; expr
expr = expr * ((alpha*ay*s - (alpha - 1)*ty)); expr
expr = expr - (alpha*ax*s - (alpha - 1)*tx); expr
expr = expr.left(); expr
expr = expr * dy; expr
expr = expr.expand(); expr
expr = expr.substitute(s=s_sol); expr
expr = expr * (ax^2 + ay^2); expr
expr = expr.simplify_rational(); expr
expr = expr.expand(); expr
expr = expr.substitute(alpha=2/7); expr

for ass, tan_sol in enumerate([{'tx': y, 'ty': -x}, {'tx': -y, 'ty': x}]):
	print('\t\t// tangent assignment ' + str(ass))
	print('\t\t// ' + str(expr.substitute(**tan_sol)))

	y_sol = solve(expr.substitute(**tan_sol), y)[0].right()
	expr2 = eq_r.substitute(y=y_sol)

	expr2 = expr2 - r^2
	expr2 = expr2 * (2*ax*ay*dx - (7*ax^2 + 5*ay^2)*dy)^2
	expr2 = expr2.simplify_rational()
	expr2 = expr2.left()
	expr2 = expr2.expand()
	expr2 = expr2.collect(x)
	for c, ord in expr2.coefficients(x):
		print('\t\tconst double c' + str(ord) + ' = ' + convert_powers(c.simplify())
			.replace('std::pow(ax, 2)', 'ax2')
			.replace('std::pow(ax, 3)', 'ax3')
			.replace('std::pow(ax, 4)', 'ax4')
			.replace('std::pow(ay, 2)', 'ay2')
			.replace('std::pow(ay, 3)', 'ay3')
			.replace('std::pow(ay, 4)', 'ay4')
			.replace('std::pow(dx, 2)', 'dx2')
			.replace('std::pow(dy, 2)', 'dy2')
			.replace('std::pow(r, 2)', 'r2')
		+ ';')

	 */

	void get_glance_location(
		const double ax, const double ay,
		const double dx, const double dy,
		const double r, const double default_x,
		const std::function<void(double, double)>& receiver
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

		const auto checker = [&](const double x, const double y) {
			const double tx = y;
			const double ty = -x;
			const double s = (ax * tx + ay * ty) / (ax * ax + ay * ay);
			const double alpha = 2 / 7.0;
//			eq_dx = t * dx == (1 - alpha) * tx + alpha * s * ax
//			eq_dy = t * dy == (1 - alpha) * ty + alpha * s * ay
			double t;
			if (std::abs(dx) >= TOL) {
				t = ((1 - alpha) * tx + alpha * s * ax) / dx;
			} else if (std::abs(dy) >= TOL) {
				t = ((1 - alpha) * ty + alpha * s * ay) / dy;
			} else {
				throw std::runtime_error{"We already checked whether dx and dy are zero..."};
			}
			if (std::abs((1 - alpha) * tx + alpha * s * ax - t * dx) > LARGER_TOL) {
				return;
			}
			if (std::abs((1 - alpha) * ty + alpha * s * ay - t * dy) > LARGER_TOL) {
				return;
			}
			receiver(x, y);
		};
		const auto compute_y = [&](const double x) {
			const double rad = r * r - x * x;
			if (std::abs(rad) < TOL) {
				checker(x, 0);
			} else {
				checker(x, std::sqrt(rad));
			}
		};

		const double c0 = -4*ax2*ay2*dx2*r2 + 28*ax3*ay*dx*dy*r2 + 20*ax*ay3*dx*dy*r2 - 49*ax4*dy2*r2 - 70*ax2*ay2*dy2*r2 - 25*ay4*dy2*r2;
		const double c2 = 25*ax4*dx2 + 74*ax2*ay2*dx2 + 49*ay4*dx2 - 48*ax3*ay*dx*dy - 48*ax*ay3*dx*dy + 49*ax4*dy2 + 74*ax2*ay2*dy2 + 25*ay4*dy2;
//		std::cout << c0 << " + " << c2 << " * x^2" << std::endl;
		billiards::math::solve_2(c0, 0, c2, default_x, compute_y);
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


def get_coefficients(expr):
	c00 = 0; c10 = 0; c01 = 0; c20 = 0; c11 = 0; c02 = 0;
	for term in expr.expand().operands():
		if   term.degree(x) == 0 and term.degree(y) == 0:
			c00 = c00 + term
		elif term.degree(x) == 1 and term.degree(y) == 0:
			c10 = c10 + term.coefficient(x)
		elif term.degree(x) == 0 and term.degree(y) == 1:
			c01 = c01 + term.coefficient(y)
		elif term.degree(x) == 2 and term.degree(y) == 0:
			c00 = c00 + term.coefficient(x^2)
		elif term.degree(x) == 1 and term.degree(y) == 1:
			c11 = c11 + term.coefficient(x*y)
		elif term.degree(y) == 0 and term.degree(y) == 2:
			c02 = c02 + term.coefficient(y^2)
		else:
			print(term)
			raise Exception('higher orders not supported')
	ret = [c00, c10, c01, c20, c11, c02]
	if not (c00 + c10 * x + c01 * y + c20 * x^2 + c11 * x * y + c02 * y^2 - expr).is_zero():
		raise Exception('Failure to get coefficients')
	return ret
 */
}






#endif //IDEA_ROLLING_GLANCE_H
