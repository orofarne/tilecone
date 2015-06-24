#include "index.hpp"

#include <gtest/gtest.h>

#include <fstream>

using namespace tilecone;

TEST(Index, Index_Can_LoadFile) {
  std::string fName = "__test_index.ini";

  {
    std::ofstream fout(fName);
    fout
      << "[Tiles]\n"
      << "BucketZoom = 10\n"
      << "TileZoom = 15\n";
  }

  Index idx(fName);

  ASSERT_EQ(10, idx.bucketZoom());
  ASSERT_EQ(15, idx.tileZoom());
}
