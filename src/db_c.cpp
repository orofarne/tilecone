#include "db.h"
#include "db.hpp"

#include <stddef.h>

#include <memory>
#include <string>
#include <exception>

using namespace tilecone;

struct __tc_db {
  std::unique_ptr<DB> db_;
  std::string lastError_;
};

extern "C" tc_db tc_new_db(const char* path, int mmappool) {
  tc_db res;

  try {
    res = new struct __tc_db;
  }
  catch(...) {
    return NULL;
  }

  try {
    res->db_.reset(new DB(path, mmappool));
  }
  catch(std::exception const& e) {
    res->lastError_ = e.what();
  }
  catch(...) {
    res->lastError_ = "UNKNOWN ERROR";
  }

  return res;
}

extern "C" tc_db tc_free_db(tc_db d) {
  delete d;
}

extern "C" int tc_db_ok(tc_db d) {
  return d && d->lastError_.empty();
}

extern "C" const char* tc_last_error(tc_db d) {
  return d->lastError_.c_str();
}

#define TESTDB \
if (!d->db_) { \
  d->lastError_ = "Invalid DB object"; \
  return 1; \
}

#define CTRY( expr ) \
try { expr } \
catch(std::exception const& e) { \
  d->lastError_ = e.what(); \
  return 1; \
} \
catch(...) { \
  d->lastError_ = "UNKNOWN ERROR"; \
  return 1; \
} \
return 0;

// Returns subtiles of a tile
extern "C" int tc_get_tiles(tc_db d, uint16_t zoom, uint64_t x, uint64_t y, void const** buf, struct tc_tile const** tiles, size_t *tiles_len) {
  if (!d->db_) {
    d->lastError_ = "Invalid DB object";
    return 1;
  }

  try {
    Tile const* tilespp;
    std::tie(*buf, tilespp, *tiles_len) = d->db_->getTiles(zoom, x, y);
    *tiles = (struct tc_tile const*)tilespp;
  }
  catch(std::exception const& e) {
    d->lastError_ = e.what();
    return 1;
  }
  catch(...) {
    d->lastError_ = "UNKNOWN ERROR";
    return 1;
  }

  return 0;
}

// Set new tile data. TileZoom only!
extern "C" int tc_set_tile(tc_db d, uint64_t x, uint64_t y, void const* data, size_t data_size) {
  if (!d->db_) {
    d->lastError_ = "Invalid DB object";
    return 1;
  }

  try {
    d->db_->setTile(x, y, data, data_size);
  }
  catch(std::exception const& e) {
    d->lastError_ = e.what();
    return 1;
  }
  catch(...) {
    d->lastError_ = "UNKNOWN ERROR";
    return 1;
  }

  return 0;
}

extern "C" uint64_t tc_bucket_zoom(tc_db d) {
  TESTDB;

  uint64_t bucketZoom;
  try {
    bucketZoom = d->db_->bucketZoom();
  }
  catch(...) {
    d->lastError_ = "bucketZoom() exception";
  }
  return bucketZoom;
}

extern "C" uint64_t tc_tile_zoom(tc_db d) {
  TESTDB;

  uint64_t tileZoom;
  try {
    tileZoom = d->db_->tileZoom();
  }
  catch(...) {
    d->lastError_ = "tileZoom() exception";
  }
  return tileZoom;
}
