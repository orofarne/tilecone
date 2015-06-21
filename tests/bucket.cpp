#include "bucket.hpp"

#include <gtest/gtest.h>

#include <memory>

using namespace tilecone;

TEST(Bucket, Bucket_Should_CreatesNewFile_WithoutError) {
  std::unique_ptr<Bucket> bucket;

  bucket.reset(new Bucket("__test_new_file.cone", 13, 19, 0, 0));

  ASSERT_TRUE((bool)bucket);
}
