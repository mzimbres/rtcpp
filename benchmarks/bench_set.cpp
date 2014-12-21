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
  {
    std::cout << "Insertion: : ";
    rt::timer t;
    c.insert(std::begin(vec), std::end(vec));
  }
  {
    std::cout << "Lookup:    : ";
    rt::timer t;
    c.insert(std::begin(vec), std::end(vec));
  }
  {
    std::cout << "Deletion:  : ";
    rt::timer t;
    c.clear();
  }
}

int main(int argc, char* argv[])
{
  std::size_t size = 600000;
  if (argc == 2)
    size = to_number<std::size_t>(argv[1]);

  const int a = 1;
  const int b = std::numeric_limits<int>::max();
  //const int b = size;

  std::vector<int> data = rt::make_rand_data(size, a, b);

  std::cout << "N = " << data.size() << std::endl;
  // Benchmarks.
  {
    std::cout << "rt::set<std::alloc>: " << std::endl;
    rt::set<int> s;
    print_bench(s, data);
  }
  {
    std::cout << "rt::set<rt::alloc>: " << std::endl;
    std::vector<char> buffer((data.size() + 2) * sizeof (rt::set<int>::node_type), 0);
    rt::allocator<int> alloc(buffer);
    rt::set<int, std::less<int>, rt::allocator<int>> s(alloc); // Uses a vector as buffer.
    print_bench(s, data);
  }
  {
    std::cout << "std::set<std::alloc>: " << std::endl;
    std::set<int> s;
    print_bench(s, data);
  }
  {
    std::cout << "std::set<rt::alloc>: " << std::endl;
    std::vector<char> buffer((data.size() + 2) * (2 * sizeof (rt::set<int>::node_type)), 0);
    rt::allocator<int> alloc(buffer);
    std::set<int, std::less<int>, rt::allocator<int>> s(std::less<int>(), alloc);
    print_bench(s, data);
  }
  std::cout << std::endl;

  return 0;
}

