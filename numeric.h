/* Copyright (c) 2008-2010. Ali H. Yahya, All rights reserved. */
/* Adapted from David R. Cheriton's Advanced Object Oriented Programming from a
  Modeling & Simulation's Perspective ~ Chapter 16: Value-Oriented Programming &
  Value Types, Numerical class implementation. */

#ifndef NUMERIC_H_96GBE836
#define NUMERIC_H_96GBE836

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
class Numeric {
public:
  /* constructors */
  Numeric() { valueIs(0); }
  Numeric(RepType v) { valueIs(v); }
  Numeric(const Numeric<UnitType,RepType>& v) { valueIs(v.value_); }

  /* default virtual destructor */
  virtual ~Numeric() {}

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
      return numeric_limits<RepType>::max();
    #else
      return numeric_limits<RepType>::max();
    #endif
  }

  static RepType minValue() {
    #ifdef WIN32
    #undef min
      return numeric_limits<RepType>::min();
    #else
      return numeric_limits<RepType>::min();
    #endif
  }
  
  /* assignment */
  
  const Numeric<UnitType,RepType>&
    operator=(const Numeric<UnitType,RepType>& v);

  /* summation */

  const RepType operator+(const Numeric<UnitType,RepType>& v) const;

  /* prefix ++ */
  const Numeric<UnitType,RepType>& operator++();

  /* postfix ++ */
  const Numeric<UnitType,RepType> operator++(int);

  const Numeric<UnitType,RepType>&
    operator+=(const Numeric<UnitType,RepType>& v);

  /* subtraction */

  const RepType operator-(const Numeric<UnitType,RepType>& v) const;

  /* prefix -- */
  const Numeric<UnitType,RepType>& operator--();

  /* postfix -- */
  const Numeric<UnitType,RepType> operator--(int);

  const Numeric<UnitType,RepType>&
    operator-=(const Numeric<UnitType,RepType>& v);

  /* multiplication */

  const RepType operator*(const Numeric<UnitType,RepType>& v) const;

  const Numeric<UnitType,RepType>&
    operator*=(const Numeric<UnitType,RepType>& v);

  /* division */
  
  const RepType operator/(const Numeric<UnitType,RepType>& v) const;

  const Numeric<UnitType,RepType>&
    operator/=(const Numeric<UnitType,RepType>& v);

  /* modulus */
  
  const RepType
  operator%(const Numeric<UnitType,RepType>& v) const;

  const Numeric<UnitType,RepType>&
  operator%=(const Numeric<UnitType,RepType>& v);

  /* relational operators */
  
  bool operator==(const Numeric<UnitType,RepType>& v) const;
  bool operator!=(const Numeric<UnitType,RepType>& v) const;
  bool operator>=(const Numeric<UnitType,RepType>& v) const;
  bool operator<=(const Numeric<UnitType,RepType>& v) const;
  bool operator< (const Numeric<UnitType,RepType>& v) const;
  bool operator> (const Numeric<UnitType,RepType>& v) const;

  /* stream operator */

  friend
  ostream& operator<<(ostream& out, const Numeric<UnitType,RepType>& _n) {
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

/* arithmetic operators */

template<typename UnitType, typename RepType>
inline const Numeric<UnitType,RepType>&
Numeric<UnitType,RepType>::operator=(const Numeric<UnitType,RepType>& v) {
  if (this != &v) { valueIs(v.value_); }
  return v;
}

template<typename UnitType, typename RepType>
inline const RepType
Numeric<UnitType,RepType>::operator+(const Numeric<UnitType,RepType>& v) const {
  return Numeric<UnitType,RepType>(value_ + v.value_);
}

template<typename UnitType, typename RepType>
inline const Numeric<UnitType,RepType>&
Numeric<UnitType,RepType>::operator++() {
  valueIs(value_ + 1);
  return *this;
}

/* postfix ++ */
template<typename UnitType, typename RepType>
inline const Numeric<UnitType,RepType> Numeric<UnitType,RepType>::operator++(int) {
   Numeric<UnitType,RepType> prev(*this);
   valueIs(value_ + 1);
   return prev;
}

template<typename UnitType, typename RepType>
inline const
Numeric<UnitType,RepType>&
Numeric<UnitType,RepType>::operator+=(const Numeric<UnitType,RepType>& v) {
  valueIs(value_ + v.value_);
  return *this;
}

/* unary minus */
template<typename UnitType, typename RepType>
inline const Numeric<UnitType,RepType>
operator-(const Numeric<UnitType,RepType>& v) {
   return Numeric<UnitType,RepType>(-1 * v.value());
}

template<typename UnitType, typename RepType>
inline const RepType
Numeric<UnitType,RepType>::operator-(const Numeric<UnitType,RepType>& v) const {
  return Numeric<UnitType,RepType>(value_ - v.value_);
}

template<typename UnitType, typename RepType>
inline const Numeric<UnitType,RepType>&
Numeric<UnitType,RepType>::operator--() {
  valueIs(value_ - 1);
  return *this;
}

template<typename UnitType, typename RepType>
inline const Numeric<UnitType,RepType> Numeric<UnitType,RepType>::operator--(int) {
   Numeric<UnitType,RepType> prev = *this;
   valueIs(value_ - 1);
   return prev;
}

template<typename UnitType, typename RepType>
inline const
Numeric<UnitType,RepType>&
Numeric<UnitType,RepType>::operator-=(const Numeric<UnitType,RepType>& v) {
  valueIs(value_ - v.value_);
  return *this;
}

template<typename UnitType, typename RepType>
inline const RepType
Numeric<UnitType,RepType>::operator*(const Numeric<UnitType,RepType>& v) const {
  return Numeric<UnitType,RepType>(value_ * v.value_);
}

template<typename UnitType, typename RepType>
inline const
Numeric<UnitType,RepType>&
Numeric<UnitType,RepType>::operator*=(const Numeric<UnitType,RepType>& v) {
  valueIs(value_ * v.value_);
  return *this;
}

template<typename UnitType, typename RepType>
inline const RepType
Numeric<UnitType,RepType>::operator/(const Numeric<UnitType,RepType>& v) const {
  return Numeric<UnitType,RepType>(value_ / v.value_);
}

template<typename UnitType, typename RepType>
inline const
Numeric<UnitType,RepType>&
Numeric<UnitType,RepType>::operator/=(const Numeric<UnitType,RepType>& v) {
  valueIs(value_ / v.value_);
  return *this;
}

template<typename UnitType, typename RepType>
inline const RepType
Numeric<UnitType,RepType>::operator%(const Numeric<UnitType,RepType>& v) const {
  return Numeric<UnitType,RepType>(value_ % v.value_);
}

template<typename UnitType, typename RepType>
inline const
Numeric<UnitType,RepType>&
Numeric<UnitType,RepType>::operator%=(const Numeric<UnitType,RepType>& v) {
  valueIs(value_ % v.value_);
  return *this;
}

/* relational operators */

template<typename UnitType, typename RepType>
inline bool
Numeric<UnitType,RepType>::operator==(const Numeric<UnitType,RepType>& v) const {
  return equal(v.value());
}

template<typename UnitType, typename RepType>
inline bool
Numeric<UnitType,RepType>::operator!=(const Numeric<UnitType,RepType>& v) const {
  return !equal(v.value());
}

template<typename UnitType, typename RepType>
inline bool
Numeric<UnitType,RepType>::operator<=(const Numeric<UnitType,RepType>& v) const {
  return value_ <= v.value_;
}

template<typename UnitType, typename RepType>
inline bool
Numeric<UnitType,RepType>::operator>=(const Numeric<UnitType,RepType>& v) const {
  return value_ >= v.value_;
}

template<typename UnitType, typename RepType>
inline bool
Numeric<UnitType,RepType>::operator<(const Numeric<UnitType,RepType>& v) const {
  return value_ < v.value_;
}

template<typename UnitType, typename RepType>
inline bool
Numeric<UnitType,RepType>::operator>(const Numeric<UnitType,RepType>& v) const {
  return value_ > v.value_;
}

} /* end of namespace Simone */

#endif
