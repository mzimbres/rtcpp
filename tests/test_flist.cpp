#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <functional>

#include <container/flist.hpp>

int main()
{
  using namespace rt;

  const std::size_t size = 40;
  flist<int> l;

  // Random number range.
  const int a = 1;
  const int b = 4;

  std::random_device rd;
  std::mt19937 gen(rd());
  std::generate_n(std::back_inserter(l), size, std::bind(std::uniform_int_distribution<>(a, b), gen));

  std::copy(std::begin(l), std::end(l), std::ostream_iterator<int>(std::cout, " "));
  std::cout << std::endl;
  l.remove_if(1);
  l.remove_if(2);
  std::copy(std::begin(l), std::end(l), std::ostream_iterator<int>(std::cout, " "));
  std::cout << std::endl;
  l.remove_if(4);
  std::copy(std::begin(l), std::end(l), std::ostream_iterator<int>(std::cout, " "));
  std::cout << std::endl;
  return 0;
}

