#pragma once

#include <sstream>

namespace rt
{

template <typename T>
T to_number(const char* str)
{
  T tmp;
  std::istringstream ss(str);
  ss >> tmp;
  return tmp;
}

}

