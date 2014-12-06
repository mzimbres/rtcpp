#include <memory/align.hpp>

using namespace rtcpp;

int main()
{
  const std::size_t a = 8;
  if (!is_power_of_two(a))
    return 1;
  const std::size_t b = 16;
  if (!is_aligned(b, a))
    return 1;
  const std::size_t c = 13;
  if (next_aligned(c, a) != 16)
    return 1;
  return 0;
}

