#include "cell_changer.hpp"
#include "formula/cell_proxy.hpp"

#include <assert.h>

#include "timer.hpp"
#include <iostream>

const size_t kMaxThreads = std::thread::hardware_concurrency();
const size_t kThreadThreshold = 100;

CellChanger::CellChanger(const std::vector<CellProxy*>& cells)
  : cells_(cells) {
  CalculateAll(cells_, GetNumOfThreads(cells_), 1);
}

void CellChanger::Change(CellProxy *start_cell) {
  auto version = 0;
  std::vector<CellProxy*> cells;
  {
    std::lock_guard<std::mutex> lock(m_);
    version = ++operation_counter;

    // TODO: remove empty (c->IsEmpty()) cells, but update it first
    auto it = std::find(cells_.begin(), cells_.end(), start_cell);
    if (it == cells_.end()) cells_.push_back(start_cell);
    
    cells = cells_;
  }

  auto num_of_threads = GetNumOfThreads(cells);
  InvalidateAll(cells, num_of_threads, version);
  CalculateAll(cells, num_of_threads, version);
}

void CellChanger::CalculateAll(const std::vector<CellProxy*>& cells, int num_of_threads, uint32_t version) {
  auto calc_function = [&cells, version](int offset, int threads) {
    for (auto i = offset; i < cells.size(); i += threads)
      cells[i]->CalculateForward(version);
  };

  if (num_of_threads == 1) {
    calc_function(0, num_of_threads);
    return;
  }
  
  std::vector<std::thread> threads;
  threads.reserve(num_of_threads);
  for (auto i = 0; i < num_of_threads; i++)
    threads.emplace_back(calc_function, i, num_of_threads);
  
  for (auto i = 0; i < num_of_threads; i++)
    threads[i].join();
}

void CellChanger::InvalidateAll(const std::vector<CellProxy*>& cells, int num_of_threads, uint32_t version) {
  auto calc_function = [&cells, version](int offset, int threads) {
    for (auto i = offset; i < cells.size(); i += threads)
      cells[i]->Invalidate(version);
  };

  if (num_of_threads == 1) {
    calc_function(0, num_of_threads);
    return;
  }
  
  std::vector<std::thread> threads;
  threads.reserve(num_of_threads);
  for (auto i = 0; i < num_of_threads; i++)
    threads.emplace_back(calc_function, i, num_of_threads);
  
  for (auto i = 0; i < num_of_threads; i++)
    threads[i].join();
}

uint32_t CellChanger::GetNumOfThreads(const std::vector<CellProxy*>& cells) const noexcept {
  return (int)std::min(kMaxThreads, cells.size() / kThreadThreshold + 1);
}