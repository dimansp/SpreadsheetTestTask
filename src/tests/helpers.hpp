#include <gtest/gtest.h>

#include <app/spreadsheet.hpp>

#define EXPECT_EMPTY(cell_value) EXPECT_TRUE(IsEmpty(cell_value));
#define EXPECT_INVALID(cell_value) EXPECT_TRUE(IsInvalid(cell_value));
#define EXPECT_INT(cell_value, i) EXPECT_TRUE(IsInt(cell_value));         \
                                  EXPECT_EQ(std::get<int>(cell_value), i);
