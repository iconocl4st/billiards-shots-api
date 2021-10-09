//
// Created by thallock on 10/9/21.
//

#ifndef IDEA_HIGH_ORDER_POLYS_H
#define IDEA_HIGH_ORDER_POLYS_H

#include <iostream>
#include <Eigen/Dense>

namespace billiards::shots::math {
	void foo() {
		Eigen::Matrix3f A;
		Eigen::Vector3f b;
		A << 1,2,3,  4,5,6,  7,8,10;
		b << 3, 3, 4;
		std::cout << "Here is the matrix A:\n" << A << std::endl;
		std::cout << "Here is the vector b:\n" << b << std::endl;
		Eigen::Vector3f x = A.colPivHouseholderQr().solve(b);
		std::cout << "The solution is:\n" << x << std::endl;
	}
}

#endif //IDEA_HIGH_ORDER_POLYS_H
