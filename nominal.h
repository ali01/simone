#ifndef NOMINAL_H_59NF9BPE
#define NOMINAL_H_59NF9BPE

namespace Simone {

template <typename UnitType, typename RepType>
class Nominal {
public:
  Nominal(RepType v) : value_(v) {}

  bool operator==(const Nominal<UnitType,RepType>& v) const {
    return value_ == v.value_;
  }

  bool operator!=(const Nominal<UnitType, RepType>& v) const {
    return value_ != v.value_;
  }

  const Nominal<UnitType, RepType>&
  operator=(const Nominal<UnitType, RepType>& v) {
    value_ = v.value_; return *this;
  }

  RepType value() const { return value_; }

protected:
  RepType value_;
};

} /* end of namespace Simone */

#endif
