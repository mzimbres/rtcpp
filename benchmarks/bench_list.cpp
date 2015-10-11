#include <list>
#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <functional>

#include <ext/mt_allocator.h>
#include <ext/pool_allocator.h>
#include <ext/bitmap_allocator.h>

#include <rtcpp/container/set.hpp>
#include <rtcpp/utility/to_number.hpp>
#include <rtcpp/container/bst_node.hpp>
#include <rtcpp/memory/node_allocator.hpp>
#include <rtcpp/utility/make_rand_data.hpp>

#include <config.h>

#include "heap_frag.hpp"
#include "print_list_bench.hpp"

namespace rt {

template <typename Iter>
void bench_allocators(Iter begin, std::size_t n)
{
  typedef std::list<int, std::allocator<int>> list_type1;
  typedef std::list<int, rt::node_allocator<int>> list_type2; // Uses a vector as buffer.
  typedef std::list<int, __gnu_cxx::__pool_alloc<int>> list_type3;
  typedef std::list<int, __gnu_cxx::bitmap_allocator<int>> list_type4;
  typedef std::list<int, __gnu_cxx::__mt_alloc<int>> list_type5;
  std::cout << n << " ";
  { // (1)
    list_type1 s;
    print_list_bench(s, begin, n);
  }
  { // (2)
    std::vector<char> buffer((n + 2) * 40, 0);
    rt::node_allocator<int> alloc(buffer);
    list_type2 s(alloc); // Uses a vector as buffer.
    print_list_bench(s, begin, n);
  }
  { // (3)
    list_type3 s;
    print_list_bench(s, begin, n);
  }
  { // (4)
    list_type4 s;
    print_list_bench(s, begin, n);
  }
  { // (5)
    list_type5 s;
    print_list_bench(s, begin, n);
  }
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
    "(1)  std::list<std::alloc>\n"
    "(2)  std::list<rt::alloc>\n"
    "(3)  std::list<__gnu_cxx::__pool_alloc>\n"
    "(4)  std::list<__gnu_cxx::bitmap_alloc>\n"
    "(5)  std::list<__mt_alloc>\n"
    "(6)  std::list<boost::node_allocator<v1>>\n"
    "(7)  std::list<boost::node_allocator<v2>>\n"
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
    std::cout << "---- std::list ----" << std::endl;
    std::vector<char*> pointers;
    if (frag) 
      pointers = heap_frag_list(B, data); // Fragments the heap.

    for (std::size_t i = 0; i < K; ++i) {
      const std::size_t ss = N + i * S;
      bench_allocators(std::begin(data), ss);
      std::cout << std::endl;
    }
    for (auto iter = std::begin(pointers); iter != std::end(pointers); ++iter)
      delete *iter;
  }
  std::cout << std::endl;
  return 0;
}

