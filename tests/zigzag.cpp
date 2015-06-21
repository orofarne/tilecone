#include "zigzag.hpp"

#include <gtest/gtest.h>

using namespace tilecone::utils;

TEST(ZigZag, zigZagPosition_ShouldWorks_With_ZoomDiff_1) {
  // +---+---+
  // | 0 | 1 |
  // +---+---+
  // | 2 | 3 |
  // +---+---+

  EXPECT_EQ(0, zigZagPosition(1, 0, 0));
  EXPECT_EQ(1, zigZagPosition(1, 1, 0));
  EXPECT_EQ(2, zigZagPosition(1, 0, 1));
  EXPECT_EQ(3, zigZagPosition(1, 1, 1));
}

TEST(ZigZag, zigZagPosition_ShouldWorks_With_ZoomDiff_2) {
  // +---+---+---+---+
  // | 0 | 1 | 4 | 5 |
  // +---+---+---+---+
  // | 2 | 3 | 6 | 7 |
  // +---+---+---+---+
  // | 8 | 9 | 12| 13|
  // +---+---+---+---+
  // | 10| 11| 14| 15|
  // +---+---+---+---+

  EXPECT_EQ(0, zigZagPosition(2, 0, 0));
  EXPECT_EQ(1, zigZagPosition(2, 1, 0));
  EXPECT_EQ(2, zigZagPosition(2, 0, 1));
  EXPECT_EQ(3, zigZagPosition(2, 1, 1));

  EXPECT_EQ(4, zigZagPosition(2, 2, 0));
  EXPECT_EQ(5, zigZagPosition(2, 3, 0));
  EXPECT_EQ(6, zigZagPosition(2, 2, 1));
  EXPECT_EQ(7, zigZagPosition(2, 3, 1));

  EXPECT_EQ(8, zigZagPosition(2, 0, 2));
  EXPECT_EQ(9, zigZagPosition(2, 1, 2));
  EXPECT_EQ(10, zigZagPosition(2, 0, 3));
  EXPECT_EQ(11, zigZagPosition(2, 1, 3));

  EXPECT_EQ(12, zigZagPosition(2, 2, 2));
  EXPECT_EQ(13, zigZagPosition(2, 3, 2));
  EXPECT_EQ(14, zigZagPosition(2, 2, 3));
  EXPECT_EQ(15, zigZagPosition(2, 3, 3));
}

TEST(ZigZag, zigZagPosition_ShouldFind_33th_Position_On_ZoomDiff_3) {
  //     0   1   2   3
  //   +---+---+---+---+---
  // 0 | 0 | 1 | 4 | 5 |
  //   +---+---+---+---+---
  // 1 | 2 | 3 | 6 | 7 |
  //   +---+---+---+---+---
  // 2 | 8 | 9 | 12| 13|
  //   +---+---+---+---+---
  // 3 | 10| 11| 14| 15|
  //   +---+---+---+---+---
  // 4 | 32| 33|   |   |
  //

  EXPECT_EQ(33, zigZagPosition(3, 1, 4));
}
