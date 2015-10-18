#include <set>
#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <functional>

#include <ext/mt_allocator.h>
#include <ext/pool_allocator.h>
#include <ext/bitmap_allocator.h>

#include <rtcpp/utility/to_number.hpp>
#include <rtcpp/memory/node_allocator.hpp>
#include <rtcpp/utility/make_rand_data.hpp>

#include <config.h>

#include "heap_frag.hpp"
#include "print_set_bench.hpp"

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
    "(1)  std::set<std::alloc>\n"
    "(2)  std::set<rt::alloc>\n"
    "(3)  std::set<__gnu_cxx::__pool_alloc>\n"
    "(4)  std::set<__gnu_cxx::bitmap_alloc>\n"
    "(5)  std::set<__mt_alloc>\n"
    << std::endl;

    return 0;
  }

  using namespace rt;

  const std::size_t N = to_number<std::size_t>(argv[1]);
  const std::size_t S = to_number<std::size_t>(argv[2]);
  const std::size_t K = to_number<std::size_t>(argv[3]);
  const std::size_t B = to_number<std::size_t>(argv[4]);
  const bool frag = !(argc == 6);

  const std::vector<int> data =
    rt::make_rand_data<int>( N + (K - 1) * S
                           , 1
                           , std::numeric_limits<int>::max());

  std::vector<char*> pointers;
  if (frag) // Fragments the heap.
    pointers = heap_frag<std::set<int>>(B, data);

  std::cout << "(1)" << std::endl;
  for (std::size_t i = 0; i < K; ++i) {
    const std::size_t n = N + i * S;
    std::cout << n << " ";
    print_set_bench( std::set<int, std::less<int>, std::allocator<int>>()
                   , std::begin(data), n); // (1)
    std::cout << std::endl;
  }
  std::cout << "(2)" << std::endl;
  for (std::size_t i = 0; i < K; ++i) {
    const std::size_t n = N + i * S;
    std::cout << n << " ";
    std::vector<char> buffer((n + 2) * 40, 0);
    rt::node_allocator<int> alloc(buffer);
    typedef std::set<int, std::less<int>, rt::node_allocator<int>> set_type2;
    set_type2 s(std::less<int>(), alloc); // Uses a vector as buffer.
    print_set_bench(s, std::begin(data), n);
    std::cout << std::endl;
  }
  std::cout << "(3)" << std::endl;
  for (std::size_t i = 0; i < K; ++i) {
    const std::size_t n = N + i * S;
    std::cout << n << " ";
    typedef std::set<int, std::less<int>, __gnu_cxx::__pool_alloc<int>> set_type3;
    set_type3 s;
    print_set_bench(s, std::begin(data), n);
    std::cout << std::endl;
  }
  std::cout << "(4)" << std::endl;
  for (std::size_t i = 0; i < K; ++i) {
    const std::size_t n = N + i * S;
    std::cout << n << " ";
    typedef std::set<int, std::less<int>, __gnu_cxx::bitmap_allocator<int>> set_type4;
    set_type4 s;
    print_set_bench(s, std::begin(data), n);
    std::cout << std::endl;
  }
  std::cout << "(5)" << std::endl;
  for (std::size_t i = 0; i < K; ++i) {
    const std::size_t n = N + i * S;
    std::cout << n << " ";
    typedef std::set<int, std::less<int>, __gnu_cxx::__mt_alloc<int>> set_type5;
    set_type5 s;
    print_set_bench(s, std::begin(data), n);
    std::cout << std::endl;
  }
  std::cout << std::endl;
  std::for_each( std::begin(pointers)
               , std::end(pointers)
               , [](char* p){ delete p;});
  return 0;
}

