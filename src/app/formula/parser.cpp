#include "parser.hpp"

#include "core_elements.hpp"
#include "operators.hpp"
#include "../cells_map.hpp"

using OperatorAction = std::function<std::unique_ptr<IFormulaElement>(std::unique_ptr<IFormulaElement>&& left, 
                                                                      std::unique_ptr<IFormulaElement>&& right)>;

template<typename BinaryOperator>
std::unique_ptr<IFormulaElement> MakeBinaryOperatorElement(std::unique_ptr<IFormulaElement>&& left, 
                                                           std::unique_ptr<IFormulaElement>&& right) {
  return std::make_unique<BinaryOperator>(std::move(left), std::move(right));
}

const std::unordered_map<char, OperatorAction> FormulaOperators = {
  {'+', &MakeBinaryOperatorElement<PlusElement>},
  {'-', &MakeBinaryOperatorElement<MinusElement>},
};

std::unique_ptr<IFormulaElement> TryMakeCoreElement(std::string_view value_str,
                                                    CellsMap* cells,
                                                    std::vector<CellProxy*>& in_formula_refs) noexcept {
  if (value_str.empty()) return nullptr;

  if (IsInt(value_str)) return std::make_unique<IntValue>(atoi(value_str.data()));
  
  auto me = cells->Get(value_str);
  in_formula_refs.push_back(me);
  return std::make_unique<ReferenceElement>(me);
}

std::unique_ptr<IFormulaElement> ParseFormula(std::string_view formula, 
                                              CellProxy* me, 
                                              CellsMap* cells,
                                              std::vector<CellProxy*>& in_formula_refs) noexcept {
  assert(me);

  std::string part;
  std::unique_ptr<IFormulaElement> root;
  const OperatorAction* last_operator = nullptr;

  // HACK: Theren't any validations!!!
  // TODO: Add validations

  for (auto c: formula) {
    const auto& it = FormulaOperators.find(c);
    if (it != FormulaOperators.end()) {
      if (!last_operator) {
        assert(!root);
        root = TryMakeCoreElement(part, cells, in_formula_refs);
      } else {
        assert(root);
        root = (*last_operator)(std::move(root), TryMakeCoreElement(part, cells, in_formula_refs));
      }

      part.clear();
      last_operator = &it->second;
      continue;
    }
    part += c;
  }

  if (!last_operator) {
    assert(!root);
    return TryMakeCoreElement(part, cells, in_formula_refs);
  }

  assert(root);
  assert(last_operator);
  return (*last_operator)(std::move(root), TryMakeCoreElement(part, cells, in_formula_refs));
}