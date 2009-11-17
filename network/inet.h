#ifndef INET_H_IA2S09LW
#define INET_H_IA2S09LW

#include <ostream>
#include "../ordinal.h"

namespace Simone {
using std::ostream;

/* forward declaration */ class AddressIPv4;
class subnet_mask_ipv4_t;
class SubnetMaskIPv4 : public Ordinal<subnet_mask_ipv4_t,uint32_t> {
   friend class AddressIPv4;
public:
   static const SubnetMaskIPv4 kMax;
   SubnetMaskIPv4(uint32_t _v) : Ordinal<subnet_mask_ipv4_t,uint32_t>(_v) {}
   SubnetMaskIPv4(const SubnetMaskIPv4& _sm=kMax) :
                                 Ordinal<subnet_mask_ipv4_t,uint32_t>(_sm.value()) {}
   
   SubnetMaskIPv4 operator&(const AddressIPv4& _addr) const;
      
   SubnetMaskIPv4 reverse() const {
      SubnetMaskIPv4 rsm(kMax.value() - value());
      return rsm;
   }
};


class address_ipv4_t;
class AddressIPv4 : public Ordinal<address_ipv4_t,uint32_t> {
   friend class SubnetMaskIPv4;
public:
   static const AddressIPv4 kMax;
   AddressIPv4(uint32_t _v=0x0) : Ordinal<address_ipv4_t,uint32_t>(_v) {}
   AddressIPv4(const AddressIPv4& _addr) :
                                  Ordinal<address_ipv4_t,uint32_t>(_addr.value_) {}
   AddressIPv4  operator&(const SubnetMaskIPv4& _sm) const 
                                                   { return value_ & _sm.value(); }
};


class MaskedIPv4 : public AddressIPv4 {
public:
   MaskedIPv4() {}
   MaskedIPv4(AddressIPv4 _addr, const SubnetMaskIPv4& _sm) : 
                                                              AddressIPv4(_addr),
                                                              subnet_mask_(_sm) {}
   MaskedIPv4(AddressIPv4 _addr) : AddressIPv4(_addr) {}
   MaskedIPv4(const MaskedIPv4& ip) :
                                      AddressIPv4(ip.unmasked()),
                                      subnet_mask_(ip.subnetMask()) {}
   MaskedIPv4& operator=(const MaskedIPv4& ip) {
      if (this != &ip) {
         value_ = ip.unmasked().value();
         subnet_mask_ = ip.subnet_mask_;
      }
      return *this;
   }
   
         AddressIPv4      address()     const { return *this & subnet_mask_; }
   const SubnetMaskIPv4&  subnetMask()  const { return subnet_mask_;         }
   const AddressIPv4      unmasked()    const { return AddressIPv4::value(); }
   
   bool prefixMatches(const MaskedIPv4& _addr) const {
      return address() == (_addr.address());
   }
   
   bool prefixMatches(const AddressIPv4& _addr) const {
      return address() == (_addr & subnetMask());
   }
   
   void addressIs(const AddressIPv4& _addr) { value_ = _addr.value();   }
   void subnetMaskIs(const SubnetMaskIPv4& _sm) { subnet_mask_ = _sm; }
protected:
   const uint32_t& value() const { return address().value(); }
   // data members ===================================================================
   SubnetMaskIPv4 subnet_mask_;
};

inline SubnetMaskIPv4 SubnetMaskIPv4::operator&(const AddressIPv4& _addr) const 
                                                { return value_ & _addr.value_; }


} /* end of namespace Simone */

#endif
