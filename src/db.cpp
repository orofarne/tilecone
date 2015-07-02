#include "db.hpp"
#include "index.hpp"
#include "bucket.hpp"

#include "lrucache.hpp"

#include <stdexcept>

#include <boost/filesystem.hpp>
#include <boost/format.hpp>

namespace fs = boost::filesystem;

namespace tilecone {

using BucketPtr = std::shared_ptr<Bucket>;

struct DB::Pimpl {
  int mmappool;
  fs::path basePath;
  std::unique_ptr<Index> idx;
  std::unique_ptr<cache::lru_cache<std::string, BucketPtr>> bucketCache;

  BucketPtr getBucket(uint64_t x, uint64_t y);
};

DB::DB(std::string const& path, int mmappool) {
  if (!fs::is_directory(path)) {
    throw std::invalid_argument(str( boost::format("%1% is not a directory") % path ));
  }

  pimpl_.reset(new Pimpl);

  pimpl_->mmappool = mmappool;
  pimpl_->basePath = fs::path(path);
  pimpl_->idx.reset(new Index((pimpl_->basePath / "index.ini").native()));
  pimpl_->bucketCache.reset(new cache::lru_cache<std::string, BucketPtr>(pimpl_->mmappool));
}

DB::~DB() {

}

std::tuple<void const*, Tile const*, size_t>
DB::getTiles(uint16_t zoom, uint64_t x, uint64_t y) {
  auto bucket = pimpl_->getBucket(x, y);
  return bucket->getTiles(zoom, x, y);
}

void
DB::setTile(uint64_t x, uint64_t y, void const* data, size_t dataSize) {
  auto bucket = pimpl_->getBucket(x, y);
  bucket->setTile(x, y, data, dataSize);
}

BucketPtr
DB::Pimpl::getBucket(uint64_t x, uint64_t y) {
  auto M = idx->tileZoom() - idx->bucketZoom();
  auto x1 = x >> M;
  auto y1 = y >> M;

  std::string bucketFile =
    (basePath / str( boost::format("%1%_%2%.cone") % x1 % y1 )).native();

  BucketPtr bucket;
  try {
    bucket = bucketCache->get(bucketFile);
  }
  catch(std::range_error const &) {
    bucket.reset(new Bucket(
      bucketFile,
      idx->bucketZoom(),
      idx->tileZoom(),
      x1,
      y1,
      idx->blockSize()
    ));
    bucketCache->put(bucketFile, bucket);
  }

  return bucket;
}

uint64_t DB::bucketZoom() {
  return pimpl_->idx->bucketZoom();
}

uint64_t DB::tileZoom() {
  return pimpl_->idx->tileZoom();
}

} // namespace
