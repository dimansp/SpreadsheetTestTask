#pragma once

#include <string_view>
#include <vector>
#include <filesystem>
#include <atomic>

#include "cells_map.hpp"
#include "cell_changer.hpp"

struct ILogger;

class Spreadsheet {
public:
  Spreadsheet(std::unique_ptr<CellsMap> map,  
              std::unique_ptr<CellChanger> changer,
              ILogger* logger);

  CellValue GetCellValue(std::string_view name) const;
  std::string GetCellFormula(std::string_view name) const;
  CellValue ChangeCell(const std::string& name, const std::string& formula);

  void Save(const std::filesystem::path& filename) const;

private:
  std::unique_ptr<CellsMap> cells_;
  std::unique_ptr<CellChanger> changer_;
  ILogger* logger_;
};
