#include <iterator>
#include <random>
#include <functional>
#include <algorithm>
#include <iterator>

namespace rtcpp {

template <typename T>
std::vector<T> make_rand_data(std::size_t size, T first, T last, int type = 2)
{
  // Generates a uniform distribution of integers in the range [first, last]
  // with size size.
  // The following options for the argument type are available.
  // 1 - The returned vector can have repeated elements.
  // 2 - All elements apear only once. The vector can have size less the "size".

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(first, last);

  std::vector<T> data;
  std::generate_n( std::back_inserter(data)
                 , size
                 , [&](){return dis(gen);});
  if (type == 1)
    return data;

  std::sort(std::begin(data), std::end(data));
  //std::unique(std::begin(data), std::end(data));
  data.erase(std::unique(std::begin(data), std::end(data)), std::end(data));
  shuffle(std::begin(data), std::end(data), gen);
  return data;
}

}

