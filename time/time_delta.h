#pragma once

#include <boost/operators.hpp>
#include <boost/date_time/time_duration.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>

#include "time_types.h"
#include <cmath>

namespace Simone {

class TimeDelta : private boost::less_than_comparable<TimeDelta,
                               boost::equality_comparable<TimeDelta> > {
   friend class Time;
public:   
   TimeDelta(double sec_dbl){
      long sec   = static_cast<long>(floor(sec_dbl));
      int res    = boost::posix_time::time_duration::num_fractional_digits();
      long fract = static_cast<long>(floor((sec_dbl - sec) * pow(10.0, res)));
      delta_     = boost::posix_time::time_duration(0, 0, sec, fract);
   }
   
   TimeDelta(long _hrs, long _min, long _sec, long _fractional=0) :
                                              delta_(_hrs, _min, _sec, _fractional) {}
   TimeDelta(const TimeDelta& rhs) : delta_(rhs.delta_.hours(),
                                            rhs.delta_.minutes(),
                                            rhs.delta_.seconds(),
                                            rhs.delta_.fractional_seconds()) {}
   
   long totalSeconds() const { return delta_.total_seconds(); }
   
   double totalSecondsDbl() const {
      double sec_dbl = delta_.total_seconds();
      int res        = boost::posix_time::time_duration::num_fractional_digits();
      double fract   = delta_.fractional_seconds() / pow(10.0, res);
      return sec_dbl + fract;
   }
   // operators ======================================================================
   TimeDelta& operator=(const TimeDelta& rhs) {
      delta_ = rhs.delta_;
      return *this;
   }
   
   bool operator<(const TimeDelta& rhs) const {
      return delta_ < rhs.delta_;
   }
   
   bool operator==(const TimeDelta& rhs) const {
      return delta_ ==  rhs.delta_;
   }
   
   // unary minus ~ allows for TimeDelta td = -td1
   TimeDelta operator-() const {
      return -delta_;
   }
   
   TimeDelta operator-(const TimeDelta& d) const {
      return delta_ - d.delta_;
   }
   
   TimeDelta operator+(const TimeDelta& d) const {
      return delta_ + d.delta_;
   }
   
   TimeDelta operator*(int rhs) const {
      return delta_ * rhs;
   }
   
   TimeDelta operator/(int divisor) const {
      return delta_ / divisor;
   }
   
   TimeDelta& operator-=(const TimeDelta& d) {
      delta_ -= d.delta_;
      return *this;
   }
   
   TimeDelta& operator+=(const TimeDelta& d) {
      delta_ += d.delta_;
      return *this;
   }
   
   TimeDelta& operator/=(int divisor)  {
      delta_ /= divisor;
      return *this;
   }
   
   TimeDelta& operator*=(int rhs) {
      delta_ *= rhs;
      return *this;
   }
   
   boost::posix_time::time_duration boost_time_duration() const { return delta_; }
protected:
   TimeDelta(boost::posix_time::time_duration rhs) : delta_(rhs.hours(),
                                                     rhs.minutes(),
                                                     rhs.seconds(),
                                                     rhs.fractional_seconds()) {}
   boost::posix_time::time_duration delta_;
};

class hours : public TimeDelta {
public:
  explicit hours(long h) : TimeDelta(h, 0, 0) {}
};

class minutes : public TimeDelta {
public:
  explicit minutes(long m) : TimeDelta(0, m, 0) {}
};

class seconds : public TimeDelta {
public:
  explicit seconds(long s) : TimeDelta(0, 0, s) {}
};

template <boost::int64_t sec_fraction>
class subsecond_duration : public TimeDelta {
   typedef boost::posix_time::time_res_traits traits_type;
public:
   explicit subsecond_duration(boost::int64_t ss) :
      TimeDelta(0, 0, 0, ss * traits_type::res_adjust() / sec_fraction) {}
};

typedef subsecond_duration<1000> milliseconds;
typedef subsecond_duration<1000> millisec;

typedef subsecond_duration<1000000> microseconds;
typedef subsecond_duration<1000000> microsec;

// #ifdef BOOST_DATE_TIME_HAS_NANOSECONDS
typedef subsecond_duration<1000000000> nanoseconds;
typedef subsecond_duration<1000000000> nanosec;
// #endif

} /* end of namespace Simone */
