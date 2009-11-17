/* Adapted from David R. Cheriton's Advanced Object Oriented Programming from a 
   Modeling & Simulation's Perspective ~ Chapter 16: Value-Oriented Programming &
   Value Types, Numerical class implementation. */

#ifndef NUMERIC_H_96GBE836
#define NUMERIC_H_96GBE836

#include <ostream>

namespace Simone {
using std::ostream;

template<typename UnitType, typename BaseType>
class Numeric {
public:
   // # allocation ===================================================================
   Numeric()           { valueIs(0); }
   Numeric(BaseType v) { valueIs(v); }
   Numeric(const Numeric<UnitType,BaseType>& v) { valueIs(v.value_); }
   virtual ~Numeric() {}
   
   // # assignment operator ----------------------------------------------------------
   const Numeric<UnitType,BaseType>&
            operator= (const Numeric<UnitType,BaseType>& v);
   
   // # member functions =============================================================
   BaseType     value() const { return value_; }
   // # arithmetic operators =========================================================
   // # summation
   const BaseType operator+ (const Numeric<UnitType,BaseType>& v) const;
   const Numeric<UnitType,BaseType>&
            operator++();
   // const Numeric<UnitType,BaseType>&
   //          operator++(int);
   const Numeric<UnitType,BaseType>&
            operator+=(const Numeric<UnitType,BaseType>& v);
   
   // # subtraction
   // friend const Numeric<UnitType,BaseType> // unary minus
   //          operator-(const Numeric<UnitType,BaseType>& v);
   const BaseType operator- (const Numeric<UnitType,BaseType>& v) const;
   
   const Numeric<UnitType,BaseType>&
            operator--();
   // const Numeric<UnitType,BaseType>&
   //          operator--(int);
   const Numeric<UnitType,BaseType>&
            operator-=(const Numeric<UnitType,BaseType>& v);
   
   // # multiplication
   const BaseType operator* (const Numeric<UnitType,BaseType>& v) const;
   const Numeric<UnitType,BaseType>&
            operator*=(const Numeric<UnitType,BaseType>& v);
   
   // # division
   const BaseType operator/ (const Numeric<UnitType,BaseType>& v) const;
   
   const Numeric<UnitType,BaseType>&
            operator/=(const Numeric<UnitType,BaseType>& v);
   
   // # modulus
   const BaseType operator% (const Numeric<UnitType,BaseType>& v) const;
   const Numeric<UnitType,BaseType>&
            operator%=(const Numeric<UnitType,BaseType>& v);
   
   // # relational operators =========================================================
   bool     operator==(const Numeric<UnitType,BaseType>& v) const;
   bool     operator!=(const Numeric<UnitType,BaseType>& v) const;
   bool     operator>=(const Numeric<UnitType,BaseType>& v) const;
   bool     operator<=(const Numeric<UnitType,BaseType>& v) const;
   bool     operator< (const Numeric<UnitType,BaseType>& v) const;
   bool     operator> (const Numeric<UnitType,BaseType>& v) const;
   
