#include "db.hpp"

#include <gtest/gtest.h>

#include <fstream>

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;
using namespace tilecone;

TEST(DB, DB_Can_OpenDatabase) {
  std::string dirName = "__test_db1";

  {
    fs::path dPath(dirName);
    fs::remove_all(dPath);
    fs::create_directory(dPath);
    std::ofstream fout((dPath / "index.ini").native());
    fout
      << "[Tiles]\n"
      << "BucketZoom = 10\n"
      << "TileZoom = 15\n"
      << "BlockSize = 1024\n";
  }

  DB db(dirName, 10);
}

TEST(DB, DB_Can_WriteData_To_Database) {
  std::string dirName = "__test_db2";
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

  DB db(dirName, 10);

  std::string testData = "My test string";
  db.setTile(316893, 163547, testData.data(), testData.size());
}


TEST(DB, DB_Can_WriteData_To_Database_And_Read_ItBack) {
  std::string dirName = "__test_db3";
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

  DB db(dirName, 10);

  std::string testData = "My test string";
  db.setTile(316893, 163547, testData.data(), testData.size());

  std::tuple<void const*, Tile const*, size_t> tilesInfo = db.getTiles(19, 316893, 163547);

  ASSERT_EQ(1, std::get<2>(tilesInfo));
  Tile const* ti = std::get<1>(tilesInfo);
  std::string resData = std::string(
    (char *)std::get<0>(tilesInfo) + ti->offset,
    ti->size
  );
  ASSERT_EQ(testData, resData);
}