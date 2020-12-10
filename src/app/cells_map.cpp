#include "cells_map.hpp"

CellsMap::CellsMap() noexcept {
  for (auto& x: blocks_)
    x.store(nullptr, std::memory_order_relaxed);
}

CellsMap::~CellsMap() noexcept {
  for (auto& x: blocks_) delete x;
}

CellProxy* CellsMap::Find(const std::string_view name) {
  auto i_index = IndexInt(name);

  auto block_index = i_index / BlockDimI;
  auto ptr = blocks_[block_index].load(std::memory_order_acquire);
  if (!ptr) return nullptr;
  return ptr->Get(name);
}

const CellProxy* CellsMap::Find(const std::string_view name) const {
  auto i_index = IndexInt(name);

  auto block_index = i_index / BlockDimI;
  auto ptr = blocks_[block_index].load(std::memory_order_acquire);
  if (!ptr) return nullptr;
  return ptr->Get(name);
}

CellProxy* CellsMap::Get(const std::string_view name) {
  auto i_index = IndexInt(name);

  auto block_index = i_index / BlockDimI;
  auto ptr = blocks_[block_index].load(std::memory_order_acquire);
  if (ptr) return ptr->Get(name);
  
  ptr = Create(block_index);
  assert(ptr != nullptr);
  return ptr->Get(name);
}

Block* CellsMap::Get(uint32_t index) {
  auto ptr = blocks_[index].load(std::memory_order_acquire);
  if (ptr) return ptr;
  
  return Create(index);
}

Block* CellsMap::Create(uint32_t block_index) {
  Block* new_block = new Block(block_index, this);
  Block* expected = nullptr;
  auto result = blocks_[block_index].compare_exchange_strong(expected, new_block, std::memory_order_acq_rel);
  if (!result) delete new_block;

  return blocks_[block_index].load();
}

void CellsMap::Visit(std::function<void(const CellProxy*)> fn) const {
  for (int i = 0; i < NumOfBlocks; i++) {
    auto ptr = blocks_[i].load(std::memory_order_acquire);
    if (!ptr) continue;
    
    ptr->Visit(fn);
  }
}
  