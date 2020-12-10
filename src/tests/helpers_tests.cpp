#include <gtest/gtest.h>

#include <app/helpers.hpp>

TEST(HelpersTest, IndexToAlphabet) {
  EXPECT_EQ("A", IndexToAlphabet(0));
  EXPECT_EQ("C", IndexToAlphabet(2));
}

TEST(HelpersTest, IndexToInt) {
  EXPECT_EQ("0", IndexToInt(0));
  EXPECT_EQ("5", IndexToInt(5));
}