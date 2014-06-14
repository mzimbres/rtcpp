#include <iostream>
#include <set>
#include <iterator>
#include <vector>
#include <stack>
#include <random>
#include <limits>

#include <boost/container/flat_set.hpp>

#include "to_number.hpp"

int main(int argc, char* argv[])
{
  if (argc != 2) {
    std::cerr << "Usage: ./a.out N" << std::endl;
    return 1;
  }
  
  const int size = to_number<int>(argv[1]);
  const int a = 1;
  const int b = std::numeric_limits<int>::max();
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(a, b);

  using namespace boost::container;

  flat_set<int> s;
  s.reserve(size);
  int n = 0;
  while (n != size) {
    auto pair = s.insert(dis(gen));
    if (pair.second)
      ++n;
  }

  //std::copy(std::begin(s), std::end(s), std::ostream_iterator<int>(std::cout, " "));
  //std::cout << std::endl;
    
  return 0;
}

