#include <gtest/gtest.h>

#include <app/spreadsheet_loader.hpp>
#include <app/logger.hpp>
#include "helpers.hpp"

class SpreadsheetTest: public ::testing::Test {
public:
  SpreadsheetTest()
    : loader_(&logger_) {}

protected:
  NullLogger logger_;
  SpreadsheetLoader loader_;
};

TEST_F(SpreadsheetTest, ChangeCell_IntToEmpty_WillReturnEmpty) {
  std::stringstream stream;
  stream << "A1 = 5";
  auto spreadsheet = loader_.Load(stream);

  EXPECT_EMPTY(spreadsheet.ChangeCell("A1", ""));
}

TEST_F(SpreadsheetTest, ChangeCell_IntToEmpty_CellWillBeEmpty) {
  std::stringstream stream;
  stream << "A1 = 5";
  auto spreadsheet = loader_.Load(stream);

  spreadsheet.ChangeCell("A1", "");
  EXPECT_EMPTY(spreadsheet.GetCellValue("A1"));
}

TEST_F(SpreadsheetTest, ChangeCell_IntChangeToEmpty_SumOfRefsWillBeInvalid) {
  std::stringstream stream;
  stream << "A1 = 5\n"
         << "A2 = 1\n"
         << "A3 = A1+A2";
  auto spreadsheet = loader_.Load(stream);

  spreadsheet.ChangeCell("A1", "");
  EXPECT_INVALID(spreadsheet.GetCellValue("A3"));
}

TEST_F(SpreadsheetTest, ChangeCell_ComplexDependancies) {
  std::stringstream stream;
  stream << "A1 = 5\n"
         << "A2 = A1+A3\n"
         << "A3 = A1+2";
  auto spreadsheet = loader_.Load(stream);

  spreadsheet.ChangeCell("A1", "10");
  EXPECT_INT(spreadsheet.GetCellValue("A2"), 22);
}

TEST_F(SpreadsheetTest, ChangeCell_IntChangeToEmpty_RefsWillBeEmpty) {
  std::stringstream stream;
  stream << "A1 = 5\n"
         << "A2 = A1";
  auto spreadsheet = loader_.Load(stream);

  spreadsheet.ChangeCell("A1", "");
  EXPECT_EMPTY(spreadsheet.GetCellValue("A2"));
}

TEST_F(SpreadsheetTest, ParserTest_Int) {
  std::stringstream stream;
  stream << "A0 = 1";

  auto spreadsheet = loader_.Load(stream);

  EXPECT_INT(spreadsheet.GetCellValue("A0"), 1);
}

TEST_F(SpreadsheetTest, ParserTest_Ref) {
  std::stringstream stream;
  stream << "A0 = 5\n"
         << "B1 = A0";

  auto spreadsheet = loader_.Load(stream);

  EXPECT_INT(spreadsheet.GetCellValue("B1"), 5);
}

TEST_F(SpreadsheetTest, ParserTest_OperatorPlus) {
  std::stringstream stream;
  stream << "A1 = 5+10+15+20";
  auto spreadsheet = loader_.Load(stream);

  EXPECT_INT(spreadsheet.GetCellValue("A1"), 50);
}

TEST_F(SpreadsheetTest, ParserTest_OperatorMinus) {
  std::stringstream stream;
  stream << "A1 = 99-9-10";
  auto spreadsheet = loader_.Load(stream);

  EXPECT_INT(spreadsheet.GetCellValue("A1"), 80);
}

TEST_F(SpreadsheetTest, ParserTest_PlusMinusMix) {
  std::stringstream stream;
  stream << "A1 = 99-9+10-1";
  auto spreadsheet = loader_.Load(stream);

  EXPECT_INT(spreadsheet.GetCellValue("A1"), 99);
}

TEST_F(SpreadsheetTest, Dependancies_1Level) {
  std::stringstream stream;
  stream << "A0 = 1\n"
         << "B1 = A0+5";
  auto spreadsheet = loader_.Load(stream);

  spreadsheet.ChangeCell("A0", "100");
  EXPECT_INT(spreadsheet.GetCellValue("B1"), 105);
}

TEST_F(SpreadsheetTest, Dependancies_2Level) {
  std::stringstream stream;
  stream << "A0 = 100\n"
         << "B1 = A0+5\n"
         << "B2 = B1+5";
  auto spreadsheet = loader_.Load(stream);

  spreadsheet.ChangeCell("A0", "50");
  EXPECT_INT(spreadsheet.GetCellValue("B2"), 60);
}