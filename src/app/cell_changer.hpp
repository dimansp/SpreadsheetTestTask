#pragma once

#include <mutex>
#include <vector>
#include <unordered_set>

#include "thread_pool.hpp"
#include "cells_map.hpp"

class CellProxy;

class CellChanger {
public:
  CellChanger(const std::vector<CellProxy*>& cells);

  void Change(CellProxy* start_cell);
  
private:
  void CalculateAll(const std::vector<CellProxy*>& cells, int num_of_threads, uint32_t version);
  void InvalidateAll(const std::vector<CellProxy*>& cells, int num_of_threads, uint32_t version);

  uint32_t GetNumOfThreads(const std::vector<CellProxy*>& cells) const noexcept;

  std::mutex m_;
  std::vector<CellProxy*> cells_;
  uint32_t operation_counter = 1;
};