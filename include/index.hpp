#pragma once

#include <cstdint>
#include <string>

namespace tilecone {

class Index {
  public:
    Index(std::string const& fName);
    ~Index();

    inline uint64_t bucketZoom() { return bucketZoom_; }
    inline uint64_t tileZoom() { return tileZoom_; }
    inline size_t blockSize() { return blockSize_; }

  protected:
    uint16_t bucketZoom_;
    uint16_t tileZoom_;
    size_t blockSize_;
};

} // namespace
