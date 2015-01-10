#include <iostream>
#include <iterator>
#include <random>
#include <limits>
#include <functional>
#include <algorithm>
#include <set>
#include <list>
#include <vector>
#include <ext/pool_allocator.h>
#include <ext/bitmap_allocator.h>
#include <ext/mt_allocator.h>

#include <rtcpp/container/bst_node.hpp>
#include <rtcpp/container/set.hpp>
#include <rtcpp/memory/allocator.hpp>
#include <rtcpp/utility/to_number.hpp>
#include <rtcpp/utility/make_rand_data.hpp>
#include <rtcpp/utility/timer.hpp>

#include <config.h>

#ifdef Boost_FOUND
#include <boost/container/flat_set.hpp>
#include <boost/container/node_allocator.hpp>
#endif

#include "heap_frag.hpp"

template <typename C, typename Iter>
void print_bench(C& c, Iter begin, std::size_t n)
{
  {
    rt::timer t;
    c.insert(begin, begin + n);
    //for (std::size_t i = 0; i < n; ++i) {
    //  auto it = c.find(begin[n - i - 1]);
    //  if (it != c.end())
    //    c.erase(it);
    //}
  }
  //if (!c.empty())
  //  throw std::runtime_error("");
}

namespace rt {

template <template <typename, typename, typename> class Set, typename Iter>
void bench_allocators(Iter begin, std::size_t n)
{
  // The different sets we will benchmark.
  typedef Set<int, std::less<int>, std::allocator<int>> set_type1;
  typedef Set<int, std::less<int>, rt::allocator<int>> set_type2; // Uses a vector as buffer.
  typedef Set<int, std::less<int>, __gnu_cxx::__pool_alloc<int>> set_type3;
  typedef Set<int, std::less<int>, __gnu_cxx::bitmap_allocator<int>> set_type4;
  typedef Set<int, std::less<int>, __gnu_cxx::__mt_alloc<int>> set_type5;
#ifdef Boost_FOUND
  typedef Set<int, std::less<int>, boost::container::node_allocator<int, 1000, 1>> set_type6;
  typedef Set<int, std::less<int>, boost::container::node_allocator<int, 1000, 2>> set_type7;
#endif
  std::cout << n << " ";
  { // (1)
    set_type1 s;
    print_bench(s, begin, n);
  }
  { // (2)
    std::vector<char> buffer((n + 2) * 40, 0);
    rt::allocator<int> alloc(buffer);
    set_type2 s(std::less<int>(), alloc); // Uses a vector as buffer.
    print_bench(s, begin, n);
  }
  { // (3)
    set_type3 s;
    print_bench(s, begin, n);
  }
  { // (4)
    set_type4 s;
    print_bench(s, begin, n);
  }
  { // (5)
    set_type5 s;
    print_bench(s, begin, n);
  }
#ifdef Boost_FOUND
  { // (6)
    set_type6 s;
    print_bench(s, begin, n);
  }
  { // (7)
    set_type7 s;
    print_bench(s, begin, n);
  }
#endif
}

}

using namespace rt;

int main(int argc, char* argv[])
{
  if ((argc != 5) && (argc != 6)) {
    std::cout <<
    "\nUsage: $ ./bench_set N S K B F\n"
    "N: The start size.\n"
    "S: The step size.\n"
    "K: How many steps.\n"
    "B: Chars between.\n"
    "F: Optional. If provided will not fragment the heap before benchmarks.\n"
    << std::endl;
    std::cout <<
    "The program output has the following layout: \n"
    "(0) (1) (2) (3) (4) (5): \n"
    "Where: \n"
    "(0)  Number of elements.\n"
    "(1)  Set<std::alloc>\n"
    "(2)  Set<rt::alloc>\n"
    "(3)  Set<__gnu_cxx::__pool_alloc>\n"
    "(4)  Set<__gnu_cxx::bitmap_alloc>\n"
    "(5)  Set<__mt_alloc>\n"
    << std::endl;

    return 0;
  }

  const std::size_t N = to_number<std::size_t>(argv[1]);
  const std::size_t S = to_number<std::size_t>(argv[2]);
  const std::size_t K = to_number<std::size_t>(argv[3]);
  const std::size_t B = to_number<std::size_t>(argv[4]);
  bool frag = true;
  if (argc == 6)
    frag = false;

  if (S > N) {
    std::cout << "S must be less than N." << std::endl;
    return 1;
  }

  const int a = 1;
  const int b = std::numeric_limits<int>::max();

  const std::size_t size = N + K * S;
  const std::vector<int> data = rt::make_rand_data<int>(size, a, b);

  {
    std::cout << "---- rt::set ----" << std::endl;
    std::vector<char*> pointers;
    if (frag) 
      pointers = heap_frag<rt::set<int>>(B, data); // Fragments the heap.

    for (std::size_t i = 0; i < K; ++i) {
      bench_allocators<rt::set>(std::begin(data), N + i * S);
      std::cout << std::endl;
    }

    for (auto iter = std::begin(pointers); iter != std::end(pointers); ++iter)
      delete *iter;
  }
  {
    std::cout << "---- std::set ----" << std::endl;
    std::vector<char*> pointers;
    if (frag) 
      pointers = heap_frag<std::set<int>>(B, data); // Fragments the heap.

    for (std::size_t i = 0; i < K; ++i) {
      const std::size_t ss = N + i * S;
      bench_allocators<std::set>(std::begin(data), ss);
      std::cout << std::endl;
    }
    for (auto iter = std::begin(pointers); iter != std::end(pointers); ++iter)
      delete *iter;
  }
#ifdef Boost_FOUND
  {
    std::cout << "---- boost::flat_set ----" << std::endl;
    for (std::size_t i = 0; i < K; ++i) {
      const std::size_t ss = N + i * S;
      boost::container::flat_set<int> s;
      s.reserve(ss);
      std::cout << ss << " ";
      print_bench(s, std::begin(data), ss);
      std::cout << std::endl;
    }
  }
#endif
  std::cout << std::endl;
  return 0;
}

