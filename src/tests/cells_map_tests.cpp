#include <gtest/gtest.h>

#include <app/cells_map.hpp>
#include <app/spreadsheet.hpp>

TEST(CellsMapTest, Get_WillReturnValidPtr) {
  CellsMap map;
  EXPECT_NE(map.Get("A0"), nullptr);
  EXPECT_NE(map.Get("C500"), nullptr);
}

TEST(CellsMapTest, Find_EmptyMap_WillReturnNullptr) {
  CellsMap map;
  EXPECT_EQ(map.Find("A0"), nullptr);
  EXPECT_EQ(map.Find("A4000"), nullptr);
}

TEST(CellsMapTest, Find_FilledMap_WillReturnValidPtr) {
  CellsMap map;
  map.Get("A10");
  EXPECT_NE(map.Find("A0"), nullptr);
}