#pragma once
#include <cmath>

namespace Simone {
namespace Math {

const double kEpsilon = 0.00001;

bool equal(double lhs, double rhs) {
   return fabs(lhs - rhs) < kEpsilon;
}

} /* end of namespace Math */
} /* end of namespace Simone */
