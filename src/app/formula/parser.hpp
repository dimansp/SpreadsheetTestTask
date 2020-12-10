#pragma once

#include <vector>
#include <string_view>

#include "i_formula_element.hpp"

class CellsMap;
class CellProxy;

std::unique_ptr<IFormulaElement> ParseFormula(std::string_view formula, 
                                              CellProxy* me, 
                                              CellsMap* cells,
                                              std::vector<CellProxy*>& refs) noexcept;