#include <vector>
#include <algorithm>
#include <limits>
#include <rtcpp/utility/make_rand_data.hpp>
#include <rtcpp/algorithm/find_intrusive.hpp>

int main()
{
  std::vector<int> data = rt::make_rand_data<int>(100 + 1, 0 , 100);

  const int n = 10;
  data[33] = n;
  data.back() = n;
  auto iter = rt::find_intrusive(std::begin(data), std::end(data), n);

  return iter != std::end(data) ? 0 : 1;
}

