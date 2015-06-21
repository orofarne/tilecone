#include "bucket.hpp"
#include "zigzag.hpp"

#include <cstring>
#include <cassert>
#include <fstream>

#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/filesystem.hpp>

namespace bip = boost::interprocess;
namespace fs = boost::filesystem;

namespace tilecone {

struct Bucket::Pimpl {
	bip::file_mapping mmf;
	bip::mapped_region mmr;
	size_t tilesCount;
};

Bucket::Bucket(char const* fName, uint16_t bucketZoom, uint16_t tileZoom, uint64_t bucketX, uint64_t bucketY)
	: bucketZoom_(bucketZoom)
	, tileZoom_(tileZoom)
	, bucketX_(bucketX)
	, bucketY_(bucketY)
{
	assert(bucketZoom_ <= tileZoom_);

	pimpl_.reset(new Pimpl);

	pimpl_->tilesCount = size_t(1) << (2 * (tileZoom_ - bucketZoom_));

	bool isDirty = false;
	if (!fs::exists(fName)) {
		std::ofstream{fName, std::ios::app};
	}
	if (fs::file_size(fName) < (pimpl_->tilesCount * sizeof(Tile))) {
		fs::resize_file(fName, pimpl_->tilesCount * sizeof(Tile));
		isDirty = true;
	}
	pimpl_->mmf = bip::file_mapping(fName, bip::read_write);
	pimpl_->mmr = bip::mapped_region(pimpl_->mmf, bip::read_write);
	if (isDirty) {
		std::memset(pimpl_->mmr.get_address(), 0, pimpl_->mmr.get_size());
	}
}

Bucket::~Bucket() {
}

std::tuple<Tile const*, size_t>
Bucket::getTile(uint16_t zoom, uint64_t x, uint64_t y) {
	if (zoom <= bucketZoom_) {
		// Return all data in bucket if bucket tile inside requested tile
		uint64_t M = uint64_t(1) << (bucketZoom_ - zoom);
		uint64_t x1 = bucketX_ / M;
		uint64_t y1 = bucketY_ / M;
		if (x1 == x && y1 == y)
			return std::make_tuple((Tile const*)pimpl_->mmr.get_address(), pimpl_->tilesCount);
		else
			return std::make_tuple((Tile const*)nullptr, (size_t)0);
	} else {
		// Search subrange of data in bucket tile
		uint64_t tileSize = 1 << (tileZoom_ > zoom ? tileZoom_ - zoom : 0);
		uint16_t zoomDiff = tileZoom_ - bucketZoom_;

		uint64_t M = uint64_t(1) << (zoom - bucketZoom_);
		if ((x / M) != bucketX_ || (y / M) != bucketY_)
			return std::make_tuple((Tile const*)nullptr, (size_t)0);

		uint64_t x1 = x % M;
		uint64_t y1 = y % M;
		uint64_t pos = utils::zigZagPosition(zoomDiff, x1, y1);
		return std::make_tuple(
			(Tile const*)pimpl_->mmr.get_address() + pos,
			(size_t)tileSize);
	}
	assert(false);
}

void
Bucket::setTile(uint64_t x, uint64_t y, void const* data, size_t dataSize) {

}


} // namespace
