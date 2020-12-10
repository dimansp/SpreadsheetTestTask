#pragma once

#include <mutex>
#include <vector>
#include <unordered_map>

#include "formula/cell_proxy.hpp"
#include "helpers.hpp"

class CellsMap;
/*
  class CellsBlock
  
  Represent 'Size' rows of spreadsheet.
  HACK: offset just for DimI. For supporting cells like 'XYZ820' add offset for DimA
*/
template<uint32_t DimI, uint32_t DimA = 'Z' - 'A'>
class CellsBlock {
public:
  explicit CellsBlock(uint32_t i_index, CellsMap* _map) noexcept;
  CellsBlock(const CellsBlock&) = delete;
  ~CellsBlock() noexcept;

  CellsBlock& operator =(const CellsBlock&) = delete;

  CellProxy* Get(std::string_view name) noexcept;
  const CellProxy* Get(std::string_view name) const noexcept;

  void Visit(std::function<void(const CellProxy*)> fn) const;

  uint32_t IOffset() const noexcept;
  uint32_t AOffset() const noexcept;

  const uint32_t i_index = 0;
  CellsMap* const map;

private:
  size_t Index(uint32_t i, uint32_t a_index) const noexcept;
  size_t LocalIndex(uint32_t i, uint32_t a_index) const noexcept;

  std::array<CellProxy*, DimI * DimA> cells_;
};

template<uint32_t DimI, uint32_t DimA>
CellsBlock<DimI, DimA>::CellsBlock(uint32_t i, CellsMap* _map) noexcept
  : i_index(i)
  , map(_map) {
  for (int y = 0; y < DimI; y++)
    for (int x = 0; x < DimA; x++)
      cells_[LocalIndex(y, x)] = new CellProxy(this, Reference(y + i_index, x));
}

template<uint32_t DimI, uint32_t DimA>
CellsBlock<DimI, DimA>::~CellsBlock() noexcept { 
  for (auto& x: cells_)
    delete x;
}

template<uint32_t DimI, uint32_t DimA>
CellProxy* CellsBlock<DimI, DimA>::Get(std::string_view name) noexcept {
  auto a = IndexAlphabet(name);
  auto i = IndexInt(name);
  if (i < IOffset()) return nullptr;
  if (i >= IOffset() + DimI) return nullptr;

  return cells_[Index(i, a)];
}

template<uint32_t DimI, uint32_t DimA>
const CellProxy* CellsBlock<DimI, DimA>::Get(std::string_view name) const noexcept {
  auto a = IndexAlphabet(name);
  auto i = IndexInt(name);
  if (i < IOffset()) return nullptr;
  if (i >= IOffset() + DimI) return nullptr;

  return cells_[Index(i, a)].get();
}

template<uint32_t DimI, uint32_t DimA>
void CellsBlock<DimI, DimA>::Visit(std::function<void(const CellProxy*)> fn) const {
  for (auto* c: cells_) {
    if (c->IsEmpty()) continue;
    fn(c);
  }
}

template<uint32_t DimI, uint32_t DimA>
size_t CellsBlock<DimI, DimA>::Index(uint32_t i, uint32_t a) const noexcept {
  return (i - IOffset()) * DimA + a;
}
template<uint32_t DimI, uint32_t DimA>
size_t CellsBlock<DimI, DimA>::LocalIndex(uint32_t i, uint32_t a) const noexcept {
  return i * DimA + a;
}

template<uint32_t DimI, uint32_t DimA>
uint32_t CellsBlock<DimI, DimA>::IOffset() const noexcept {
  return i_index * DimI;
}
template<uint32_t DimI, uint32_t DimA>
uint32_t CellsBlock<DimI, DimA>::AOffset() const noexcept {
  return 0;
}

/*
  class CellsMap

  Отображает тестовое имя ячейки в реальную ячейку. Состоит из блоков 
  фикс. размера. При необходимости можно реализовать многоуровневую систему маппинга блоков,
  таким образом обеспечив черезвычайно большой объем доступных ячеек. Плюсы:
  ячейки легко искать, легко поддерживать "разряженную" таблицу (когда ячейки разбиты 
  на группы, а между группами много пустых ячеек). Минусы: каждый уровень маппинга требует
  накладных расходов, некоторой памяти в которой будет таблица указателей фикс. размера.
  В данный момент указатели на блоки read-only в том смысле, что создав блок однажды он
  будет существовать всегда, это позволяет сделать простой lock-free доступ к ячейкам.
*/
class CellsMap {
public:
  CellsMap() noexcept;
  ~CellsMap() noexcept;
  
  CellProxy* Find(const std::string_view name);
  const CellProxy* Find(const std::string_view name) const;
  CellProxy* Get(const std::string_view name);

  void Visit(std::function<void(const CellProxy*)> fn) const;

  Block* Get(uint32_t index);
private:
  Block* Create(uint32_t block_index);

  static const uint32_t NumOfBlocks = 1800;
  std::array<std::atomic<Block*>, NumOfBlocks> blocks_;
  
};