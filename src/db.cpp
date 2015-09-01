#include "db.hpp"
#include "index.hpp"
#include "bucket.hpp"

#include "lrucache.hpp"

#include <stdexcept>

#include <boost/filesystem.hpp>
#include <boost/format.hpp>

#ifdef TILECONE_MT
#include <boost/thread/mutex.hpp>
#endif

namespace fs = boost::filesystem;

namespace tilecone {

using BucketPtr = std::shared_ptr<Bucket>;

struct DB::Pimpl {
  int mmappool;
  fs::path basePath;
  std::unique_ptr<Index> idx;
  std::unique_ptr<cache::lru_cache<std::string, BucketPtr>> bucketCache;
#ifdef TILECONE_MT
  boost::mutex cache_mutex;
#endif

  BucketPtr getBucket(uint16_t zoom, uint64_t x, uint64_t y, bool create);
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
  try {
    auto bucket = pimpl_->getBucket(zoom, x, y, false);
    return bucket->getTiles(zoom, x, y);
  }
  catch(Bucket::NotFound const&) {
    return std::make_tuple((void *)nullptr, (Tile const*)nullptr, (size_t)0);
  }
}

void
DB::setTile(uint64_t x, uint64_t y, void const* data, size_t dataSize) {
  auto bucket = pimpl_->getBucket(pimpl_->idx->tileZoom(), x, y, true);
  bucket->setTile(x, y, data, dataSize);
}

BucketPtr
DB::Pimpl::getBucket(uint16_t zoom, uint64_t x, uint64_t y, bool create) {
#ifdef TILECONE_MT
   boost::mutex::scoped_lock scoped_lock(cache_mutex);
#endif

  auto M = zoom - idx->bucketZoom();
  auto x1 = M >= 0 ? x >> M : x << M;
  auto y1 = M >= 0 ? y >> M : x << M;

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
      idx->blockSize(),
      create
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
