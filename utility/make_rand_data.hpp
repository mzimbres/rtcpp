#include <iterator>
#include <random>
#include <functional>
#include <algorithm>
#include <iterator>

namespace rtcpp {

template <typename T>
std::vector<T> make_rand_data(std::size_t size, T first, T last)
{
  // Generates a vector of random numbers in the range [first, last]
  // The generated vector has no repeated elements.

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(first, last);
  std::vector<T> data;
  std::generate_n( std::back_inserter(data)
                 , size
                 , [&](){return dis(gen);});
  std::sort(std::begin(data), std::end(data));
  //std::unique(std::begin(data), std::end(data));
  data.erase(std::unique(std::begin(data), std::end(data)), std::end(data));
  shuffle(std::begin(data), std::end(data), gen);
  return data;
}

}

