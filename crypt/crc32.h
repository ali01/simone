#ifndef CRC32_H_UWEJF2D
#define CRC32_H_UWEJF2D

/* simone includes */
#include "../nominal.h"

namespace Simone {

class CRC32 : public Nominal<CRC32,uint32_t> {
public:
  CRC32(uint32_t v=0) : Nominal<CRC32,uint32_t>(v) {}
  CRC32(const void *buffer, size_t byte_count);
};

} /* end of namespace Simone */

#endif
