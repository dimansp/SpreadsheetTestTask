#pragma once

#include <chrono>

using namespace std::chrono_literals;

class Timer {
public:
  Timer() {
    start_ = std::chrono::system_clock::now();
  }

  double DurationSec() {
    auto now = std::chrono::system_clock::now();
    auto diff = now - start_;
    return (double)std::chrono::duration_cast<std::chrono::milliseconds>(diff).count() / 1000;
  }

private:
  std::chrono::time_point<std::chrono::system_clock> start_;
};
