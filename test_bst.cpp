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

  bst<int> t1(size);
  bst<int> t2(size);

  int n = 0;
  while (n != size) {
    auto pair = t1.insert(dis(gen));
    if (pair.second)
      ++n;
  }

  //t1.copy(t2);

  for (auto iter = t1.rbegin(); iter != t1.rend(); ++iter)
    std::cout << *iter << " ";
  std::cout << std::endl;

  //std::copy(std::begin(t1), std::end(t1), std::ostream_iterator<int>(std::cout, " "));
  //std::cout << std::endl;
  std::copy(t1.rbegin(), t1.rend(), std::ostream_iterator<int>(std::cout, " "));
  std::cout << std::endl;
    
  return 0;
}

