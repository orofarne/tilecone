#include "db.h"

#include <gtest/gtest.h>

#include <fstream>

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

TEST(DB_C, DB_Can_WriteData_To_Database_And_Read_ItBack) {
  std::string dirName = "__test_db3_c";
  fs::path dPath(dirName);

  {
    fs::remove_all(dPath);
    fs::create_directory(dPath);
    std::ofstream fout((dPath / "index.ini").native());
    fout
      << "[Tiles]\n"
      << "BucketZoom = 15\n"
      << "TileZoom = 19\n"
      << "BlockSize = 1024\n";
  }

  tc_db d = tc_new_db(dirName.c_str(), 10);

  std::string testData = "My test string";
  tc_set_tile(d, 316893, 163547, testData.data(), testData.size());

  void const* buf;
  struct tc_tile const* tiles;
  size_t tiles_len;
  tc_get_tiles(d, 19, 316893, 163547, &buf, &tiles, &tiles_len);

  ASSERT_EQ(1, tiles_len);
  std::string resData = std::string(
    (char *)buf + tiles[0].offset,
    tiles[0].size
  );
  ASSERT_EQ(testData, resData);

  tc_free_db(d);
}

TEST(DB_C, DB_Can_Load_DBInfo) {
  std::string dirName = "__test_db3_c";
  fs::path dPath(dirName);

  {
    fs::remove_all(dPath);
    fs::create_directory(dPath);
    std::ofstream fout((dPath / "index.ini").native());
    fout
      << "[Tiles]\n"
      << "BucketZoom = 15\n"
      << "TileZoom = 19\n"
      << "BlockSize = 1024\n";
  }

  tc_db d = tc_new_db(dirName.c_str(), 10);

  ASSERT_EQ(15, tc_bucket_zoom(d));
  ASSERT_EQ(19, tc_tile_zoom(d));

  tc_free_db(d);
}
