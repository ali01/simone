/* Copyright (c) 2008-2010. Ali H. Yahya, All rights reserved. */

#ifndef INET_H_IA2S09LW
#define INET_H_IA2S09LW

#include <ostream>
#include <sstream>
#include <string>
#include <arpa/inet.h>
using std::string;
using std::ostream;
using std::stringstream;

#include "../ordinal.h"
#include "../thread/concurrent_collection_element.h"

namespace Simone {

/* forward declaration */ class AddressIPv4;
class SubnetMaskIPv4 : public Ordinal<SubnetMaskIPv4,uint32_t> {
   friend class AddressIPv4;
public:
   // TODO: consider converting to SubnetMaskIPv4 object
   SubnetMaskIPv4(uint32_t _v=kMax) : Ordinal<SubnetMaskIPv4,uint32_t>(_v) {}
   SubnetMaskIPv4(const SubnetMaskIPv4& _sm) :
                                 Ordinal<SubnetMaskIPv4,uint32_t>(_sm.value()) {}
   
   SubnetMaskIPv4 operator&(const AddressIPv4& _addr) const;
      
   SubnetMaskIPv4 reverse() const {
      SubnetMaskIPv4 rsm(kMax - value());
      return rsm;
   }
private:
   static const uint32_t kMax = 0xFFFFFFFF;
};

class AddressIPv4 : public Ordinal<AddressIPv4,uint32_t> {
   friend class SubnetMaskIPv4;
public:
   static const AddressIPv4 kZero;
   
   explicit AddressIPv4(uint32_t _a) : Ordinal<AddressIPv4,uint32_t>(_a) {}
   AddressIPv4(uint32_t _a, uint32_t _mask) : Ordinal<AddressIPv4,uint32_t>(_a),
                                              subnet_mask_(_mask) {}
   AddressIPv4(const AddressIPv4& _addr)
      : Ordinal<AddressIPv4,uint32_t>(_addr.value_),
        subnet_mask_(_addr.subnetMask()) {}
   AddressIPv4(const AddressIPv4& _addr, const SubnetMaskIPv4& _sm)
      : Ordinal<AddressIPv4,uint32_t>(_addr.value_),
        subnet_mask_(_addr.subnetMask().value() & _sm.value()) {}
   
   AddressIPv4& operator=(const AddressIPv4& ip) {
      if (this != &ip) {
         value_       = ip.address().value();
         subnet_mask_ = ip.subnetMask().value();
      }
      return *this;
   }
   
   AddressIPv4 address() const {
      return AddressIPv4(value_);
   }
   
   const SubnetMaskIPv4& subnetMask() const {
      return subnet_mask_;
   }
   
   void addressIs(const AddressIPv4& _addr) {
      value_ = _addr.value();
   }
   
   void subnetMaskIs(const SubnetMaskIPv4& _sm) {
      subnet_mask_ = _sm;
   }
   
   virtual bool contains(const AddressIPv4& _addr) const {
      return *this == (_addr & this->subnetMask());
   }
   
   AddressIPv4 operator&(const SubnetMaskIPv4& _sm) const {
      return AddressIPv4(value() & _sm.value());
   }
   
   uint32_t value() const {
      return value_ & subnetMask().value();
   }
   
   const string str() const {
      stringstream ss;
      ss << *this;
      return ss.str();
   }
   
   const char *c_str() const {
      return str().c_str();
   }
   
   friend ostream& operator<<(ostream& out, const AddressIPv4& _addr) {
      uint32_t addr_v = _addr.value();
      uint32_t curr_octet = addr_v >> 24;
      out << curr_octet << ".";
      curr_octet = (addr_v << 8) >> 24;
      out << curr_octet << ".";
      curr_octet = (addr_v << 16) >> 24;
      out << curr_octet << ".";
      curr_octet = (addr_v << 24) >> 24;
      out << curr_octet;
      return out;
   }
private:
   SubnetMaskIPv4 subnet_mask_;
};

inline SubnetMaskIPv4 SubnetMaskIPv4::operator&(const AddressIPv4& _addr) const {
   AddressIPv4 addr = _addr & *this;
   return addr.value();
}

} /* end of namespace Simone */

#endif
