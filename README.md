RTCPP
============

  The idea of this project is to implement realtime data structures and algorithms
  in C++. Most of what is implemented here was taken from the book "The Art of Computer
  programming Vol. 1 and 3".

Node based
=============

  The disign for node based containers I came up with, can be though as
  something between Boost.Intrusive (but less intrusive) and STL containers. I
  also affers more guarantees than STL containers, for example, all member
  functions are noexcept. As an example, let us see the class bst, that is
  equivalent to std::set:

  ```
  typedef bst<int>::node_type node_type; // Container node type.

  std::vector<node_type> buffer1(10); // Buffer of nodes in a vector.
  std::list<node_type> buffer2(10); // Buffer of nodes in a list.
  std::array<node_type, 10> buffer3; // Buffer of nodes in an array.

  // Since nodes have links, we can use them to form a stack. So let us link
  // the stack.
  node_type* avail1 = link_stack(std::begin(buffer1), std::end(buffer1));
  node_type* avail2 = link_stack(std::begin(buffer2), std::end(buffer2));
  node_type* avail3 = link_stack(std::begin(buffer3), std::end(buffer3));

  // Now we need an allocator-like interface to the stack
  node_stack<node_type> stack1(avail1);
  node_stack<node_type> stack2(avail2);
  node_stack<node_type> stack3(avail3);

  // Binary search trees that allocates buffers from buffer1
  bst<int> a1(std::ref(stack1));
  bst<int> b1(std::ref(stack1));

  // Binary search trees that allocates buffers from buffer2
  bst<int> a2(std::ref(stack2));
  bst<int> b2(std::ref(stack2));

  // Binary search trees that allocates buffers from buffer3
  bst<int> a3(std::ref(stack3));
  bst<int> b3(std::ref(stack3));
  ```
  Now that I showed what the interface looks like. Let do some more deep
  consideratons.

  Node based containers were my main motivation to begin this project. The bad
  thing about standard containers in C++ is the way they handle memory. There are
  some things that make me regret using them:

  1) If you are using the default allocator you are probabily using malloc for
     each new item inserted in your container, malloc in turn makes use of
     system calls to make room for the new inserted item. Imagine yourself
     using new to allocate space for an int as would be the case for
     std::set<int> for example. That is a very bad use of memory. To avoid
     that, one has to use custom allocators, however this does no solve all the
     problems (Even in the new C++11 allocator model).

  2) Dynamic allocations on the heap on systems that demand 7 - 24 availability
     are dangerous as you can end up with a very fragmented heap affecting 
     program performace.

  3) Even though one can use memory pools with allocators, they are more suitable
     for situations where the container size must increase on demand and one
     cannot place an upper bound on the maximum container size. In my job I
     often can place an upper bound on the container size. That means I am
     paying for what I do not need.

  This is the problem that allocators cannot handle properly:

  a) The allocator model is not suitable for constant time allocation for
     node-based containers.  With "constant" I mean something that does no
     depend on the container size or on how fragmented the heap may be. I do
     not mean "amortized" constant time. This fact is due mainly to the fact
     that the node_type used by the container is not exposed to the programmer.
     If it were exposed, once could easily achieve contant time by using node
     links to build an avail stack. For example, assuming that node type has an
     llink, we could allocate a node in the following way

                    node_pointer q = avail;
                    if (avail)
                      avail = avail->llink;
                    return q;

     Deallocation of a node pointed by p is as simple as

                    if (!p) return;
                    p->llink = avail;
                    avail = p;

     We see that just a couple of instructions are used, which makes allocation
     and deallocation constant time. See the class node_stack for how I handle
     this.

Binary Searce Trees
===================

Let me list some important facts about my implementation of a Binary Search Tree
(the bst class)

1) All member functions are exception safe and noexcept.

2) It is realtime. The time taken to allocate a node is constant and
   independent of the heap state. But since the tree is unbalanced no
   logarithmic time can be guaranteed and that may be undesirable in realtime
   applications. (You will have to wait a bit for my implementation of a
   balanced tree).

3) It is very nice to compare its performance when using std::allocator.
   Yes it is generic enough to be usable with std::allocator, however it is 
   not 100% the same interface as in STL.

4) Its design is not the same as std::set by its interface is prety similar.
   The are only some missing functions currently.

To play with the benchmarks use the program bench_bst. Example usage:

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

