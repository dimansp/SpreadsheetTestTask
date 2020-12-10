#pragma once

#include <mutex>
#include <queue>
#include <thread>
#include <condition_variable>

class ThreadPool {
public:
  ThreadPool();
  ~ThreadPool();

  bool Enqueue(std::function<void()>&& fn);

private:
  mutable std::mutex mutex_;
  std::vector<std::thread> workers_;
  std::queue<std::function<void()>> tasks_;
  std::condition_variable signal_;
  bool stop_ = false;
};
