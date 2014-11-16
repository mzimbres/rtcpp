#pragma once

#include <chrono>

namespace rtcpp {

class timer {
  private:
  std::chrono::time_point<std::chrono::system_clock> m_start;
  public:
  timer()
  : m_start(std::chrono::system_clock::now())
  {}
  ~timer()
  {
    std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - m_start;
    std::cout << elapsed_seconds.count() << "s" << std::endl;
  }
};

}

