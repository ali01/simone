#ifndef TIME_H_VYGBQUZT
#define TIME_H_VYGBQUZT

#include <ctime>
#include <errno.h>
#include <sys/time.h>

#ifdef __APPLE__
#include "portability/clock_gettime_stub.h"
#endif

#include "Numeric.h"

namespace Simone {
// TODO: implement monotonic time support
// TODO: perform error handling 

class seconds_t; /* unit type stub class declaration */
class Seconds : public Simone::Numeric<seconds_t,time_t> {
public:
   Seconds(time_t v=0) : Simone::Numeric<seconds_t,time_t>(v) {}
};


class nanoseconds_t; /* unit type stub class declaration */
class Nanoseconds : public Simone::Numeric<nanoseconds_t,uint64_t> {
public:
   Nanoseconds(uint64_t v=0) : Simone::Numeric<nanoseconds_t,uint64_t>(v) {}
};

/* forward declaration */ class TimeDelta;
class Time {
public:
   Time(clockid_t cid=CLOCK_REALTIME) {
      timespec t_spec;
      if(clock_gettime(cid, &t_spec) == -1) { throw errno; }
      seconds_     = t_spec.tv_sec;
      nanoseconds_ = (uint64_t)t_spec.tv_nsec;
   }
   
   Time(const Time& o) : seconds_(o.seconds()), nanoseconds_(o.nanoseconds()) {}
   const Time& operator=(const Time& o) {
      if (this != &o) {
         seconds_ = o.seconds();
         nanoseconds_ = o.nanoseconds();
      }
      return *this;
   }
   
   static const Time current() {
      Time t;
      return t;
   }
   
   static const Time monotonic() {
      Time t(CLOCK_MONOTONIC);
      return t;
   }
   
   // accessors ======================================================================
   const Seconds     seconds() const { return seconds_; }
   const Nanoseconds nanoseconds() const { return nanoseconds_; }
   
   // operators ======================================================================
   
   bool operator==(const Time& o) const {
      return (seconds() == o.seconds() && nanoseconds() == o.nanoseconds());
   }
   
   bool operator!=(const Time& o) const { return ! (*this == o); }
   
   bool operator<(const Time& o) const {
      if (seconds() < o.seconds()) { return true; }
      if(seconds() == o.seconds() && nanoseconds() < o.nanoseconds()) { return true; }
      return false;
   }
   
   bool operator<=(const Time& o) const {
      if (seconds() <= o.seconds()) { return true; }
      if(seconds() == o.seconds() && nanoseconds() <= o.nanoseconds()){ return true; }
      return false;
   }
   
   bool operator>(const Time& o) const { return ! (*this <= o); }
   
   bool operator>=(const Time& o) const { return ! (*this < o); }
   
   timespec *byteRepInit(timespec *ref) {
      ref->tv_sec = seconds_.value();
      ref->tv_nsec = nanoseconds_.value();
      return ref;
   }
protected:
   Time(Seconds _sec, Nanoseconds _nsec) : seconds_(_sec), nanoseconds_(_nsec) {}
   Time(time_t _sec, uint64_t _nsec) : seconds_(_sec), nanoseconds_(_nsec) {}
   
   Seconds     seconds_;
   Nanoseconds nanoseconds_;
   
   static const Time& add(Time& _t, const Time& _delta);
   static const Time& subtract(Time& _t, const Time& _delta);
   
   friend const Time operator+(const Time& _time, const TimeDelta& _delta);
   friend const Time operator-(const Time& _time, const TimeDelta& _delta);
   friend const Time& operator+=(Time& _time, const TimeDelta& _delta);
   friend const Time& operator-=(Time& _time, const TimeDelta& _delta);
};



class TimeDelta : public Time {
public:
   TimeDelta() {}
   TimeDelta(const Time& _o) : Time(_o) {}
   TimeDelta(time_t _sec, uint64_t _nsec=0) : Time(_sec, _nsec) {}
   TimeDelta(Seconds _sec, Nanoseconds _nsec=0) : Time(_sec, _nsec) {}
   
   friend const TimeDelta operator+(const TimeDelta& _d1, const TimeDelta& _d2);
   friend const TimeDelta operator-(const TimeDelta& _d1, const TimeDelta& _d2);
   friend const TimeDelta& operator+=(TimeDelta& _d1, const TimeDelta& _d2);
   friend const TimeDelta& operator-=(TimeDelta& _d1, const TimeDelta& _d2);
};



inline const Time& Time::add(Time& _t, const Time& _delta) {
   _t.seconds_     += _delta.seconds();
   _t.nanoseconds_ += _delta.nanoseconds();
   while (_t.nanoseconds() >= 1e9) {
      ++_t.seconds_;
      _t.nanoseconds_ -= 1e9;
   }
   return _t;
}

inline const Time& Time::subtract(Time& _t, const Time& _delta) {
   while (_t.nanoseconds() < _delta.nanoseconds()) {
      --_t.seconds_;
      _t.nanoseconds_ += 1e9;
   }
   _t.seconds_     -= _delta.seconds();
   _t.nanoseconds_ -= _delta.nanoseconds();
   return _t;
}

// arithmetic operators ==============================================================
// TODO: return base type rather than Time object
inline const Time operator+(const Time& _time, const TimeDelta& _delta) {
   Time t(_time);
   return Time::add(t, _delta);
}

inline const Time operator-(const Time& _time, const TimeDelta& _delta) {
   Time t(_time);
   return Time::subtract(t, _delta);
}

inline const Time& operator+=(Time& _time, const TimeDelta& _delta) {
   return Time::add(_time, _delta);
}

inline const Time& operator-=(Time& _time, const TimeDelta& _delta) {
   return Time::subtract(_time, _delta);
}

inline const TimeDelta operator+(const TimeDelta& _d, const TimeDelta& _delta) {
   TimeDelta t(_d);
   return Time::add(t, _delta);
}

inline const TimeDelta operator-(const TimeDelta& _d, const TimeDelta& _delta) {
   TimeDelta t(_d);
   return Time::subtract(t, _delta);
}

inline const TimeDelta& operator+=(TimeDelta& _d, const TimeDelta& _delta) {
   Time::add(_d, _delta);
   return _d;
}

inline const TimeDelta& operator-=(TimeDelta& _d, const TimeDelta& _delta) {
   Time::subtract(_d, _delta);
   return _d;
}

} /* end of namespace Simone */

#endif
