/* Copyright (c) 2008-2010. Ali H. Yahya, All rights reserved. */
/* Adapted from David R. Cheriton's Advanced Object Oriented Programming from a
   Modeling & Simulation's Perspective ~ Chapter 3: Events, Notifications, and
   Callbacks. */

#ifndef NAMED_INTERFACE_H_T2HL0ZA0
#define NAMED_INTERFACE_H_T2HL0ZA0

/* stl includes */
#include <string>

/* simone includes */
#include "ptr_interface.h"

namespace Simone {

class NamedInterface : public PtrInterface<NamedInterface> {
public:
  const string& name() const { return name_; }

protected:
  NamedInterface(const string& name) {}
  
private:
  string name_;
};

} /* end of namespace Simone */

#endif
