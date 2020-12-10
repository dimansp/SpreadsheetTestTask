#pragma once

#include <iostream>

struct ILogger {
  virtual ~ILogger() {}

  virtual void Print(std::string_view str) = 0;
  virtual void FlushLine() = 0;
};

class StdOutLogger: public ILogger {
public:
  void Print(std::string_view str) final {
    std::cout << str;
  }
  void FlushLine() final {
    std::cout << std::endl;
  }
};

class NullLogger: public ILogger {
public:
  void Print(std::string_view str) final {}
  void FlushLine() final {}
};

class LogStream {
public:
  LogStream(ILogger* logger)
    : logger_(logger) {
    assert(logger);
  }

  ~LogStream() {
    logger_->FlushLine();
  }

  LogStream& operator <<(std::string_view str) {
    logger_->Print(str);
    return *this;
  }
  LogStream& operator <<(int v) {
    logger_->Print(std::to_string(v));
    return *this;
  }
  LogStream& operator <<(double v) {
    logger_->Print(std::to_string(v));
    return *this;
  }
  LogStream& operator <<(float v) {
    logger_->Print(std::to_string(v));
    return *this;
  }

private:
  ILogger* logger_;
};

#define LOG(logger) LogStream(logger)
