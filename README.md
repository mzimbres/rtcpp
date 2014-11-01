RTCPP
============

  The idea of this project is to implement real-time data structures and algorithms
  in C++. Most of what is implemented here was taken from the book "The Art of Computer
  programming Vol. 1 and 3".

Node based
=============

  The design for node based containers I came up with, can be though as
  something between Boost.Intrusive (but less intrusive) and STL containers
  and offers more guarantees than STL containers. Some of the things that are
  not properly addressed by STL node-based containers are:

  1) Dynamic allocations (with system calls) on every insertion.  To avoid
    that, one has to use custom allocators, however this does no solve all the
    problems.

  2) Heap fragmentation that impact performance, specially for small data types
     like POD (plain old data).

  3) More suitable when the container must grow on demand. If you do not need
     this feature, you are paying for what you do not need.

  4) Unsuitable for constant time allocations i.e. allocations that does not
     depend on the container size and on heap fragmentation.
  
  To understand how I address these problems, let us see a small example of
  the class bst, that offers almost the same interface as std::set.

  ```
  typedef bst<int>::node_type node_type; // Container node type.

  std::vector<node_type> buffer1(10); // Buffer of nodes in a vector.
  std::list<node_type> buffer2(10); // Buffer of nodes in a list.

  // Links the nodes.
  node_type* avail1 = link_stack(std::begin(buffer1), std::end(buffer1));
  node_type* avail2 = link_stack(std::begin(buffer2), std::end(buffer2));

  // Now we need an allocator-like interface to the stack
  node_stack<node_type> stack1(avail1);
  node_stack<node_type> stack2(avail2);

  // Binary search trees that allocates nodes from buffer1
  bst<int> a1(std::ref(stack1));
  bst<int> b1(std::ref(stack1));

  // Binary search trees that allocates nodes from buffer2
  bst<int> a2(std::ref(stack2));
  bst<int> b2(std::ref(stack2));
  ```
  As we see, one first needs the container node_type. Once we know it, we can
  declare the buffers to store the nodes. The only demand on the container is
  that it offers forward iterators. Usually the user won't want to store the buffers
  on a list, because of memory fragmentation.

  Once we have a buffers of nodes, we can link them together to form an
  avail stack, so that allocation and deallocation converts on pushing and
  popping from the avail stack. The algorithm to link the stack assumes that
  each node has at least one link to another node and that it is called llink.
  
Binary Search Trees
===================

Let me list some important facts about my implementation of a Binary Search Tree
(the bst class)

1) All member functions are exception safe and noexcept.

2) It is real-time. The time taken to allocate a node is constant and
   independent of the heap state. But since the tree is unbalanced no
   logarithmic time can be guaranteed and that may be undesirable in real-time
   applications. (You will have to wait a bit for my implementation of a
   balanced tree).

3) It is generic enough to be usable with std::allocator. However it is not
   100% the same interface as in STL.

4) Its design is not the same as std::set by its interface is pretty similar.
   The are only some missing functions currently.

To play with the benchmarks use the program bench_bst.

Compilation
=============

  To compile you will need a C++11 compiler, CMake and Boost. This is the
  command I use on cmake (maybe without all the optimization flags):

  cmake ../../rtcpp/ -DCMAKE_CXX_FLAGS="-Wall -Wextra -Werror -std=c++0x -Ofast -fipa-pta \
  -flto -funsafe-loop-optimizations" -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_COMPILER=g++ -DBOOST_ROOT=${BOOST}

  I have experienced some strange problems with std::revers_iterators on gcc. For example:

  - Clang - Release : Tests succeed.
  - Clang - Debug   : Tests succeed.
  - GCC   - Release : Test 3 fails. Unclean compilation
  - GCC   - Debug   : Tests succeed. Unclean compilation

  Compiler versions are:

  GCC 4.8.2
  Clang 3.4

