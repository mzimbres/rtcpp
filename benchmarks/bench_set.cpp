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

template <typename C, typename Iter>
void print_bench(C& c, Iter iter, std::size_t n)
{
  rt::timer t;
  c.insert(iter, iter + n);
}

int main(int argc, char* argv[])
{
  if (argc != 4) {
    std::cout <<
    "\nUsage: $ ./bench_set N S K\n"
    "N: The start size.\n"
    "S: The step size.\n"
    "K: How many steps.\n"
    << std::endl;
    std::cout <<
    "The benchmarks have the following layout: \n"
    "N (rt::set<std::alloc>) (rt::set<rt::alloc>) (std::set<std::alloc>) (std::set<rt::alloc>): \n"
    << std::endl;

    return 0;
  }

  const std::size_t N = to_number<std::size_t>(argv[1]);
  const std::size_t S = to_number<std::size_t>(argv[2]);
  const std::size_t K = to_number<std::size_t>(argv[3]);

  if (S > N) {
    std::cout << "S must be less than N." << std::endl;
    return 1;
  }

  const int a = 1;
  const int b = std::numeric_limits<int>::max();

  const std::size_t size = N + K * S;
  std::vector<int> data = rt::make_rand_data(size, a, b);

  for (std::size_t i = 0; i < K; ++i) {
    const std::size_t ss = N + i * S;
    std::cout << ss << " ";
    {
      rt::set<int> s;
      print_bench(s, std::begin(data), ss);
    }
    {
      std::vector<char> buffer((ss + 2) * sizeof (rt::set<int>::node_type), 0);
      rt::allocator<int> alloc(buffer);
      rt::set<int, std::less<int>, rt::allocator<int>> s(alloc); // Uses a vector as buffer.
      print_bench(s, std::begin(data), ss);
    }
    {
      std::set<int> s;
      print_bench(s, std::begin(data), ss);
    }
    {
      std::vector<char> buffer((ss + 2) * (2 * sizeof (rt::set<int>::node_type)), 0);
      rt::allocator<int> alloc(buffer);
      std::set<int, std::less<int>, rt::allocator<int>> s(std::less<int>(), alloc);
      print_bench(s, std::begin(data), ss);
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;

  return 0;
}

