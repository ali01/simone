#ifndef ORIENTATION_H_RLEFK5CK
#define ORIENTATION_H_RLEFK5CK

#include "ptr_interface.h"
#include "nominal.h"

namespace Simone {

class Orientation : public Simone::Nominal<Orientation,short> {
public:
  
  static const Orientation kNorth;
  static const Orientation kSouth;
  static const Orientation kEast;
  static const Orientation kWest;
  
  /* copy constructor */
  Orientation(const Orientation& _o);
  
private:
  Orientation(short _d);
  
  /* member functions */
  void valueIs(short v);
};

inline
Orientation::Orientation(const Orientation& _o) : 
  Simone::Nominal<Orientation,short>(_o.value()) {}

inline
Orientation::Orientation(short _d) :
  Simone::Nominal<Orientation,short>(_d) {}

} /* end of namespace Simone */

#endif
