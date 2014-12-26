##RTCPP

  The idea of this project is to make real-time use of STL containers by means
  of *real-time allocators*.  "Real-time" here means a guarantee that the
  program will execute a particular task in at most n steps (or processor
  cycles).
  
  When the standard places a guarantee that insertion on an std::list has
  constant time complexity, it means operations inside the list and does not
  account the complexity of each node allocation, which may be constant or not
  depending on the allocation algorithm itself.

  This unpredictability is very undesirable in many applications like 24/7 and
  real-time systems.

  To make a real-time guarantee on a given operation, we have to get rid of all
  sources of unpredictable behaviour, some of them are.
  
  1. Dynamic allocations of the heap.
  2. Heap fragmentation.
  3. Use of exceptions.

  It is debatable whether use of exceptions is really important or not so I
  will not focus on it.  Before we talk about how I addressed these problems,
  let us see some examples and benchmarks.

## Benchmarks

In the link below show the time taken to fill a `std::set` and my own
implementation of it `rt::set`. Each one is tested with `std::allocator`
and my real-time allocator `rt::allocator`.

![Insertion time](fig/set_insertion.pdf).

In this graph the reader can see almost 50% performance improvement when `std::set` and `rt::set`
are used with a real-time allocator. More importantly, this improvement is
independent of the current heap state as I will show in next graph. (To come).

## Using rt::allocator

Let us see an example of how to use the real-time allocator (these examples can
be found in the examples directory). 

```c++
  typedef std::set<int, std::less<int>, rt::allocator<int>> rt_set_type;

  std::array<char, 1000> buffer = {{}}; // 1000 bytes buffer.
  rt::allocator<int> alloc(buffer);
  rt_set_type t1(alloc);
  rt_set_type t2(alloc);

  t1 = {5, 3, 7, 20, 1};
  t2 = {44, 22, 8, 44, 33};

  // Print t1 and t2 ...
```
In the code snippet above, we see a 1000 bytes buffer being shared among t1 and
t2.  All memory allocations happen inside the buffer and no call to operator
new is made. It makes the most efficient use of memory as all nodes are
allocated sequentially, that means you can easily fit them all on the cache.

## Rationale

In this section I will try to give the user an overview of the design
of the real-time allocator `rt::allocator`.

The first important thing is: **it is designed for node-based containers**
and should not be used with other containers.

When the user creates an allocator instance like `rt::allocator<int> alloc(buffer)`
the object `alloc` will stores a pointer to the buffer and its size.

##Compilation

  To compile you will need a C++11 compiler and CMake. This is the command I
  use on cmake (maybe without all the optimization flags):

  cmake ../../rtcpp/ -DCMAKE_CXX_FLAGS="-Wall -Wextra -Werror -std=c++0x -Ofast -fipa-pta \
  -flto -funsafe-loop-optimizations -fno-exceptions -fno-rtti" -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_COMPILER=g++ -DBOOST_ROOT=${BOOST}

  I have tested the code with the following compilers.

  GCC 4.7.3
  GCC 4.8.2
  GCC 5.0
  Clang 3.4

##Credits

Most of what is implemented here was taken from the book
  "The Art of Computer Programming Vol. 1 and 3".