   friend ostream& operator<<(ostream& out, const Numeric<UnitType,BaseType>& _n) {
      return out << _n.value_;
   }
protected:
   // # member functions =============================================================
   virtual void valueIs(BaseType v) { value_ = v; }
   // # data members =================================================================
   BaseType value_;
};

// # copy constructor -------------------------------------------------------------
template<typename UnitType, typename BaseType>
inline const
Numeric<UnitType,BaseType>& 
Numeric<UnitType,BaseType>::operator=(const Numeric<UnitType,BaseType>& v) {
   if (this != &v) { valueIs(v.value_); }
   return v;
}

// # arithmetic operators ============================================================
// # summation -----------------------------------------------------------------------
template<typename UnitType, typename BaseType>
inline const BaseType
Numeric<UnitType,BaseType>::operator+(const Numeric<UnitType,BaseType>& v) const {
   return Numeric<UnitType,BaseType>(value_ + v.value_).value();
}

template<typename UnitType, typename BaseType>
inline const Numeric<UnitType,BaseType>& Numeric<UnitType,BaseType>::operator++() {
   valueIs(value_ + 1);
   return *this;
}

// template<typename UnitType, typename BaseType>
// inline const Numeric<UnitType,BaseType>& Numeric<UnitType,BaseType>::operator++(int) {
//    Numeric<UnitType,BaseType> prev = *this;
//    valueIs(value_ + 1);
//    return prev;
// }

template<typename UnitType, typename BaseType>
inline const
Numeric<UnitType,BaseType>&
Numeric<UnitType,BaseType>::operator+=(const Numeric<UnitType,BaseType>& v) {
   valueIs(value_ + v.value_);
   return *this;
}

// # subtraction ---------------------------------------------------------------------

// template<typename UnitType, typename BaseType>
// inline const
// Numeric<UnitType,BaseType> operator-(const Numeric<UnitType,BaseType>& v) {
//    return Numeric<UnitType,BaseType>(-1 * v.value_);
// }

template<typename UnitType, typename BaseType>
inline const BaseType
Numeric<UnitType,BaseType>::operator-(const Numeric<UnitType,BaseType>& v) const {
   return Numeric<UnitType,BaseType>(value_ - v.value_);
}

template<typename UnitType, typename BaseType>
inline const Numeric<UnitType,BaseType>& Numeric<UnitType,BaseType>::operator--() {
   valueIs(value_ - 1);
   return *this;
}

// template<typename UnitType, typename BaseType>
// inline const Numeric<UnitType,BaseType>& Numeric<UnitType,BaseType>::operator--(int) {
//    Numeric<UnitType,BaseType> prev = *this;
//    valueIs(value_ - 1);
//    return prev;
// }

template<typename UnitType, typename BaseType>
inline const
Numeric<UnitType,BaseType>&
Numeric<UnitType,BaseType>::operator-=(const Numeric<UnitType,BaseType>& v) {
   valueIs(value_ - v.value_);
   return *this;
}

// # multiplication ------------------------------------------------------------------

template<typename UnitType, typename BaseType>
inline const BaseType
Numeric<UnitType,BaseType>::operator*(const Numeric<UnitType,BaseType>& v) const {
   return Numeric<UnitType,BaseType>(value_ * v.value_);
}

template<typename UnitType, typename BaseType>
inline const
Numeric<UnitType,BaseType>&
Numeric<UnitType,BaseType>::operator*=(const Numeric<UnitType,BaseType>& v) {
   valueIs(value_ * v.value_);
   return *this;
}

// # division ------------------------------------------------------------------------

template<typename UnitType, typename BaseType>
inline const BaseType
Numeric<UnitType,BaseType>::operator/(const Numeric<UnitType,BaseType>& v) const {
   return Numeric<UnitType,BaseType>(value_ / v.value_);
}

template<typename UnitType, typename BaseType>
inline const
Numeric<UnitType,BaseType>&
Numeric<UnitType,BaseType>::operator/=(const Numeric<UnitType,BaseType>& v) {
   valueIs(value_ / v.value_);
   return *this;
}

// # modulus -------------------------------------------------------------------------
template<typename UnitType, typename BaseType>
inline const BaseType
Numeric<UnitType,BaseType>::operator%(const Numeric<UnitType,BaseType>& v) const {
   return Numeric<UnitType,BaseType>(value_ % v.value_);
}

template<typename UnitType, typename BaseType>
inline const
Numeric<UnitType,BaseType>&
Numeric<UnitType,BaseType>::operator%=(const Numeric<UnitType,BaseType>& v) {
   valueIs(value_ % v.value_);
   return *this;
}

// # relational operators ============================================================

template<typename UnitType, typename BaseType>
inline bool
Numeric<UnitType,BaseType>::operator==(const Numeric<UnitType,BaseType>& v) const {
   return value_ == v.value_;
}

template<typename UnitType, typename BaseType>
inline bool
Numeric<UnitType,BaseType>::operator!=(const Numeric<UnitType,BaseType>& v) const {
   return value_ != v.value_;
}

template<typename UnitType, typename BaseType>
inline bool
Numeric<UnitType,BaseType>::operator<=(const Numeric<UnitType,BaseType>& v) const {
   return value_ <= v.value_;
}

template<typename UnitType, typename BaseType>
inline bool
Numeric<UnitType,BaseType>::operator>=(const Numeric<UnitType,BaseType>& v) const {
   return value_ >= v.value_;
}

template<typename UnitType, typename BaseType>
inline bool
Numeric<UnitType,BaseType>::operator<(const Numeric<UnitType,BaseType>& v) const {
   return value_ < v.value_;
}

template<typename UnitType, typename BaseType>
inline bool 
Numeric<UnitType,BaseType>::operator>(const Numeric<UnitType,BaseType>& v) const {
   return value_ > v.value_;
}

} /* end of namespace Simone */

#endif
