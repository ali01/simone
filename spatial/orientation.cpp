#include "orientation.h"

#include "../exception.h"

namespace Simone {

/* for implementation purposes only */
enum { NORTH=0, SOUTH, EAST, WEST };

const Orientation Orientation::kNorth(NORTH);
const Orientation Orientation::kSouth(SOUTH);
const Orientation Orientation::kEast(EAST);
const Orientation Orientation::kWest(WEST);

void
Orientation::valueIs(short v) {
  if (v < NORTH || v > WEST) {
    string msg = "unexpected value in initialization of orientation";
    throw Simone::RangeException(__FILE__, __LINE__, msg);
  }
}

} /* end of namespace Simone */
