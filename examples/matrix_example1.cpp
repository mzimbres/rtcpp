
#include <rtcpp/container/matrix.hpp>

int main()
{
  using vec3 = rtcpp::matrix<double, 3, 1>;
  using mat3 = rtcpp::matrix<double, 3, 3>;

  const vec3 v = {2, 4, 6};
  const mat3 m = {1, 2, 3, 4, 5, 6, 7, 8, 9};

  const vec3 r = 2 * m * v + 3 * v + v / 2;

  std::cout << r;
  return 0;
}

