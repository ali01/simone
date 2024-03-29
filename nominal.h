/* Copyright (c) 2008-2010. Ali H. Yahya, All rights reserved. */
/* Adapted from David R. Cheriton's Advanced Object Oriented Programming from a
   Modeling & Simulation's Perspective ~ Chapter 16: Value-Oriented
   Programming & Value Types, Numerical class implementation. */

#ifndef NOMINAL_H_EI68A6E8
#define NOMINAL_H_EI68A6E8

#include <sstream>
#include <ostream>
#include <limits>
#include <string>
using std::stringstream;
using std::ostream;
using std::numeric_limits;
using std::string;

namespace Simone {

template<typename UnitType, typename RepType>
class Nominal {
public:
  /* constructors */
  Nominal() { valueIs(0); }
  Nominal(RepType v) { valueIs(v); }
  Nominal(const Nominal<UnitType,RepType>& v) { valueIs(v.value_); }

  /* default virtual destructor */
  virtual ~Nominal() {}

  /* member functions */
  
  RepType value() const { return value_; }
  
  virtual const string str() const {
    stringstream ss;
    ss << *this;
    return ss.str();
  }
  
  /* assignment */
  const Nominal<UnitType,RepType>&
    operator=(const Nominal<UnitType,RepType>& v);

  /* relational operators */
  
  bool operator==(const Nominal<UnitType,RepType>& v) const;
  bool operator!=(const Nominal<UnitType,RepType>& v) const;

  /* stream operator */
  friend ostream&
  operator<<(ostream& out, const Nominal<UnitType,RepType>& _n) {
    return out << _n.value_;
  }
protected:
  /* member functions */
  virtual void valueIs(RepType v) { value_ = v; }
  virtual bool equal(RepType v) const {
    return value_ == v;
  }

  /* data members */
  RepType value_;
};

/* assignment */
template<typename UnitType, typename RepType>
inline const Nominal<UnitType,RepType>&
Nominal<UnitType,RepType>::operator=(const Nominal<UnitType,RepType>& v) {
  if (this != &v) { valueIs(v.value_); }
  return v;
}

/* relational operators */

template<typename UnitType, typename RepType>
inline bool
Nominal<UnitType,RepType>::operator==(const Nominal<UnitType,RepType>& v) const {
  return equal(v.value());
}

template<typename UnitType, typename RepType>
inline bool
Nominal<UnitType,RepType>::operator!=(const Nominal<UnitType,RepType>& v) const {
  return !equal(v.value());
}

} /* end of namespace Simone */

#endif
