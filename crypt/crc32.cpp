#include "crc32.h"

/* boost includes */
#include <boost/crc.hpp>

namespace Simone {

CRC32::CRC32(const void *buffer, size_t byte_count) {
  boost::crc_32_type result;
  result.process_bytes(buffer, byte_count);
  valueIs(result.checksum());
}

} /* end of namespace Simone */
