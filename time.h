/* Copyright (c) 2008-2010. Ali H. Yahya, All rights reserved. */

#pragma once

#include <boost/operators.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>

#include "exception.h"
#include "time/time_delta.h"

namespace Simone {

struct Clock {
   enum Type { kSecondLocal, kSecondUniversal, kMicrosecLocal, kMicrosecUniversal };
};

class Time
   : private boost::less_than_comparable<Time,boost::equality_comparable<Time> > {
public:
   enum SpecialValue { kNow, kInfinity, kNegInfinity, kMax, kMin, kNull };
   
   explicit Time(Clock::Type _type) {
      switch (_type) {
         case Clock::kSecondLocal:
            ptime_ = boost::posix_time::second_clock::local_time();
            break;
         case Clock::kSecondUniversal:
            ptime_ = boost::posix_time::second_clock::universal_time();
            break;
         case Clock::kMicrosecLocal:
            ptime_ = boost::posix_time::microsec_clock::local_time();
            break;
         case Clock::kMicrosecUniversal:
            ptime_ = boost::posix_time::microsec_clock::universal_time();
            break;
         default: throw AttributeNotSupportedException(__FILE__, __LINE__, "clock_type unknown");
      }
   }
   
   explicit Time(SpecialValue _v=kNull) {
      switch (_v) {
         case kNow:
            ptime_ = boost::posix_time::microsec_clock::universal_time();
            break;
         case kInfinity:
            ptime_ = boost::posix_time::ptime(boost::date_time::pos_infin);
            break;
         case kNegInfinity:
            ptime_ = boost::posix_time::ptime(boost::date_time::neg_infin);
            break;
         case kMax:
            ptime_ = boost::posix_time::ptime(boost::date_time::max_date_time);
            break;
         case kMin:
            ptime_ = boost::posix_time::ptime(boost::date_time::min_date_time);
            break;
         case kNull:
            ptime_ = boost::posix_time::ptime(boost::date_time::not_a_date_time);
            break;
         default: throw AttributeNotSupportedException(__FILE__, __LINE__, "special_value unknown");
      }
   }
   
   Time(const Time& _o) : ptime_(_o.ptime_) {}
   
   // operators ======================================================================
   Time& operator=(const Time& rhs) {
      ptime_ = rhs.ptime_;
      return *this;
   }
   
   bool operator==(const Time& rhs) const {
      return ptime_ == rhs.ptime_;
   }
   
   bool operator<(const Time& rhs) const {
      return ptime_ < rhs.ptime_;
   }
   
   TimeDelta operator-(const Time& rhs) const {
      return ptime_ - rhs.ptime_;
   }
   
   Time operator+(const TimeDelta& td) const {
      return ptime_ + td.delta_;
   }
   
   Time operator-(const TimeDelta& td) const {
      return ptime_ - td.delta_;
   }
   
   Time& operator+=(const TimeDelta& td) {
      ptime_ += td.delta_;
      return *this;
   }
   
   Time& operator-=(const TimeDelta& td)  {
      ptime_ -= td.delta_;
      return *this;
   }
   
   boost::posix_time::ptime ptime() { return ptime_; }
private:
   Time(boost::posix_time::ptime _rhs) : ptime_(_rhs) {}
   boost::posix_time::ptime ptime_;
};

} /* end of namespace Simone */
