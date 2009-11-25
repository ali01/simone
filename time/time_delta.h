#pragma once

#include <boost/operators.hpp>
#include <boost/date_time/time_duration.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>

#include "time_types.h"

namespace Simone {

class TimeDelta : private boost::less_than_comparable<TimeDelta,
                               boost::equality_comparable<TimeDelta> > {
   friend class Time;
   friend struct this_thread;
public:
   typedef boost::posix_time::time_duration duration_type;
   typedef boost::posix_time::time_res_traits rep_type;
   typedef boost::posix_time::time_res_traits traits_type;
   typedef boost::posix_time::time_res_traits::day_type day_type;
   typedef boost::posix_time::time_res_traits::hour_type hour_type;
   typedef boost::posix_time::time_res_traits::min_type min_type;
   typedef boost::posix_time::time_res_traits::sec_type sec_type;
   typedef boost::posix_time::time_res_traits::fractional_seconds_type
                                                              fractional_seconds_type;
   typedef boost::posix_time::time_res_traits::tick_type tick_type;
   typedef boost::posix_time::time_res_traits::impl_type impl_type;
   
   TimeDelta(float unconvertedHrs){
      int hrs = (int)(unconvertedHrs +.5);
      float minutesLeft = (unconvertedHrs - hrs) * 60;
      int min = (int)(minutesLeft+ .5);
      float secondsLeft = (minutesLeft - min)*60;
      int sec = (int)(secondsLeft + .5);
      float milliLeft = (secondsLeft - sec)*1000;
      int milli = (int)(milliLeft + .5); //todo is this fractional seconds? is this formula right?
      delta_ = boost::posix_time::time_duration(hrs, min, sec, milli);
   }

   TimeDelta(long _hrs, long _min, long _sec, long _fractional=0) :
                                              delta_(_hrs, _min, _sec, _fractional) {}
   TimeDelta(const TimeDelta& rhs) : delta_(rhs.delta_.hours(),
                                            rhs.delta_.minutes(),
                                            rhs.delta_.seconds(),
                                            rhs.delta_.fractional_seconds()) {}
   
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
   
protected:
   TimeDelta(boost::posix_time::time_duration rhs) : delta_(rhs.hours(),
                                                     rhs.minutes(),
                                                     rhs.seconds()) {}
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

typedef boost::date_time::subsecond_duration<TimeDelta,1000> millisec;
typedef boost::date_time::subsecond_duration<TimeDelta,1000> milliseconds;

typedef boost::date_time::subsecond_duration<TimeDelta,1000000> microsec;
typedef boost::date_time::subsecond_duration<TimeDelta,1000000> microseconds;

#ifdef BOOST_DATE_TIME_HAS_NANOSECONDS

typedef date_time::subsecond_duration<TimeDelta,1000000000> nanosec;
typedef date_time::subsecond_duration<TimeDelta,1000000000> nanoseconds;

#endif

} /* end of namespace Simone */
