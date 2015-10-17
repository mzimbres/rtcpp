#pragma once

#include <rtcpp/utility/timer.hpp>

namespace rt {

template <typename C, typename Iter>
void print_list_bench(C c, Iter begin, std::size_t n)
{
  const std::size_t s = n / 2;
  c.insert(std::end(c), begin, begin + s);
  {
    rt::timer t;
    for (std::size_t i = 0; i <= s; ++i) {
      auto iter = std::find( std::begin(c)
                           , std::end(c)
                           , begin[i]);
      if (iter != std::end(c))
        c.erase(iter);
      c.push_front(begin[n - i - 1]);
    }
    for (std::size_t i = 0; i <= s; ++i) {
      auto iter = std::find( std::begin(c)
                           , std::end(c)
                           , begin[n - i - 1]);
      if (iter != std::end(c))
        c.erase(iter);
      c.push_front(begin[i]);
    }
  }
}

}

