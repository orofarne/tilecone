#include "bucket.hpp"
#include "zigzag.hpp"

#include <cstring>
#include <cassert>
#include <fstream>

#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/filesystem.hpp>

#ifdef TILECONE_MT
#include <boost/thread/shared_mutex.hpp>
#endif

namespace bip = boost::interprocess;
namespace fs = boost::filesystem;

namespace tilecone {

struct MMFile {
	bip::file_mapping mmf;
	bip::mapped_region mmr;
};

struct Bucket::Pimpl {
	std::string fName;
	std::unique_ptr<MMFile> mm;
	size_t tilesCount;
	size_t indexSize;
#ifdef TILECONE_MT
	boost::shared_mutex shMu;
#endif
};

Bucket::Bucket(std::string const& fName, uint16_t bucketZoom, uint16_t tileZoom, uint64_t bucketX, uint64_t bucketY, size_t blockSize)
	: bucketZoom_(bucketZoom)
	, tileZoom_(tileZoom)
	, bucketX_(bucketX)
	, bucketY_(bucketY)
	, blockSize_(blockSize)
{
	assert(bucketZoom_ <= tileZoom_);

	pimpl_.reset(new Pimpl);

	pimpl_->fName = fName;
	pimpl_->tilesCount = size_t(1) << (2 * (tileZoom_ - bucketZoom_));
	pimpl_->indexSize = pimpl_->tilesCount * sizeof(Tile);

	bool isDirty = false;
	if (!fs::exists(pimpl_->fName)) {
		std::ofstream{pimpl_->fName, std::ios::app};
	}
	if (fs::file_size(pimpl_->fName) < (pimpl_->tilesCount * sizeof(Tile))) {
		fs::resize_file(pimpl_->fName, pimpl_->indexSize);
		isDirty = true;
	}
	pimpl_->mm.reset(new MMFile);
	pimpl_->mm->mmf = bip::file_mapping(pimpl_->fName.c_str(), bip::read_write);
	pimpl_->mm->mmr = bip::mapped_region(pimpl_->mm->mmf, bip::read_write);
	if (isDirty) {
		std::memset(pimpl_->mm->mmr.get_address(), 0, pimpl_->mm->mmr.get_size());
	}
}

Bucket::~Bucket() {
}

void const *
Bucket::data() {
	return (uint8_t *)pimpl_->mm->mmr.get_address() + pimpl_->indexSize;
}

std::tuple<void const*, Tile const*, size_t>
Bucket::getTiles(uint16_t zoom, uint64_t x, uint64_t y) {
#ifdef TILECONE_MT
	// get shared access
	boost::shared_lock<boost::shared_mutex> lock(pimpl_->shMu);
#endif

	if (zoom <= bucketZoom_) {
		// Return all data in bucket if bucket tile inside requested tile
		uint64_t M = uint64_t(1) << (bucketZoom_ - zoom);
		uint64_t x1 = bucketX_ / M;
		uint64_t y1 = bucketY_ / M;
		if (x1 == x && y1 == y)
			return std::make_tuple(data(), (Tile const*)pimpl_->mm->mmr.get_address(), pimpl_->tilesCount);
		else
			return std::make_tuple(data(), (Tile const*)nullptr, (size_t)0);
	} else {
		// Search subrange of data in bucket tile
		uint64_t tileSize = 1 << (tileZoom_ > zoom ? tileZoom_ - zoom : 0);
		uint16_t zoomDiff = tileZoom_ - bucketZoom_;

		uint64_t M = uint64_t(1) << (zoom - bucketZoom_);
		if ((x / M) != bucketX_ || (y / M) != bucketY_)
			return std::make_tuple(data(), (Tile const*)nullptr, (size_t)0);

		uint64_t x1 = x % M;
		uint64_t y1 = y % M;
		uint64_t pos = utils::zigZagPosition(zoomDiff, x1, y1);
		return std::make_tuple(
			data(),
			(Tile const*)pimpl_->mm->mmr.get_address() + pos,
			(size_t)tileSize);
	}
	assert(false);
}

void
Bucket::setTile(uint64_t x, uint64_t y, void const* data, size_t dataSize) {
#ifdef TILECONE_MT
	// get upgradable access
  boost::upgrade_lock<boost::shared_mutex> lock(pimpl_->shMu);
  // get exclusive access
  boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock(lock);
#endif

	uint16_t zoomDiff = tileZoom_ - bucketZoom_;
	uint64_t M = uint64_t(1) << (zoomDiff);
	uint64_t pos = utils::zigZagPosition(zoomDiff, x % M, y % M);
	auto tile = (Tile *)pimpl_->mm->mmr.get_address() + pos;

	if (dataSize <= tile->capacity) {
		memcpy(
			(uint8_t *)pimpl_->mm->mmr.get_address() + pimpl_->indexSize + tile->offset,
			data,
			dataSize
		);
		tile->size = dataSize;
	} else {
		auto nBlocks = (dataSize - tile->capacity) / blockSize_ +
											((dataSize - tile->capacity) % blockSize_ == 0 ? 0 : 1);

		// Resize file..
		// Close mapping
		pimpl_->mm.reset();

		// Add new blocks
		auto fSize = fs::file_size(pimpl_->fName);
		fs::resize_file(pimpl_->fName, fSize + blockSize_*nBlocks);

		// Reopen mapping
		pimpl_->mm.reset(new MMFile);
		pimpl_->mm->mmf = bip::file_mapping(pimpl_->fName.c_str(), bip::read_write);
		pimpl_->mm->mmr = bip::mapped_region(pimpl_->mm->mmf, bip::read_write);

		tile = (Tile *)pimpl_->mm->mmr.get_address() + pos; // fix tile link

		// Move next data
		auto tileEnd = pimpl_->indexSize + tile->offset + tile->capacity;
		for (auto dOff = fSize - blockSize_; dOff >= tileEnd; dOff -= blockSize_) {
			memmove(
				(uint8_t *)pimpl_->mm->mmr.get_address() + dOff + blockSize_*nBlocks,
				(uint8_t *)pimpl_->mm->mmr.get_address() + dOff,
				blockSize_
			);
		}

		// Fix index
		for (auto i = pos + 1; i < pimpl_->tilesCount; ++i) {
			auto tile = (Tile *)pimpl_->mm->mmr.get_address() + i;
			tile->offset += blockSize_*nBlocks;
		}

		// Set data
		auto tile = (Tile *)pimpl_->mm->mmr.get_address() + pos;
		memcpy(
			(uint8_t *)pimpl_->mm->mmr.get_address() + pimpl_->indexSize + tile->offset,
			data,
			dataSize
		);
		tile->size = dataSize;
		tile->capacity += blockSize_*nBlocks;
	}
}

} // namespace
