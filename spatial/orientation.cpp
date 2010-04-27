#include "orientation.h"

#include "../exception.h"

namespace Simone {

/* for implementation purposes only */
enum { NORTH=0, SOUTH, EAST, WEST };

const Orientation Orientation::kNorth(NORTH);
const Orientation Orientation::kSouth(SOUTH);
const Orientation Orientation::kEast(EAST);
const Orientation Orientation::kWest(WEST);

Orientation::Orientation() : Simone::Nominal<Orientation,short>(NORTH) {}

Orientation::Orientation(const Orientation& _o) : 
  Simone::Nominal<Orientation,short>(_o.value()) {}

Orientation::Orientation(short _d) :
  Simone::Nominal<Orientation,short>(_d) {}

void
Orientation::valueIs(short v) {
  if (v < NORTH || v > WEST) {
    string msg = "unexpected value in initialization of orientation";
    throw Simone::RangeException(__FILE__, __LINE__, msg);
  }
  Simone::Nominal<Orientation,short>::valueIs(v);
}

} /* end of namespace Simone */
