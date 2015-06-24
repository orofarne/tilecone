#include "db.hpp"
#include "index.hpp"

#include <stdexcept>

#include <boost/filesystem.hpp>
#include <boost/format.hpp>

namespace fs = boost::filesystem;

namespace tilecone {

struct DB::Pimpl {
  int mmappool;
  fs::path basePath;
  std::unique_ptr<Index> idx;
};

DB::DB(std::string const& path, int mmappool) {
  if (!fs::is_directory(path)) {
    throw std::invalid_argument(str( boost::format("%1% is not a directory") % path ));
  }

  pimpl_.reset(new Pimpl);

  pimpl_->mmappool = mmappool;
  pimpl_->basePath = fs::path(path);
  pimpl_->idx.reset(new Index((pimpl_->basePath / "index.ini").native()));
}

DB::~DB() {

}

std::tuple<void const*, Tile const*, size_t>
DB::getTiles(uint16_t zoom, uint64_t x, uint64_t y) {

}

void
DB::setTile(uint64_t x, uint64_t y, void const* data, size_t dataSize) {

}

} // namespace
