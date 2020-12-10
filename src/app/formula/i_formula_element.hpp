#pragma once

#include "cell_value.hpp"

/*
  class IFormulaElement
*/
struct IFormulaElement {
  virtual ~IFormulaElement() noexcept {}

  virtual CellValue Calculate(uint32_t version) noexcept = 0;
};
