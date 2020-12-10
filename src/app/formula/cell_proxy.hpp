#pragma once

#include <mutex>
#include <vector>

#include "i_formula_element.hpp"
#include "../helpers.hpp"

class CellsMap;
template<uint32_t I, uint32_t A>
class CellsBlock;

class CellProxy {
public:
  CellProxy(Block* _block, const Reference& r);

  CellValue Value() const;
  std::string Formula() const;
  std::string Name() const;

  void Change(std::unique_ptr<IFormulaElement>&& e, 
              const std::string& formula);

  void Load(std::unique_ptr<IFormulaElement>&& e, 
            const std::string& formula);

  CellValue CalculateForward(uint32_t version);
  void Invalidate(uint32_t version);

  void AppendBackRefs(std::vector<CellProxy*>& found_cells);

  bool IsEmpty() const;

  const Reference reference;
  Block* const block;

private:
  CellValue CalculateByElementNoLock(uint32_t version);
  
  mutable std::recursive_mutex m_;

  uint32_t version_ = 0;
  uint32_t invalidate_version_ = 1;
  
  std::string formula_;

  CellValue value_ = CellValue(EmptyValue());

  std::unique_ptr<IFormulaElement> element_ = nullptr;
};
