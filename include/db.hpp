#pragma once

#include "tile.hpp"

#include <tuple>
#include <memory>
#include <string>
#include <cstdint>

namespace tilecone {

class DB {
  public:
    DB(std::string const& path, int mmappool);
    ~DB();

    // Returns subtiles of a tile
    std::tuple<void const*, Tile const*, size_t> getTiles(uint16_t zoom, uint64_t x, uint64_t y);
    // Set new tile data. TileZoom only!
    void setTile(uint64_t x, uint64_t y, void const* data, size_t dataSize);

  private:
    struct Pimpl;
    std::unique_ptr<Pimpl> pimpl_;
};

}
