#include "core_elements.hpp"

#include "cell_proxy.hpp"

/*
  IntValue
*/
IntValue::IntValue(int v) noexcept
  : value_(v) {}

CellValue IntValue::Calculate(uint32_t version) noexcept {
  return value_;
}

/* 
  ReferenceElement 
*/
ReferenceElement::ReferenceElement(CellProxy* ref) noexcept
  : ref_(ref) {}

CellValue ReferenceElement::Calculate(uint32_t version) noexcept {
  return ref_->CalculateForward(version);
}