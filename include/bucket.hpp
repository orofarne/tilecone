#pragma once

#include "tile.hpp"

#include <cstdint>
#include <tuple>
#include <memory>

namespace tilecone {

class Bucket {
	public:
		Bucket(char const* fName, uint16_t bucketZoom, uint16_t tileZoom, uint64_t bucketX, uint64_t bucketY);
		~Bucket();

		// Returns subtiles of a tile
		std::tuple<Tile const*, size_t> getTile(uint16_t zoom, uint64_t x, uint64_t y);
		// Set new tile data. TileZoom only!
		void setTile(uint64_t x, uint64_t y, void const* data, size_t dataSize);

	private:
		struct Pimpl;

	private:
		uint16_t bucketZoom_;
		uint16_t tileZoom_;
		uint64_t bucketX_;
		uint64_t bucketY_;
		std::unique_ptr<Pimpl> pimpl_;
};

} // namespace
