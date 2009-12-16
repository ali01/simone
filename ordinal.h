/* Copyright (c) 2008-2010. Ali H. Yahya, All rights reserved. */

#ifndef ORDINAL_H_ENSZR7PR
#define ORDINAL_H_ENSZR7PR

#include <ostream>

#include "thread/concurrent_collection_element.h"

namespace Simone {
using std::ostream;

template <typename UnitType,typename RepType>
class Ordinal : public thread::ConcurrentCollectionElement {
public:
   Ordinal(RepType _v) : value_(_v) {}
   Ordinal(const Ordinal<UnitType,RepType>& _o) : value_(_o.value()) {}
   virtual Ordinal<UnitType,RepType>& operator=(const Ordinal<UnitType,RepType>& _o) {
      if (this != &_o) { value_ = _o.value(); }
      return *this;
   }
   virtual bool operator==(const Ordinal<UnitType,RepType>& _o) const
                                                     { return value() == _o.value(); }
                                                     
   virtual bool operator!=(const Ordinal<UnitType,RepType>& _o) const
                                                     { return value() != _o.value(); }
                                                     
   virtual bool operator< (const Ordinal<UnitType,RepType>& _o) const
                                                     { return value() <  _o.value(); }
                                                     
   virtual bool operator<=(const Ordinal<UnitType,RepType>& _o) const
                                                     { return value() <= _o.value(); }
                                                     
   virtual bool operator> (const Ordinal<UnitType,RepType>& _o) const
                                                     { return value() >  _o.value(); }
                                                     
   virtual bool operator>=(const Ordinal<UnitType,RepType>& _o) const
                                                     { return value() >= _o.value(); }
                                                     
   friend ostream& operator<<(ostream& out, const Ordinal<UnitType,RepType>& _o) 
                                                     { return out << _o.value(); }

   virtual RepType value() const { return value_; }
protected:
   // data members ===================================================================
   RepType value_;
};

} /* end of namespace Simone */

#endif
