##RTCPP


  The idea of this project is to implement a realtime version of the C++
  Standard Template Library (STL).

  "Realtime" here means a guarantee that the program will execute a particular
  task in at most n steps (or processor cycles). To honor this demand we have
  to get rid of all sources of unpredictable behaviour. Some of them are:

  1. Heap fragmentation.
  2. Use of operator new.
  3. Use of exceptions.

  Before we talk about how I addressed these problems, let us see some examples.

##Realtime version of std::set.

The reader can find these examples in the directory "examples". For benchmarks,
see the program bench_bst.

```c++
  rt::bst<int> t1 = {5, 3, 7, 20, 1, 44, 22, 8}; // equivalent of std:set.

  std::copy( std::begin(t1)
           , std::end(t1)
           , std::ostream_iterator<int>(std::cout, " "));

  std::cout << std::endl;
```

The output of this program is "1 3 5 7 8 20 22 44". The class `rt::bs`t is the
equivalent of `std::set`. Since we did not provide an allocator it is using a
default constructed one, that provides storage for 1k bytes of storage. To
use a different storge size one needs an explicit allocator.

```c++
  std::array<char, 200> buffer = {{}}; // 200 bytes buffer.
  rt::allocator<int> alloc(buffer);
  rt::bst<int> t1(alloc);
  rt::bst<int> t2(alloc);

  t1 = {5, 3, 7, 20, 1};
  t2 = {44, 22, 8, 44, 33};

  std::copy( std::begin(t1)
           , std::end(t1)
           , std::ostream_iterator<int>(std::cout, " "));

  std::cout << std::endl;

  std::copy( std::begin(t2)
           , std::end(t2)
           , std::ostream_iterator<int>(std::cout, " "));

  std::cout << std::endl;
```
In the code snipet above, we see a 200 bytes buffer being shared
by the two sets t1 and t2. The program outputs

"1 3 5 7 20 "
"22 44      ",

the reason for which t2 does not contain the five numbers is that
the 200 bytes buffer is not enough storage for both t1 and t2.

The realtime version of std::set has the same interface as std::set itself but
requires more guarantees on its template parameters i.e. the compare function
and the allocator.

##Compilation

  To compile you will need a C++11 compiler and CMake. This is the command I
  use on cmake (maybe without all the optimization flags):

  cmake ../../rtcpp/ -DCMAKE_CXX_FLAGS="-Wall -Wextra -Werror -std=c++0x -Ofast -fipa-pta \
  -flto -funsafe-loop-optimizations -fno-exceptions -fno-rtti" -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_COMPILER=g++ -DBOOST_ROOT=${BOOST}

  I have experienced some strange problems with std::revers_iterators on gcc. For example:

  - Clang - Release : Tests succeed.
  - Clang - Debug   : Tests succeed.
  - GCC   - Release : Test 3 fails. Unclean compilation
  - GCC   - Debug   : Tests succeed. Unclean compilation

  Compiler versions are:

  GCC 4.8.2
  Clang 3.4

##Credits

Most of what is implemented here was taken from the book
  "The Art of Computer Programming Vol. 1 and 3".

