#include <list>
#include <iostream>
#include <algorithm>

#include <rtcpp/utility/to_number.hpp>
#include <rtcpp/utility/make_rand_data.hpp>

#include "heap_frag.hpp"
#include "print_list_bench.hpp"

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
    "(0) (1): \n"
    "Where: \n"
    "(0)  Number of elements.\n"
    "(1)  std::list<std::alloc>\n"
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
    make_rand_data<int>( N + (K - 1) * S
                       , 1
                       , std::numeric_limits<int>::max());

  std::vector<char*> pointers;
  if (frag) 
    pointers = heap_frag_list(B, data); // Fragments the heap.

  std::cout << data.size() << " " << frag << " " << B << std::endl;
  for (std::size_t i = 0; i < K; ++i) {
    const std::size_t n = N + i * S;
    std::cout << n << " ";
    print_list_bench<std::list<int>>( std::list<int>()
                                    , std::begin(data)
                                    , n);
    std::cout << std::endl;
  }
  std::for_each( std::begin(pointers)
               , std::end(pointers)
               , [](char* p){ delete p;});
  std::cout << std::endl;
  return 0;
}

