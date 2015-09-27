#include <array>
#include <set>

#include <rtcpp/utility/print.hpp>
#include <rtcpp/container/set.hpp>
#include <rtcpp/memory/node_allocator.hpp>

int main()
{
  typedef std::set<int, std::less<int>, rt::node_allocator<int>> rt_set_type;

  std::array<char, 300> buffer = {{}};
  rt::node_allocator<int> alloc(buffer);

  rt_set_type t1(std::less<int>(), alloc);
  rt_set_type t2(std::less<int>(), alloc);

  try {
    t1 = {5, 3, 7, 20, 1};
    t2 = {44, 22, 8, 44, 33};
  } catch (const std::bad_alloc& e) {
    std::cout << "There is not enough memory." << std::endl;
  }

  print(t1);
  print(t2);
}

