#include <vector>
#include <algorithm>
#include <limits>
#include <rtcpp/utility/make_rand_data.hpp>
#include <rtcpp/algorithm/find_fast.hpp>

int main()
{
  std::vector<int> data = rt::make_rand_data<int>(100 + 1, 0 , 100);

  data[33] = 10;
  auto iter = rt::find_fast(std::begin(data), std::end(data), 10);

  return iter != std::end(data) ? 0 : 1;
}

