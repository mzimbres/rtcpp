##RTCPP

  The idea of this project is to make real-time use of
  STL containers by means of real-time allocators.

  "Real-time" here means a guarantee that the program will execute a particular
  task in at most n steps (or processor cycles). When the standard says that
  insertion on an std::list is constant time, it means operations inside the
  list and does not account for example, whether allocation of nodes are
  constant time or not. However, the performance of allocations can become very
  bad in situations where the heap is fragmented. This is an undesirable effect
  in many real-time systems.

  To make a real-time guarantee on a given operation, we have to get rid of all
  sources of unpredictable behaviour, some of them are.
  
  1. Dynamic allocations of the heap.
  2. Heap fragmentation.
  3. Use of exceptions.

  It is debatable whether use of exceptions is really important or not so I will not
  focus on it.  Before we talk about how I addressed these problems, let us see
  some examples and benchmarks.

## Benchmarks

In the link below show the time taken to fill a `std::set` and my own
implementation of it `rt::set`. Each one is tested with `std::allocator`
and my real-time allocator `rt::allocator`.

![Insertion time](fig/set_insertion.pdf),

To run the benchmarks use the program `bench_set`.

## Real-time allocator

Let us see an example of how to use the real-time allocator (these examples can
be found in the examples directory). 

```c++
  typedef std::set<int, std::less<int>, rt::allocator<int>> rt_set_type;

  std::array<char, 1000> buffer = {{}}; // 200 bytes buffer.
  rt::allocator<int> alloc(buffer);
  rt_set_type t1(alloc);
  rt_set_type t2(alloc);

  t1 = {5, 3, 7, 20, 1};
  t2 = {44, 22, 8, 44, 33};

  // Print t1 and t2 ...
```
In the code snippet above, we see a 300 bytes buffer being shared among t1 and
t2.  All memory allocations happen inside the buffer and no call to operator
new is made.

## Real-time allocators

The main difficult I have experienced when wrting the real-time allocator, is
the following:

Container writers are allowed to use Allocator::allocate(n) with n different
from 1. That prevents me from writing real-time allocation, where nodes are
pre-allocated and linked together to form a stack. That way allocating and
deallocating translates into popping and pushing from the stack. This is a way
of achieving truly constant time allocation as opposed to "amortized".  This
way, no matter how fragmented the heap may be or how long the system has been
running, allocation will last the same time. It has the additional benefit
that allocation logic is kept inside the allocator.

##Compilation

  To compile you will need a C++11 compiler and CMake. This is the command I
  use on cmake (maybe without all the optimization flags):

  cmake ../../rtcpp/ -DCMAKE_CXX_FLAGS="-Wall -Wextra -Werror -std=c++0x -Ofast -fipa-pta \
  -flto -funsafe-loop-optimizations -fno-exceptions -fno-rtti" -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_COMPILER=g++ -DBOOST_ROOT=${BOOST}

  I have tested the code with the following compilers.

  GCC 4.8.2
  GCC 5.0
  Clang 3.4

##Credits

Most of what is implemented here was taken from the book
  "The Art of Computer Programming Vol. 1 and 3".

