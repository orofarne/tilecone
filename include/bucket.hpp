#pragma once

#include "tile.hpp"

#include <string>
#include <cstdint>
#include <tuple>
#include <memory>
#include <exception>

namespace tilecone {

class Bucket {
	public:
		class NotFound : public std::exception {
			public:
				NotFound(std::string fName);
				virtual ~NotFound();
				virtual const char* what() const noexcept;
			protected:
				std::string message_;
		};

	public:
		Bucket(std::string const& fName, uint16_t bucketZoom, uint16_t tileZoom, uint64_t bucketX, uint64_t bucketY, size_t blockSize, bool create);
		~Bucket();

		// Returns subtiles of a tile
		std::tuple<void const*, Tile const*, size_t> getTiles(uint16_t zoom, uint64_t x, uint64_t y);
		// Set new tile data. TileZoom only!
		void setTile(uint64_t x, uint64_t y, void const* data, size_t dataSize);

	private:
		struct Pimpl;

		// Returns link to data
		void const * data();

	private:
		uint16_t bucketZoom_;
		uint16_t tileZoom_;
		uint64_t bucketX_;
		uint64_t bucketY_;
		size_t blockSize_;
		std::unique_ptr<Pimpl> pimpl_;
};

} // namespace
