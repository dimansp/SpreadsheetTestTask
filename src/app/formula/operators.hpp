#pragma once

#include "i_formula_element.hpp"

/*
  class BinaryOperatorElement
*/
template<class Fn>
class BinaryOperatorElement: public IFormulaElement {
public:
  BinaryOperatorElement(std::unique_ptr<IFormulaElement>&& left, 
                        std::unique_ptr<IFormulaElement>&& right) noexcept;

  CellValue Calculate(uint32_t version) noexcept override;

private:
  std::unique_ptr<IFormulaElement> left_;
  std::unique_ptr<IFormulaElement> right_;
};

template<class Fn>
BinaryOperatorElement<Fn>::BinaryOperatorElement(std::unique_ptr<IFormulaElement>&& left, 
                                                 std::unique_ptr<IFormulaElement>&& right) noexcept
    : left_(std::move(left))
    , right_(std::move(right)) {}

template<class Fn>
CellValue BinaryOperatorElement<Fn>::Calculate(uint32_t version) noexcept {
  auto lvalue = left_->Calculate(version);
  if (IsInvalid(lvalue)) return CellValue(InvalidValue());

  auto rvalue = right_->Calculate(version);
  if (IsInvalid(rvalue)) return CellValue(InvalidValue());

  if (IsEmpty(lvalue) && IsEmpty(rvalue)) return CellValue(EmptyValue());
  if (IsEmpty(lvalue)) return CellValue(InvalidValue());
  if (IsEmpty(rvalue)) return CellValue(InvalidValue());

  // TODO: better rules different left/right result types

  assert(IsInt(lvalue));
  assert(IsInt(rvalue));

  return CellValue(Fn::Do(std::get<int>(lvalue), std::get<int>(rvalue)));
}

/*
  Binary Operators Implementation
*/

struct PlusFunctor {
  static int Do(int left, int right) {
    return left + right;
  }
};
using PlusElement = BinaryOperatorElement<PlusFunctor>;

struct MinusFunctor {
  static int Do(int left, int right) {
    return left - right;
  }
};
using MinusElement = BinaryOperatorElement<MinusFunctor>;

