#include "spreadsheet.hpp"

#include <fstream>
#include <assert.h>

#include "timer.hpp"
#include "logger.hpp"
#include "formula/parser.hpp"

Spreadsheet::Spreadsheet(std::unique_ptr<CellsMap> map, 
                         std::unique_ptr<CellChanger> changer,
                         ILogger* logger)
  : cells_(std::move(map))
  , changer_(std::move(changer))
  , logger_(logger) {}

CellValue Spreadsheet::GetCellValue(std::string_view name) const {
  auto* cell = cells_->Find(name);
  if (!cell) return CellValue();
  return CellValue(cell->Value());
}

std::string Spreadsheet::GetCellFormula(std::string_view name) const {
  const auto* cell = cells_->Find(name);
  if (!cell) return "";
  return cell->Formula();
}

CellValue Spreadsheet::ChangeCell(const std::string& name, const std::string& formula) {
  auto cell_obj = cells_->Get(name);
  assert(cell_obj);

  std::vector<CellProxy*> refs;
  auto formula_obj = ParseFormula(formula, cell_obj, cells_.get(), refs);

  Timer timer;
  cell_obj->Change(std::move(formula_obj), formula);
  changer_->Change(cell_obj);

  LOG(logger_) << "Change time: " << timer.DurationSec() << "s";
  return cell_obj->Value();
}

void Spreadsheet::Save(const std::filesystem::path& filename) const {
  std::ofstream file(filename);

  auto save_fn = [&file](const CellProxy* cell) {
    file << cell->Name() << " = " << cell->Value() << "\n";
  };

  cells_->Visit(save_fn);
  file.close();
}