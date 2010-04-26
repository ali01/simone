#ifndef ORIENTATION_H_RLEFK5CK
#define ORIENTATION_H_RLEFK5CK

#include "../ptr_interface.h"
#include "../nominal.h"

namespace Simone {

class Orientation : public Simone::Nominal<Orientation,short> {
public:
  
  static const Orientation kNorth;
  static const Orientation kSouth;
  static const Orientation kEast;
  static const Orientation kWest;
  
  /* default constructor */
  Orientation();
  
  /* copy constructor */
  Orientation(const Orientation& _o);
  
private:
  Orientation(short _d);
  
  /* member functions */
  void valueIs(short v);
};

} /* end of namespace Simone */

#endif
