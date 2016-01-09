#include <rtcpp/memory/align.hpp>

using namespace rt;

int main()
{
  if (!is_power_of_two<2>::value)
    return 1;

  if (!is_power_of_two<4>::value)
    return 1;

  if (!is_power_of_two<8>::value)
    return 1;

  if (!is_power_of_two<16>::value)
    return 1;

  if (!is_power_of_two<32>::value)
    return 1;

  if (!is_power_of_two<64>::value)
    return 1;

  if (!is_aligned<8>(16))
    return 1;

  if (align_next<8>(13) != 16)
    return 1;

  if (align_next<4>(1) != 4)
    return 1;

  if (align_next<4>(27) != 28)
    return 1;

  if (align_next<8>(27) != 32)
    return 1;

  return 0;
}

