#include "spreadsheet_loader.hpp"

#include <fstream>

#include "logger.hpp"
#include "timer.hpp"
#include "formula/parser.hpp"

SpreadsheetLoader::SpreadsheetLoader(ILogger* logger)
  : logger_(logger) {}

Spreadsheet SpreadsheetLoader::Load(const std::filesystem::path& filename) {
  std::ifstream file(filename);
  return Load(file);
}

Spreadsheet SpreadsheetLoader::Load(std::istream& stream) {
  loaded_cells_.clear();
  auto map = std::make_unique<CellsMap>();

  Timer timer;
  ParseInputStream(stream, *map);
  
  auto begin_calc_stemp = timer.DurationSec();
  auto changer = std::make_unique<CellChanger>(loaded_cells_);
  auto end_stemp = timer.DurationSec();

  LOG(logger_) << "CalculateAll: " << end_stemp - begin_calc_stemp << "s";
  LOG(logger_) << "Load: " << end_stemp << "s";

  return Spreadsheet(std::move(map), std::move(changer), logger_);
}

void SpreadsheetLoader::ParseInputStream(std::istream& stream, CellsMap& cells) {
  Timer timer;
  
  while (!stream.eof()) {
    std::string cell, eq, formula;
    stream >> cell >> eq >> formula;
        
    if (cell.empty()) continue;

    //TODO: check erros here instead asserts
    assert(eq == "=");
    assert(!formula.empty());

    auto cell_obj = cells.Get(cell);
    assert(cell_obj);

    std::vector<CellProxy*> forward_refs;
    auto formula_obj = ParseFormula(formula, cell_obj, &cells, forward_refs);
    assert(formula_obj);
    cell_obj->Load(std::move(formula_obj), formula);

    loaded_cells_.push_back(cell_obj);
  }

  LOG(logger_) << "Parse file: " << timer.DurationSec() << "s"; 
}

void SpreadsheetLoader::CalculateAll(int num_of_threads, uint32_t version) {
  auto calc_function = [this, version](int offset, int threads) {
    for (auto i = offset; i < loaded_cells_.size(); i += threads)
      loaded_cells_[i]->CalculateForward(version);
  };

  if (num_of_threads == 1) {
    calc_function(0, num_of_threads);
    return;
  }
  
  std::vector<std::thread> threads;
  threads.reserve(num_of_threads);
  for (auto i = 0; i < num_of_threads; i++)
    threads.emplace_back(calc_function, i, num_of_threads);
  
  for (auto i = 0; i < num_of_threads; i++)
    threads[i].join();
}