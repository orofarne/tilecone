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

TEST(DB, DB_Can_WriteData_To_Database_WithCorrectName) {
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

  ASSERT_TRUE( fs::exists(dPath / "19805_10221.cone") );
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

TEST(DB, DB_Can_Load_DBInfo) {
  std::string dirName = "__test_db4";
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

  ASSERT_EQ(15, db.bucketZoom());
  ASSERT_EQ(19, db.tileZoom());
}

TEST(DB, DB_Can_WriteData_MultipleTimes_To_Database_And_Read_ItBack) {
  std::string dirName = "__test_db5";
  fs::path dPath(dirName);

  {
    fs::remove_all(dPath);
    fs::create_directory(dPath);
    std::ofstream fout((dPath / "index.ini").native());
    fout
      << "[Tiles]\n"
      << "BucketZoom = 15\n"
      << "TileZoom = 19\n"
      << "BlockSize = 10\n";
  }

  DB db(dirName, 1);

  std::string testData = "My";
  db.setTile(316893, 163547, testData.data(), testData.size());
  testData += " biiiiiiiiiiiiiiiiiiiiiiiiiiiig";
  db.setTile(316893, 163547, testData.data(), testData.size());
  testData += " test string with unicorns";
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

TEST(DB, DB_Can_WriteData_MultipleTimes_WithSmallString_To_Database_And_Read_ItBack) {
  std::string dirName = "__test_db6";
  fs::path dPath(dirName);

  {
    fs::remove_all(dPath);
    fs::create_directory(dPath);
    std::ofstream fout((dPath / "index.ini").native());
    fout
      << "[Tiles]\n"
      << "BucketZoom = 15\n"
      << "TileZoom = 19\n"
      << "BlockSize = 10\n";
  }

  DB db(dirName, 1);

  std::string testData = "My";
  db.setTile(316893, 163547, testData.data(), testData.size());
  testData += " biiiiiiiiiiiiiiiiiiiiiiiiiiiig test string with unicorns";
  db.setTile(316893, 163547, testData.data(), testData.size());
  testData = "My small string";
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

TEST(DB, DB_Can_WriteData_MultipleTimes_To_Database_And_Read_Back_NextCell) {
  std::string dirName = "__test_db7";
  fs::path dPath(dirName);

  {
    fs::remove_all(dPath);
    fs::create_directory(dPath);
    std::ofstream fout((dPath / "index.ini").native());
    fout
      << "[Tiles]\n"
      << "BucketZoom = 15\n"
      << "TileZoom = 19\n"
      << "BlockSize = 10\n";
  }

  DB db(dirName, 1);

  std::string testData = "My test string";
  db.setTile(316893, 163547, testData.data(), testData.size());
  std::string testData2 = "string";
  db.setTile(316893, 163546, testData2.data(), testData2.size());
  testData2 += " with sooooooooooooooo biiiiiiiiiiiiiiiiiiiiiiiiiiiig unicorns";
  db.setTile(316893, 163546, testData2.data(), testData2.size());

  std::tuple<void const*, Tile const*, size_t> tilesInfo = db.getTiles(19, 316893, 163547);

  ASSERT_EQ(1, std::get<2>(tilesInfo));
  Tile const* ti = std::get<1>(tilesInfo);
  std::string resData = std::string(
    (char *)std::get<0>(tilesInfo) + ti->offset,
    ti->size
  );
  ASSERT_EQ(testData, resData);
}

TEST(DB, DB_Can_WriteData_To_Database_And_Read_ItBack_On_AnotherZoom) {
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

  std::tuple<void const*, Tile const*, size_t> tilesInfo = db.getTiles(18, 158446, 81773);

  ASSERT_EQ(4, std::get<2>(tilesInfo));
  Tile const* ti = std::get<1>(tilesInfo);
  ti += 3;
  std::string resData = std::string(
    (char *)std::get<0>(tilesInfo) + ti->offset,
    ti->size
  );
  ASSERT_EQ(testData, resData);
}
