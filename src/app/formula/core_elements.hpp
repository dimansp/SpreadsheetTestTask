#pragma once

#include "i_formula_element.hpp"

class CellProxy;

/*
  class IntValue
*/
class IntValue: public IFormulaElement {
public:
  explicit IntValue(int v) noexcept;

  CellValue Calculate(uint32_t version) noexcept override;

private:
  const int value_;
};

/*
  class ReferenceElement
*/
class ReferenceElement: public IFormulaElement {
public:
  explicit ReferenceElement(CellProxy* ref) noexcept;

  CellValue Calculate(uint32_t version) noexcept override;

private:
  CellProxy* ref_;
};