#include "bucket.hpp"

#include <gtest/gtest.h>

#include <memory>

#include <boost/filesystem.hpp>

using namespace tilecone;
namespace fs = boost::filesystem;

TEST(Bucket, Bucket_Should_CreatesNewFile_WithoutError) {
  std::string fName = "__test_new_file.cone";

  // Remove old file
  fs::remove(fName);

  std::unique_ptr<Bucket> bucket(new Bucket(fName, 13, 19, 0, 0, 1024));

  ASSERT_TRUE((bool)bucket);
}

TEST(Bucket, Bucket_Should_Can_StoreItem) {
  std::string fName = "__test_store.cone";

  // Remove old file
  fs::remove(fName);

  std::unique_ptr<Bucket> bucket(new Bucket(fName, 15, 19, 19805, 10221, 1024));

  std::string testData = "My test string";
  bucket->setTile(316894, 163547, testData.data(), testData.size());
}

TEST(Bucket, Bucket_Should_Can_StoreItem_And_ReadItBack) {
  std::string fName = "__test_store.cone";

  // Remove old file
  fs::remove(fName);

  std::unique_ptr<Bucket> bucket(new Bucket(fName, 15, 19, 19805, 10221, 1024));

  std::string testData = "My test string";
  bucket->setTile(316893, 163547, testData.data(), testData.size());

  std::tuple<void const*, Tile const*, size_t> tilesInfo = bucket->getTiles(19, 316893, 163547);

  ASSERT_EQ(1, std::get<2>(tilesInfo));
  Tile const* ti = std::get<1>(tilesInfo);
  std::string resData = std::string(
    (char *)std::get<0>(tilesInfo) + ti->offset,
    ti->size
  );
  ASSERT_EQ(testData, resData);
}
