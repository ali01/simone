#pragma once

#include "../numeric.h"

namespace Simone {

class Hours : public Numeric<Hours,long> {
public:
   Hours(long v=0) : Numeric<Hours,long>(v) {}
};

class Minutes : public Numeric<Minutes,long> {
public:
   Minutes(long v=0) : Numeric<Minutes,long>(v) {}
};

class Seconds : public Numeric<Seconds,long> {
public:
   Seconds(long v=0) : Numeric<Seconds,long>(v) {}
};


class Nanoseconds : public Numeric<Nanoseconds,long> {
public:
   Nanoseconds(long v=0) : Numeric<Nanoseconds,long>(v) {}
};

} /* end of namespace Simone */
