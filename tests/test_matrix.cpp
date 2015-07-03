#include <rtcpp/container/matrix.hpp>

using namespace rtcpp;

typedef matrix<double, 3, 1> vec3;
typedef matrix<double, 4, 1> vec4;
typedef matrix<double, 6, 1> vec6;
typedef matrix<double, 1, 1> mat1;
typedef matrix<double, 3, 3> mat3;
typedef matrix<double, 6, 6> mat6;


template <typename T>
matrix<T, 2, 2> gen_mat()
{
  matrix<T, 2, 2> mat = {1, 2, 3, 4};
  return mat;
}

bool test_div1()
{
  typedef matrix<double, 2, 2> mat_type;

  mat_type m1(1);
  const mat_type tmp = m1 / 2 + m1 / 2;
  const bool b1 = std::abs(tmp(0, 0) - 1) < 1e-15;
  const bool b2 = std::abs(tmp(0, 1) - 1) < 1e-15;
  const bool b3 = std::abs(tmp(1, 0) - 1) < 1e-15;
  const bool b4 = std::abs(tmp(1, 1) - 1) < 1e-15;
  return b1 && b2 && b3 && b4;
}

bool test_div2()
{
  typedef matrix<double, 2, 2> mat_type;

  mat_type m1(2);
  m1 /= 2;
  const bool b1 = std::abs(m1(0, 0) - 1) < 1e-15;
  const bool b2 = std::abs(m1(0, 1) - 1) < 1e-15;
  const bool b3 = std::abs(m1(1, 0) - 1) < 1e-15;
  const bool b4 = std::abs(m1(1, 1) - 1) < 1e-15;
  return b1 && b2 && b3 && b4;
}

bool test_mult1()
{
  typedef matrix<int, 2, 2> mat_type;

  mat_type m1(1);
  const mat_type m2 = 1024 * m1;
  const mat_type m3 = m1 * m1 * m1 * m1 * m1 * m1 * m1 * m1 * m1 * m1 * m1;
  return m3 == m2;
}

bool test_mult2()
{
  typedef matrix<int, 1, 2> mat_type1;
  typedef matrix<int, 2, 1> mat_type2;
  typedef matrix<int, 1, 1> mat_type3;

  mat_type1 m1(1);
  mat_type2 m2(1);
  const mat_type3 m3 = m1 * m2;
  return m3(0, 0) == 2;
}

bool test_mult3()
{
  typedef matrix<int, 2, 2> mat_type1;

  mat_type1 m1;
  m1(0, 0) = 5;
  m1(0, 1) = 7;
  m1(1, 0) = 3;
  m1(1, 1) = 4;
  mat_type1 m2;
  m2(0, 0) = 1;
  m2(0, 1) = 8;
  m2(1, 0) = 10;
  m2(1, 1) = 6;
  mat_type1 m3;
  m3(0, 0) = 75;
  m3(0, 1) = 82;
  m3(1, 0) = 43;
  m3(1, 1) = 48;
  const mat_type1 m4 = m1 * m2;
  return m3 == m4;
}

bool test_assign_plus_minus()
{
  typedef matrix<int, 2, 2> mat_type;
  mat_type m1(1), m2(1), m3(2), m4(3);

  m1 += m1;
  if (m1 != m3)
    return false;
  m1 -= m2;
  if (m1 != m2)
    return false;
  m1 += m1 + m2;
  if (m1 != m4)
    return false;
  return true;
}

bool test_assign_scalar_mult()
{
  typedef matrix<int, 2, 2> mat_type;
  mat_type m1(1), m2(3);
  m1 *= 3;
  if (m1 == m2)
    return true;
  return false;
}

bool test_assign_mult()
{
  typedef matrix<int, 2, 2> mat_type;
  mat_type m1(1), m2(2), m3(3), m4(12);
  m1 *= m3;

  if (m1 != m3)
    return false;

  m2 *= m2 * m3;
  if (m1 != m4)
    return false;

  return true;
}

int main()
{
  typedef matrix<int, 2, 2> mat_type;

  const mat_type mat1 = gen_mat<int>();

  // Tests operator()
  if ((mat1(0, 0) != 1) || (mat1(0, 1) != 2) || (mat1(1, 0) != 3) || (mat1(1, 1) != 4))
    return 1;

  const mat_type mat2 = gen_mat<int>();
  if (mat1 != mat2)
    return 1;

  const mat_type tmp1(mat1);
  if (tmp1 != mat1)
    return 1;

  const mat_type tmp2 = mat1;
  if (tmp2 != mat1)
    return 1;

  const mat_type tmp3 = 2 * tmp2 + 3 * mat2 - 4 * tmp1;
  if (tmp3 != mat1)
    return 1;

  if (!test_mult1())
    return 1;

  if (!test_mult2())
    return 1;

  if (!test_mult3())
    return 1;

  if (!test_assign_plus_minus())
    return 1;

  if (!test_assign_scalar_mult())
    return 1;

  if (!test_div1())
    return 1;

  if (!test_div2())
    return 1;

  std::cout << "tmp1:\n";
  std::cout << tmp1 << "\n";
  std::cout << "tmp2:\n";
  std::cout << tmp2 << "\n";
  std::cout << "tmp3:\n";
  std::cout << tmp3 << "\n";

  return 0;
}

