## Real-time allocators.

### Table of contents

* [Introduction](#introduction)
* [Motivation and scope](#motivation-and-scope)
* [What prevents realtime node allocation in current c++?] (#What-prevents-realtime-node-allocation-in-current-c++?)
* [Impact on the Standard](#impact-on-the-standard)
* [Sample implementation](#Sample-implementation)
* [Benchmarks](#Benchmarks)
* [Building the project](#Building-the-project)
* [References](#references)

### Introduction

The importance of linked data structures like trees and linked lists
in computer science cannot be over-emphasised, however, it has become
a common trend in the last years to move away from such data
structures in C++. The motivation for this move is not due to the
memory overhead of storing links in each node, but on the fact that
they are not cache friendly. As matter of fact, many people today
prefer too use the flat alternatives and pay linear "insertion in the
middle" complexities than constant time complexity at the cost of
memory fragmentation and unpredictable performance loss.

On some domains, like realtime applications or systems that aim 24/7
the unpredictability introduced by memory fragmentation and is simply
not affordable.

Even though the sub-optimal access patterns are inherent to linked
data structures, in the c++ node-based containers it is worser than it
should due to the design of allocators. This proposal concerns itself
with a small non-breaking addition to the standard, that could
strongly improve performance and render c++ node-based containers
usable in hard-real-time contexts.

The idea is to work with a preallocated buffer of nodes and link them
as a stack.  When an element is inserted in the container we pop one
node from the stack, when an element is removed we push it back into
the stack.  Pushing and popping from a stack are simple
operations, only some pointers are changed, or in other words, only a
couple of instructions are used, regardless of how fragmented the
heap is.

### Motivation

* We can achieve platform independent node allocation and do not
have to rely on unstandardized allocation algorithms and system calls
like when using malloc.

* It is rather common to know in advance how many elements we want to
insert in the container or have at least a reasonable upper bound of
this value. That means we are paying for a flexibility we do not need,
when using the std::allocator.

* Keeping all nodes in sequential memory addresses improve
data locality and reduces fragmentation.

* Most node-based containers implementations seems to already
support this kind of allocation.

### What prevents realtime node allocation in current c++?

The idea is to make node-based containers support allocators whose
allocate function can return only one node at once.

```c++
std::allocator::allocate(n) // Supported today
std::allocator::allocate() // Desired
```

A realtime node-based allocator cannot return more
than one consecutive node at once.

### Impact on the Standard

This proposal does not require any breaking change.

### Sample implementation

Please see the example directory for examples of use.

```c++
  std::array<char, 2000> buffer = {{}};
  rt::allocator<int> alloc(buffer);

  std::list<int, rt::allocator<int>> t1(alloc);
  t1 = {5, 3, 7, 20, 1, 44, 22, 8};

  print(t1);
```

### Benchmarks

The links below shows the time taken to fill a `std::set` and my own
implementation of it `rt::set. Each one is tested with five allocators:

  1. `std::allocator`.
  2. `rt::allocator`. (The real-time allocator.)
  3. `__gnu_cxx::__pool_alloc`.
  4. `__gnu_cxx::bitmap_alloc`.
  5. `__gnu_cxx::__mt_alloc`.
  6. `boost::constainer::node_allocator<int, 100000, 1>`.
  7. `boost::constainer::node_allocator<int, 100000, 2>`.

The benchmarks are performed on a scenario with a fragmented heap,
where I dynamically allocate many `char`'s on the heap and leave
some holes for the nodes that will be allocated by the container. 

![std::set insertion time](fig/std_set_insertion.png),
![rt::set insertion time](fig/rt_set_insertion.png),

### Building the project

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

### References

* [Knuth](The Art of Computer Programming)

