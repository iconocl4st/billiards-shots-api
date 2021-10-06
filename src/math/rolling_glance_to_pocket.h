//
// Created by thallock on 10/5/21.
//

#ifndef IDEA_ROLLING_GLANCE_UNKNOWN_DEST_H
#define IDEA_ROLLING_GLANCE_UNKNOWN_DEST_H

namespace billiards::shots::math {

/*
docker run -it sagemath/sagemath:latest

import re
# TODO: should use std::sqrt and std::cbrt...
def convert_powers(expr):
	return re.sub(
	 	r'([a-zA-Z0-9]*)\^([0-9]+)', r'std::pow(\g<1>, \g<2>)', str(expr)
	).replace('/', '/(double) ')

def replace_vars(e):
	return (e
		.replace('std::pow(r1, 2)', 'r12')
		.replace('std::pow(r2, 2)', 'r22')
		.replace('std::pow(px, 2)', 'px2')
		.replace('std::pow(px, 3)', 'px3')
		.replace('std::pow(py, 2)', 'py2')
		.replace('std::pow(py, 3)', 'py3')
		.replace('std::pow(ax, 2)', 'ax2')
		.replace('std::pow(ay, 2)', 'ay2')
		.replace('px2 + py2', 'px2ppy2')
		.replace('ax2 + ay2', 'ax2pay2')
		.replace('sqrt(px2ppy2 - r12)', 'sradi')
		.replace('(double) (px2ppy2)', 'px2ppy2')
		.replace('(double) (ax2pay2)', 'ax2pay2')
		.replace('(double) py', 'py')
		.replace('(double) px', 'px')
	)

# Rolling glance direction.
# Using the model in https://billiards.colostate.edu/technical_proofs/new/TP_A-4.pdf


# The object ball is centered at the origin
x, y = var('x y') # Location of cue at glance
dx, dy = var('dx dy') # Location of the unknown desired cue destination
ax, ay = var('ax ay') # Location of the current cue position
px, py = var('px py') # Location of the pocket
r1 = var('r1') # The sum of radius of the cue ball
r2 = var('r2') # The sum of radius of the object ball
alpha = var('alpha') # This will be set to 2/7

# FOR TEST
# ax = -1; ay = 1; dx = 2; dy = -1; r = 0.5

# The tangent direction must be one of the following. Note, it will have magnitude (r1 + r2) ^ 2.
tx, ty = var('tx ty')
# tx = y; ty = -x;
# t1x = -y; t0y = x;

# We need the location of the glance to be at radius r1 + r2 from the object ball
eq_r1 = x^2 + y^2 == (r1 + r2)^2

# Let s1 represent the distance travelled along the aim line such it is
# orthogonal to the tangent line:
s = var('s')
eq_s = (s * ax) * (s * ax - tx) + (s * ay) * (s * ay - ty) == 0

# The desired cue location is orthogonal to the pocket:
eq_o = dx * (dx - px) + dy * (dy - py) == 0

# That point must be at distance r1 from the pocket
eq_r2 = (dx - px)^2 + (dy - py)^2 == r1^2

# The rolling glance means that the direction from the glance point satisfies
eq_dx = dx - x == (1 - alpha) * tx + alpha * s * ax
eq_dy = dy - y == (1 - alpha) * ty + alpha * s * ay

# We need to solve for x, y, dx, dy
# First, we solve for s. One solution is s = 0, so we divide it out.
s_sol = solve(eq_s / s, s)[0].right(); s_sol

expr1 = eq_r2;
expr1 = expr1 - r1^2
expr1 = expr1.left()
expr1 = expr1.expand()

expr2 = eq_o
expr2 = eq_o.left()
expr2 = expr2.expand()

dx_sol_in_dy = solve(expr1 - expr2, dx)[0].right()
nz_px_dy_sols = solve(eq_o.substitute(dx=dx_sol_in_dy), dy)
nz_px_dy_sol1 = nz_px_dy_sols[0].right()
nz_px_dy_sol2 = nz_px_dy_sols[1].right()
nz_px_dx_sol1 = dx_sol_in_dy.substitute(dy=nz_px_dy_sol1)
nz_px_dx_sol2 = dx_sol_in_dy.substitute(dy=nz_px_dy_sol2)

dy_sol_in_dx = solve(expr1 - expr2, dy)[0].right()
nz_py_dx_sols = solve(eq_o.substitute(dy=dy_sol_in_dx), dx)
nz_py_dx_sol1 = nz_py_dx_sols[0].right()
nz_py_dx_sol2 = nz_py_dx_sols[1].right()
nz_py_dy_sol1 = dy_sol_in_dx.substitute(dx=nz_py_dx_sol1)
nz_py_dy_sol2 = dy_sol_in_dx.substitute(dx=nz_py_dx_sol2)

expr3 = eq_dx; expr4 = eq_dy
expr3 = expr3 - dx + x; expr4 = expr4 - dy + y
expr3 = expr3.right(); expr4 = expr4.right()
expr3 = expr3.substitute(s=s_sol); expr4 = expr4.substitute(s=s_sol)
expr3 = expr3 * (ax^2 + ay^2); expr4 = expr4 * (ax^2 + ay^2)
expr3 = expr3.simplify_rational(); expr4 = expr4.simplify_rational()
expr3 = expr3.substitute(alpha=2/7); expr4 = expr4.substitute(alpha=2/7)

for assumption, sols in [
	('if (std::abs(px) > TOL) {', [(nz_px_dx_sol1, nz_px_dy_sol1), (nz_px_dx_sol2, nz_px_dy_sol2)]),
	('} else if (std::abs(py) > TOL) {', [(nz_py_dx_sol1, nz_py_dy_sol1), (nz_py_dx_sol2, nz_py_dy_sol2)]),
	('}', []),
]:
	print('\t\t' + assumption)
	for dx_sol, dy_sol in sols:
		print('\t\t\td_sols.emplace_back(')
		print('\t\t\t\tfalse,')
		print('\t\t\t\t' + replace_vars(convert_powers(dx_sol)) + ',')
		print('\t\t\t\t' + replace_vars(convert_powers(dy_sol)) + ');')

for tan_sol in [{'tx': y, 'ty': -x}, {'tx': -y, 'ty': x}]:
	print('\t\t\t\t// tangent solution tx=' + str(tan_sol['tx']) + ', ty=' + str(tan_sol['ty']))
	expr5 = expr3; expr6 = expr4;
	expr5 = expr5.substitute(**tan_sol); expr6 = expr6.substitute(**tan_sol)
	xy_sols = solve([expr5 == 0, expr6 == 0], x, y)[0]
	x_sol = xy_sols[0].right(); y_sol = xy_sols[1].right()
	print('\t\t\t\txy_sols.emplace_back(')
	print('\t\t\t\t\t' + replace_vars(convert_powers(x_sol)) + ',')
	print('\t\t\t\t\t' + replace_vars(convert_powers(y_sol)) + ');')

 */


