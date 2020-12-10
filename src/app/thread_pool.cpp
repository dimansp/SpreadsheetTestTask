#include "thread_pool.hpp"

#include <assert.h>

ThreadPool::ThreadPool() {
  auto task_func = [this] () {
    while(true) {
      std::function<void()> task;
      {
        std::unique_lock<std::mutex> lock(mutex_);
        
        signal_.wait(lock, [this]{ return stop_ || !tasks_.empty(); });
        if (stop_) return;
        assert(!tasks_.empty());
        
        task = std::move(tasks_.front());
        tasks_.pop();
      }
      task();
    }
  };

  for (auto i = 0; i < std::thread::hardware_concurrency(); i++) {
    workers_.emplace_back(task_func);
  }
}

ThreadPool::~ThreadPool() {
  {
    std::unique_lock<std::mutex> lock(mutex_);
    stop_ = true;
  }
  signal_.notify_all();
  for(auto& w: workers_)
    w.join();
}

bool ThreadPool::Enqueue(std::function<void()>&& fn) {
  {
    std::unique_lock<std::mutex> lock(mutex_);
    assert(!stop_);
    
    tasks_.emplace(fn);
  }
  signal_.notify_one();
  return true;
}