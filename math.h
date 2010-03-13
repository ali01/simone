/* Copyright (c) 2008-2010. Ali H. Yahya, All rights reserved. */

#ifndef MATH_H_NJRC8OE6
#define MATH_H_NJRC8OE6

#include <cmath>

namespace Simone {
namespace math {

const double kEpsilon = 0.00001;

inline bool equal(double lhs, double rhs) {
   return fabs(lhs - rhs) < kEpsilon;
}

} /* end of namespace math */
} /* end of namespace Simone */


#endif
