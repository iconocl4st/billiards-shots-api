//
// Created by thallock on 9/28/21.
//

#ifndef IDEA_SIMPLE_CALCULATIONS_H
#define IDEA_SIMPLE_CALCULATIONS_H

#include "common/geometry/geometry.h"

namespace billiards::geometry {

	inline
	MaybeBool on_same_side(
		const MaybePoint& s1, const MaybePoint& s2,
		const MaybePoint& p1, const MaybePoint& p2
	) {
		auto line = through(s1, s2);
		auto d1 = line.at(p1);
		auto d2 = line.at(p2);
		return (
			d1.abs() < TOLERANCE ||
			d2.abs() < TOLERANCE ||
			d1.signum() == d2.signum()
		);
	}

// distance to project onto point?
	MaybeBool points_are_colinear(const MaybePoint& p1, const MaybePoint& p2, const MaybePoint& p3) {
		const auto s1 = p1.y - p2.y;
		const auto s2 = p2.x - p1.x;
		const auto s3 = p1.x * p2.y - p1.y * p2.x;
//		if (std::abs(s3) < TOL) {
//			//
//		}
		const auto d = p3.x * s1 + p3.y * s2 + s3;
		return d.abs() < TOLERANCE;
	}

	inline
	MaybePoint extend_line(const MaybePoint& p1, const MaybePoint& p2, const MaybeDouble& d) {
		auto diff = p2 - p1;
		return p1 + diff * (d / diff.norm() + 1);
	}
}

#endif //IDEA_SIMPLE_CALCULATIONS_H
