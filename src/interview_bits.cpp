#include <iostream>
#include <bitset>

int count_bits(int v)
{
  int c = 0;
  for (; v; ++c)
    v &= v - 1;

  return c;
}

auto op(unsigned o, unsigned m, unsigned s)
{
  return ((o >> s) & m) | (o & m) << s;
}

auto reverse(unsigned o)
{
  o = op(o, 0x55555555,  1);
  o = op(o, 0x33333333,  2);
  o = op(o, 0x0F0F0F0F,  4);
  o = op(o, 0x00FF00FF,  8);
  o = op(o, 0x0000FFFF, 16);

  return o;
}

auto add(unsigned a, unsigned b)
{
  while (b != 0) {
    auto sum = a ^ b; // Without carry.
    b = (a & b) << 1;
    a = sum;
  }

  return a;
}

int main()
{
  std::cout << count_bits(8) << std::endl;
  std::cout << count_bits(15) << std::endl;
  std::cout << count_bits(10) << std::endl;
  std::cout << count_bits(100) << std::endl;
  std::cout << count_bits(127) << std::endl;

  std::cout << std::bitset<4 * 8>(182927) << std::endl;
  std::cout << std::bitset<4 * 8>(reverse(182927)) << std::endl;

  int a = 23;
  int b = 22;
  std::cout << a << " + " << b << ": " << add(a, b) << std::endl;
}

