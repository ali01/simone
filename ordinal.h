/* Copyright (c) 2008-2010. Ali H. Yahya, All rights reserved. */
/* Adapted from David R. Cheriton's Advanced Object Oriented Programming from a
   Modeling & Simulation's Perspective ~ Chapter 16: Value-Oriented
   Programming & Value Types, Numerical class implementation. */

#ifndef ORDINAL_H_G09I1KEU
#define ORDINAL_H_G09I1KEU

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
class Ordinal {
public:
  /* constructors */
  Ordinal() { valueIs(0); }
  Ordinal(RepType v) { valueIs(v); }
  Ordinal(const Ordinal<UnitType,RepType>& v) { valueIs(v.value_); }

  /* default virtual destructor */
  virtual ~Ordinal() {}

  /* member functions */
  
  RepType value() const { return value_; }
  
  virtual const string str() const {
    stringstream ss;
    ss << *this;
    return ss.str();
  }
  
  /* static functions */
  
  static RepType maxValue() {
    #ifdef WIN32
    #undef max
    #endif
    return numeric_limits<RepType>::max();
  }

  static RepType minValue() {
    #ifdef WIN32
    #undef min
    #endif
    return numeric_limits<RepType>::min();
  }
  
  /* assignment */
  
  const Ordinal<UnitType,RepType>&
    operator=(const Ordinal<UnitType,RepType>& v);

  /* relational operators */
  
  bool operator==(const Ordinal<UnitType,RepType>& v) const;
  bool operator!=(const Ordinal<UnitType,RepType>& v) const;
  bool operator>=(const Ordinal<UnitType,RepType>& v) const;
  bool operator<=(const Ordinal<UnitType,RepType>& v) const;
  bool operator< (const Ordinal<UnitType,RepType>& v) const;
  bool operator> (const Ordinal<UnitType,RepType>& v) const;

  /* stream operator */

  friend
  ostream& operator<<(ostream& out, const Ordinal<UnitType,RepType>& _n) {
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
inline const Ordinal<UnitType,RepType>&
Ordinal<UnitType,RepType>::operator=(const Ordinal<UnitType,RepType>& v) {
  if (this != &v) { valueIs(v.value_); }
  return v;
}

/* relational operators */

template<typename UnitType, typename RepType>
inline bool
Ordinal<UnitType,RepType>::operator==(const Ordinal<UnitType,RepType>& v) const {
  return equal(v.value());
}

template<typename UnitType, typename RepType>
inline bool
Ordinal<UnitType,RepType>::operator!=(const Ordinal<UnitType,RepType>& v) const {
  return !equal(v.value());
}

template<typename UnitType, typename RepType>
inline bool
Ordinal<UnitType,RepType>::operator<=(const Ordinal<UnitType,RepType>& v) const {
  return value_ <= v.value_;
}

template<typename UnitType, typename RepType>
inline bool
Ordinal<UnitType,RepType>::operator>=(const Ordinal<UnitType,RepType>& v) const {
  return value_ >= v.value_;
}

template<typename UnitType, typename RepType>
inline bool
Ordinal<UnitType,RepType>::operator<(const Ordinal<UnitType,RepType>& v) const {
  return value_ < v.value_;
}

template<typename UnitType, typename RepType>
inline bool
Ordinal<UnitType,RepType>::operator>(const Ordinal<UnitType,RepType>& v) const {
  return value_ > v.value_;
}

} /* end of namespace Simone */

#endif
