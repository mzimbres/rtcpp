## Allocators for node-based containers in C++

### Table of contents

* [Introduction](##introduction)
* [Motivation and scope](##motivation-and-scope)
* [Impact on the Standard](##impact-on-the-standard)
* [Sample implementation](##Sample-implementation)
* [Benchmarks](##Benchmarks)
* [Building the project](##Building-the-project)
* [References](##references)

### Introduction

The importance of linked data structures in computer
science, like trees and linked lists, cannot be
over-emphasised, yet, in the last couple of years it has
become a common trend in C++ to move away from such data
structures. The motivation for this move is not due to the
memory overhead of storing links in each node, but on the
fact that the memory access patterns associated with them,
are not optimal. As matter of fact, many people today prefer
too use the flat alternatives and pay linear insertion time
complexities than constant time complexity at the cost of
memory fragmentation and unpredictable performance loss.

On some domains, like realtime applications or systems that
aim 24/7 the unpredictability introduced by memory
fragmentation is simply unaffordable.

Even though the sub-optimal access patterns are inherent to
linked data structures, we think that a small non-breaking
addition on the C++ standard could strongly improve
performance and render c++ node-based containers usable
even hard-real-time contexts.

The core of the idea is to make node-based containers
support allocators that use pre-allocated and stack-like
pre-linked nodes. When an element is inserted in the
container the allocator pops one node from the stack,
when an element is removed the allocator pushes it back
into the stack.  Pushing and popping
from a stack are cheap operations that do not depend how
fragmented the memory is.

The allocate and deallocate member functions look like this
in node-based containers.

```c++
pointer allocate()
{
  char* p = m_stack.pop(); 
  if (!p)
    throw std::bad_alloc();
  return p; 
}
void deallocate(pointer p)
{
  m_stack.push(p);
}
```

As the reader may have noticed, these functions differ from
their standard definitions by the fact that they do not have
an argument to inform the size to be allocated or
deallocated. It is not possible for the allocate member
function to allocate more than one consecutive node.

### Motivation

* We can achieve platform independent node allocation and do not
have to rely on unstandardized allocation algorithms and system calls
like when using malloc.

* It is rather common to know in advance how many elements we want to
insert in the container or have at least a reasonable upper bound of
this value. That means we are paying for a flexibility we do not need,
when using the std::allocator.

* Keeping all nodes in sequential memory addresses improves
data locality and reduces fragmentation.

* Most node-based container implementations seems to already
support this kind of allocation.

### Impact on the Standard

This proposal does not require any breaking change.

### Sample implementation

Please see the rtcpp project on github.

```c++
  std::array<char, 2000> buffer = {{}};
  rt::allocator<int> alloc(buffer);

  std::list<int, rt::allocator<int>> t1(alloc);
  t1 = {5, 3, 7, 20, 1, 44, 22, 8};

  print(t1);
```

### Benchmarks

The links below shows the time taken to fill a `std::set`
and my own implementation of it `rt::set. Each one is tested
with five allocators:

  1. `std::allocator`.
  2. `rt::allocator`. (The real-time allocator.)
  3. `__gnu_cxx::__pool_alloc`.
  4. `__gnu_cxx::bitmap_alloc`.
  5. `__gnu_cxx::__mt_alloc`.
  6. `boost::constainer::node_allocator<int, 100000, 1>`.
  7. `boost::constainer::node_allocator<int, 100000, 2>`.

The benchmarks are performed on a scenario with a fragmented
heap, where I dynamically allocate many `char`'s on the heap
and leave some holes for the nodes that will be allocated by
the container. 

![std::set insertion time](fig/std_set_insertion.png),
![rt::set insertion time](fig/rt_set_insertion.png),

### Building the project

  To compile you will need a C++11 compiler and CMake. This
  is the command I use on cmake (maybe without all the
  optimization flags):

  cmake ../../rtcpp/ \
  -DCMAKE_CXX_FLAGS="-Wall -Wextra -Werror -std=c++0x" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_COMPILER=g++ -DBOOST_ROOT=${BOOST}

  I have tested the code with the following compilers.

  GCC 4.7.3
  GCC 4.8.2
  GCC 5.0
  Clang 3.4

### References

* [Knuth](The Art of Computer Programming)

