### Node allocators in C++

"Size management adds undue difficulties and inefficiencies to
any allocator design" (Alexandrescu)

**Abstract**: This is a non-breaking proposal to the C++ standard
that aims to reduce allocator complexity, support realtime
allocation and improve performance on node based containers. An
example implementation is provided together with benchmarks.

### Table of contents

* [Introduction](#Introduction)
* [Motivation and scope](#Motivation-and-scope)
* [Impact on the Standard](#Impact-on-the-standard)
* [References](#References)

### Introduction

The importance of linked data structures in computer science,
like trees and linked lists, cannot be over-emphasised, yet, in
the last couple of years it has become a common trend in C++ to
move away from such data structures due to their sub-optimal
memory access patterns.  In fact, many people today prefer to use
the flat alternatives and pay O(n) insertion time than O(1) at
the cost of memory fragmentation and unpredictable performance
loss.

On some domains, like **realtime applications**, **embedded
systems** or **systems that aim 24/7 availability**, the
unpredictability introduced by memory fragmentation is simply
unaffordable.

Even though the sub-optimal access patterns are inherent to
linked data structures, we think that a small non-breaking
addition to the C++ allocator model could strongly improve
performance and render them usable even **hard-real-time**
contexts.

The core of the idea is to make node based containers
support allocators that can serve only one size of memory blocks.
Allocating and deallocating blocks with the same size is as
simple as pushing and popping from a stack, which, beyond other
advantages, has constant time complexity (O(1)) complexity.

The allocate and deallocate member functions look like this in
these allocators.

```c++
pointer allocate()
{
  pointer p = m_stack.pop(); 
  if (!p)
    throw std::bad_alloc();
  return p; 
}

void deallocate(pointer p)
{
  m_stack.push(p);
}
```
### Motivation and scope

Some of the motivations behind this proposal are:

* Support the most natural and fastest allocation scheme for
  linked data structures. In libstd++ and libc++ for example, it
  is already possible to use this allocation technique, since n
  is always equal to 1 on calls of allocate(n).

* Support hard-realtime allocation for node-based containers.
  This is highly desirable to improve C++ use in embedded
  systems.

* Most allocators found in the literature are overly complicated
  as a result of having to handle blocks of different size. In
  node containers this is unnecessary since the requested memory
  have always the same size.

* State of the art allocators like boost::node_allocator already
  tries to optimize when allocate(n) happens to be called with
  n = 1. 

* The user should not *have to* provide allocators that
  handle different allocation sizes when containers do not make
  use of this feature. Users pay for a feature that is not used.

* Avoid wasted space behind allocations. It is pretty common that
  allocators allocate more memory than requested to store
  informations like the size of the allocated block.

* Keep nodes in as-compact-as-possible buffers, either on the
  stack or on the heap, improving cache locality, performance
  and making them specially useful for embedded programming.

* This proposal makes it easy to work with pre-allocated and
  pre-linked nodes.

A common question that are raised by users is: *Why dont't you
simply test whether n = 1 and pass allocation to your allocator?*
```c++
pointer allocate(std::size_t n)
{
  if (n == 1)
    return foo.allocate(); // Calls the node allocator.
  return bar.allocate(n); // Calls regular allocators.
}
```
There are a couple of reasons why this is an undesirable approach.

  1) The parameter n is not a compile time constant that
  can change in runtime, therefore, there is always the
  annoying possiblity of having n != 1. The node allocator
  becomes useless in this case.  We need a guarantee that the
  container will call the node allocator whenever it can.

  2) The possibility of having n != 1 means I have to handle
  allocation with different sizes, which it is exactly what I am
  trying to avoid.

  3) Containers in all major libraries always use n == 1, that
  means the condition ``if (n == 1)`` is an unnecessary overhead.

The influence of fragmentation on performance is a well known on
the C++ community and subject of many talks in conferences,
therefore I am not going to repeat results here for the sake of
readability. There is plenty of material on CPPCON for example.
The interested user can also refer the project
https://github.com/mzimbres/rtcpp for benchmarks of the
node_allocator.

### Impact on the Standard

This proposal does not require any breaking change. We require
that all node based containers favor the overload ``allocate()``
over ``allocate(size_type)`` for all node allocations inside the
container, whenever this member function is available. The same
reasoning applies to ``deallocate(pointer)`` over
``deallocate(pointer, size_type)``

In order to test whether the new overload is present, it is
necessary to add a new member to ```std::allocator_traits``` so
that container implementors have means to know which function has
to be used i.e. call allocate(n) or allocate().

```c++
using use_node_alloc = std::true_type;
```

The containers that are affected are: std::forward_list,
std::list, std::set, std::multiset, std::unordered_set,
std::unordered_multiset, std::map, std::multimap,
std::unordered_map, std::unordered_multimap

### References

* The Art of Computer Programming, Vol. 1, Donald Knuth.

