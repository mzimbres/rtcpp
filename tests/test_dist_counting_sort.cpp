#include <vector>
#include <algorithm>
#include <limits>
#include <rtcpp/utility/make_rand_data.hpp>
#include <rtcpp/algorithm/dist_counting_sort.hpp>

int main()
{
  const std::size_t N = 2000000;
  const std::size_t A = 0;
  const std::size_t B = 200;
  std::vector<int> data = rt::make_rand_data<int>(N ,A ,B);

  dist_counting_sort(std::begin(data), N, A, B);

  return std::is_sorted(std::begin(data), std::end(data)) ? 0 : 1;
}

