#include <list>
#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <functional>

#include <rtcpp/algorithm/find_fast.hpp>
#include <rtcpp/utility/to_number.hpp>
#include <rtcpp/utility/make_rand_data.hpp>
#include <rtcpp/utility/timer.hpp>

int main(int argc, char* argv[])
{
  if (argc != 2) {
    std::cout <<
    "\nUsage: $ ./bench_find N\n"
    "N: The start size.\n"
    << std::endl;

    return 0;
  }

  const std::size_t n = rt::to_number<std::size_t>(argv[1]);

  std::vector<int> data =
    rt::make_rand_data<int>( n + 1
                           , std::numeric_limits<int>::min()
                           , std::numeric_limits<int>::max());

  std::cout << "rt::find_fast" << std::endl;
  {
    const auto a = std::begin(data);
    const auto b = std::end(data);
    rt::timer t;
    for (std::size_t i = 0; i < n; ++i) {
      auto iter = rt::find_fast(a, b, data[i]);
      if (iter == b) {
        std::cout << "Something wrong ..." << std::endl;
        return 1;
      }
    }
  }
  std::cout << std::endl;
  std::cout << "std::find" << std::endl;
  {
    const auto a = std::begin(data);
    auto b = std::end(data);
    --b;
    rt::timer t;
    for (std::size_t i = 0; i < n; ++i) {
      auto iter = std::find(a, b, data[i]);
      if (iter == b) {
        std::cout << "Something wrong ..." << std::endl;
        return 1;
      }
    }
  }

  std::cout << std::endl;
  return 0;
}

