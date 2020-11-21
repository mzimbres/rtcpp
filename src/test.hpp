#pragma once

#include <stdexcept>

#define RT_TEST(name) \
  void name(const char* f = #name)

#define RT_CHECK(b) \
  if (!(b)) \
    throw std::runtime_error(f);
 
namespace rt
{

void print(std::vector<int> const& v)
{
  for (auto o: v)
    std::cout << o << " ";
  std::cout << std::endl;
}

} // rt

