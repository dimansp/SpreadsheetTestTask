#pragma once

#include <string>
#include <mutex>
#include <vector>
#include <filesystem>
#include <unordered_map>
#include <thread>
#include <variant>

#include "spreadsheet.hpp"

class SpreadsheetLoader {
public:
  SpreadsheetLoader(ILogger* logger);

  Spreadsheet Load(const std::filesystem::path& filename);
  Spreadsheet Load(std::istream& stream);

private:
  void ParseInputStream(std::istream& stream, CellsMap& cells);
  void CalculateAll(int number_of_threads, uint32_t version);
  void SetupBackRefs();

  ILogger* logger_;
  std::vector<CellProxy*> loaded_cells_;
};