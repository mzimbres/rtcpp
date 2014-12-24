#include <iostream>
#include <iterator>
#include <random>
#include <limits>
#include <functional>
#include <algorithm>
#include <set>
#include <list>

#include <container/bst_node.hpp>
#include <container/set.hpp>
#include <memory/allocator.hpp>
#include <utility/to_number.hpp>
#include <utility/make_rand_data.hpp>
#include <utility/timer.hpp>

template <typename C>
void print_bench(C& c, const std::vector<int>& vec)
{
  rt::timer t;
  c.insert(std::begin(vec), std::end(vec));
}

int main(int argc, char* argv[])
{
  std::size_t size = 600000;
  if (argc == 2)
    size = to_number<std::size_t>(argv[1]);

  const int a = 1;
  const int b = std::numeric_limits<int>::max();
  //const int b = size;

  std::cout << std::endl;
  std::cout <<
  "The benchmarks have the following layout: \n"
  "N (rt::set<std::alloc>) (rt::set<rt::alloc>) (std::set<std::alloc>) (std::set<rt::alloc>): \n"
  << std::endl;

  std::vector<int> data = rt::make_rand_data(size, a, b);

  std::cout << data.size() << " ";
  {
    rt::set<int> s;
    print_bench(s, data);
  }
  {
    std::vector<char> buffer((data.size() + 2) * sizeof (rt::set<int>::node_type), 0);
    rt::allocator<int> alloc(buffer);
    rt::set<int, std::less<int>, rt::allocator<int>> s(alloc); // Uses a vector as buffer.
    print_bench(s, data);
  }
  {
    std::set<int> s;
    print_bench(s, data);
  }
  {
    std::vector<char> buffer((data.size() + 2) * (2 * sizeof (rt::set<int>::node_type)), 0);
    rt::allocator<int> alloc(buffer);
    std::set<int, std::less<int>, rt::allocator<int>> s(std::less<int>(), alloc);
    print_bench(s, data);
  }
  std::cout << std::endl;

  return 0;
}

