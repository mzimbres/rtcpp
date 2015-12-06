#include <vector>
#include <algorithm>
#include <limits>
#include <rtcpp/utility/make_rand_data.hpp>
#include <rtcpp/algorithm/comp_counting_sort.hpp>

int main()
{
  std::vector<int> data =
    rt::make_rand_data<int>( 10000
                           , 1
                           , std::numeric_limits<int>::max());

  rt::comp_counting_sort(std::begin(data), std::end(data));

  return std::is_sorted(std::begin(data), std::end(data)) ? 0 : 1;
}

