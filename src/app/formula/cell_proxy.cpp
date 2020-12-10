#include "cell_proxy.hpp"

#include <assert.h>
#include <unordered_map>
#include <map>
#include "../helpers.hpp"

CellProxy::CellProxy(Block* _block, const Reference& r)
  : reference(r)
  , block(_block) {}
  
CellValue CellProxy::Value() const {
  std::lock_guard<std::recursive_mutex> lock(m_);
  return value_;
}
std::string CellProxy::Formula() const {
  const std::lock_guard<std::recursive_mutex> lock(m_);
  return formula_;
}
std::string CellProxy::Name() const {
  return ReferenceToString(reference);
}

void CellProxy::Change(std::unique_ptr<IFormulaElement>&& e, 
                       const std::string& formula) {
  
  const std::lock_guard<std::recursive_mutex> lock(m_);

  element_ = std::move(e);
  formula_ = formula;
}

void CellProxy::Load(std::unique_ptr<IFormulaElement>&& e, 
                     const std::string& formula) {
  element_ = std::move(e);
  formula_ = formula;
}

CellValue CellProxy::CalculateForward(uint32_t version) {
  const std::lock_guard<std::recursive_mutex> lock(m_);

  assert(invalidate_version_ >= version);
  if (invalidate_version_ > version) return value_;

  assert(invalidate_version_ >= version_);
  if (invalidate_version_ == version_) return value_;

  value_ = CalculateByElementNoLock(version);
  version_ = version;
  return value_;
}

CellValue CellProxy::CalculateByElementNoLock(uint32_t version) {
  if (!element_) return CellValue(EmptyValue());
  return element_->Calculate(version);
}

void CellProxy::Invalidate(uint32_t version) {
  const std::lock_guard<std::recursive_mutex> lock(m_);

  if (invalidate_version_ >= version) return;
  invalidate_version_ = version;
}

bool CellProxy::IsEmpty() const {
  const std::lock_guard<std::recursive_mutex> lock(m_);
  return element_ == nullptr;
}