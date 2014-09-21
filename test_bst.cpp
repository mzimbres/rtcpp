#include <iostream>
#include <iterator>
#include <vector>
#include <stack>
#include <random>
#include <limits>
#include <functional>

#include "bst.hpp"
#include "to_number.hpp"

int main(int argc, char* argv[])
{
  if (argc != 2) {
    std::cerr << "Usage: ./a.out N" << std::endl;
    return 1;
  }
  
  const int size = to_number<int>(argv[1]);
  const int a = 1;
  //const int b = std::numeric_limits<int>::max();
  const int b = size;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(a, b);

  bst<int> t(size);

  int n = 0;
  while (n != size) {
    auto pair = t.insert(dis(gen));
    if (pair.second)
      ++n;
  }

  std::copy(std::begin(t), std::end(t), std::ostream_iterator<int>(std::cout, " "));
  std::cout << std::endl;
  std::copy(t.rbegin(), t.rend(), std::ostream_iterator<int>(std::cout, " "));
  std::cout << std::endl;
    
  return 0;
}