	class DestLocation {
	public:
		bool same_direction;
		double dx;
		double dy;

		DestLocation(bool b, double dx, double dy) : same_direction{b}, dx{dx}, dy{dy} {}
		~DestLocation() = default;
	};

	class GlanceSolution {
	public:
		double x, y, dx, dy;
		GlanceSolution(double x, double y, double dx, double dy)
			: x{x}
			, y{y}
			, dx{dx}
			, dy{dy}
		{}

		const bool same_as(const GlanceSolution& other) const {
			return (
				std::abs(x - other.x) < TOLERANCE &&
				std::abs(y - other.y) < TOLERANCE &&
				std::abs(dx - other.dx) < TOLERANCE &&
				std::abs(dy - other.dy) < TOLERANCE
			);
		}
	};

	void solve_glance_and_pocket(
		const double ax, const double ay,
		const double px, const double py,
		const double opx, const double opy,
		const double r1, const double r2,
		const std::function<void(const GlanceSolution& sol)>& receiver
	) {
		const double px2 = px * px;
		const double px3 = px2 * px;
		const double py2 = py * py;
		const double py3 = py2 * py;
		const double r12 = r1 * r1;
		const double r22 = r2 * r2;
		const double px2ppy2 = px2 + py2;
		const double ax2pay2 = ax2 + ay2;
		const double radi = px2ppy2 - r12;
		if (radi < 0) {
			// The pocket is too close to the object ball
			return;
		}
		const double sradi = std::sqrt(radi);
		if (px2ppy2 < TOL) {
			// The object is at the pocket
			return;
		}
		if (ax2pay2 < TOL) {
			// the cue is at the object
			return;
		}

		std::list<GlanceSolution> unique_sols;
		const auto unique_checker = [&](const GlanceSolution& g) {
			for (const GlanceSolution& prev: unique_sols) {
				if (prev.same_as(g)) {
					return;
				}
			}
			unique_sols.push_back(g);
			receiver(g);
		};
		const auto check_sol = [&](const GlanceSolution& g) {
			unique_checker(g);
		};
		std::list<DestLocation> d_sols;
		d_sols.emplace_back(true, dx + r1 * (opx - dx), dy + r2 * (opy - dy));
		// These all assume that x * px + y * py < 0
		if (std::abs(px) > TOL) {
			d_sols.emplace_back(
				false,
				(px2ppy2 - r12 - (px2*py + py3 - py*r12 - sradi*px*r1)*py/px2ppy2)/px,
				(px2*py + py3 - py*r12 - sradi*px*r1)/px2ppy2);
			d_sols.emplace_back(
				false,
				(px2ppy2 - r12 - (px2*py + py3 - py*r12 + sradi*px*r1)*py/px2ppy2)/px,
				(px2*py + py3 - py*r12 + sradi*px*r1)/px2ppy2);
		} else if (std::abs(py) > TOL) {
			d_sols.emplace_back(
				false,
				(px3 + px*py2 - px*r12 - sradi*py*r1)/px2ppy2,
				(px2ppy2 - r12 - (px3 + px*py2 - px*r12 - sradi*py*r1)*px/px2ppy2)/py);
			d_sols.emplace_back(
				false,
				(px3 + px*py2 - px*r12 + sradi*py*r1)/px2ppy2,
				(px2ppy2 - r12 - (px3 + px*py2 - px*r12 + sradi*py*r1)*px/px2ppy2)/py);
		}

		for (const auto& d_sol : d_sols) {
			const double dx = d_sol.dx;
			const double dy = d_sol.dy;

			std::list<std::pair<double, double>> xy_sols;
			// tangent solution tx=y, ty=-x
			// tangent solution tx=y, ty=-x
			xy_sols.emplace_back(
				-1/(double) 12*(7*ax2*dy + 5*ay2*dy - (7*ax2 + 2*ax*ay + 7*ay2)*dx)/ax2pay2,
				1/(double) 12*(7*ax2*dy - 2*ax*ay*dy + 7*ay2*dy + (5*ax2 + 7*ay2)*dx)/ax2pay2);
			// tangent solution tx=-y, ty=x
			xy_sols.emplace_back(
				1/(double) 12*(7*ax2*dy + 5*ay2*dy + (7*ax2 - 2*ax*ay + 7*ay2)*dx)/ax2pay2,
				1/(double) 12*(7*ax2*dy + 2*ax*ay*dy + 7*ay2*dy - (5*ax2 + 7*ay2)*dx)/ax2pay2);

			for (const auto xy_sol& : xy_sols) {
				const double x = xy_sol.first;
				const double y = xy_sol.second;
				bool is_same_direction = opx * (x - px) + opy * (y - opy) >= 0;
				if (d_sol.same_direction != is_same_direction) {
					continue;
				}
				// TODO: move dx, dy to be along the correct line...
				check_sol(GlanceSolution{x, y, dx, dy});
			}
		}
	}
/*
 * No longer needed:
 *
 *


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

		expr5 = expr3.substitute(dx=dx_sol, dy=dy_sol); expr6 = expr4.substitute(dx=dx_sol, dy=dy_sol)
		expr5 = expr5 * px * (px^2 + py^2); expr6 = expr6 * py * (px^2 + py^2)
		expr5 = expr5.simplify_rational(); expr6 = expr6.simplify_rational()
		for tan_sol in [{'tx': y, 'ty': -x}, {'tx': -y, 'ty': x}]:
			print('\t\t// tangent solution tx=' + str(tan_sol['tx']) + ', ty=' + str(tan_sol['ty']))
			expr5 = expr5.substitute(**tan_sol); expr6 = expr6.substitute(**tan_sol)
			expr5 = expr5.expand(); expr6 = expr6.expand()
			xy_sols = solve([expr5, expr6], x, y)[0]
			x_sol = xy_sols[0].right(); y_sol = xy_sols[1].right()
			x_sol = x_sol * ((ax^2 + ay^2)*px^2 + (ax^2 + ay^2)*py^2)
			y_sol = y_sol * ((ax^2 + ay^2)*px^2 + (ax^2 + ay^2)*py^2)
			x_sol = x_sol.simplify_rational(); y_sol = y_sol.simplify_rational()

		print('\t\t{')
		print('\t\t\tconst double dx = ' + replace_vars(convert_powers(dx_sol)) + ';')
		print('\t\t\tconst double dy = ' + replace_vars(convert_powers(dy_sol)) + ';')
		print('\t\t\tconst double x = ' + replace_vars(convert_powers(x_sol)) + ';')
		print('\t\t\tconst double y = ' + replace_vars(convert_powers(y_sol)) + ';')
		print('\t\t}')








		#for name, c in zip(
		#	['a00', 'a10', 'a01', 'a20', 'a11', 'a02'],
		#	get_coefficients(expr5)
		#):
		#	print(name, c)
		#for name, c in zip(
		#	['b00', 'b10', 'b01', 'b20', 'b11', 'b02'],
		#	get_coefficients(expr6)
		#):
		#	print(name, c)




# Still need to satisfy eq_dx eq_dy eq_r1 eq_r2

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
}
*/
}

#endif //IDEA_ROLLING_GLANCE_UNKNOWN_DEST_H
