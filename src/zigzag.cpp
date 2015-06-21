#include "zigzag.hpp"

namespace tilecone {
namespace utils {

size_t zigZagPosition(uint16_t zoomDiff, uint64_t x, uint64_t y) {
  size_t pos = 0;

  for(uint16_t i = 0; i < zoomDiff; ++i) {
    uint64_t coordMask = uint64_t(1) << i;
    size_t xi = (x & coordMask) >> i;
    size_t yi = (y & coordMask) >> i;
    pos += (yi << (2*i + 1)) + (xi << (2*i));
  }

  return pos;
}

} // namespace
} // namespace
