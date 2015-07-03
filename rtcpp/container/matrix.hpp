#pragma once

#include <iostream>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <array>

#include <rtcpp/algorithm/snorm.hpp>

#include "row_major_idx.hpp"

namespace rtcpp {

template <typename Derived>
struct matrix_traits;

template <typename E>
class matrix_expr {
  public:
  using value_type = typename matrix_traits<E>::value_type;
  using size_type = typename matrix_traits<E>::size_type;
  static const std::size_t rows = matrix_traits<E>::rows;
  static const std::size_t cols = matrix_traits<E>::cols;
  value_type operator()(size_type i, size_type j) const {return static_cast<const E&>(*this)(i, j);}
  operator E&() {return static_cast<E&>(*this);}
  operator const E&() const {return static_cast<const E&>(*this);}
};

template <typename T, std::size_t M, std::size_t N>
class matrix : public matrix_expr<matrix<T, M, N> > {
public:
  static const std::size_t rows = M;
  static const std::size_t cols = N;
  static const std::size_t data_size = rows * cols;
private:
  using data_type = std::array<T, rows * cols>;
public:
  using value_type = typename data_type::value_type;
  using size_type = typename data_type::size_type;
  using iterator = typename data_type::iterator;
  using const_iterator = typename data_type::const_iterator;
  using reference = typename data_type::reference;
  using const_reference = typename data_type::const_reference;
private:
  data_type m_data;
public:
  matrix() {}
  explicit matrix(const T& val) { fill(val);};
  reference operator[](size_type i) {return m_data[i];}
  const_reference operator[](size_type i) const {return m_data[i];}
  value_type operator()(size_type i, size_type j) const {return m_data[row_major_idx(i, j, N)];}
  reference operator()(size_type i, size_type j) {return m_data[row_major_idx(i, j, N)];}
  iterator begin() {return m_data.begin();}
  iterator end() {return m_data.end();}
  const_iterator begin() const {return m_data.begin();}
  const_iterator end() const {return m_data.end();}
  const_iterator cbegin() const {return m_data.begin();}
  const_iterator cend() const {return m_data.end();}
  const_iterator row_cbegin(size_type i) const {return m_data.begin() + row_major_idx(i, 0, N);}
  reference front() {return *begin();}
  const_reference front() const {return *cbegin();}
  reference back() {return *(begin() + data_size);}
  const_reference back() const {return *(cbegin() + data_size);}
  const_iterator row_cend(size_type i) const {return row_cbegin(i) + N;}
  iterator row_begin(size_type i) {return m_data.begin() + row_major_idx(i, 0, N);}
  iterator row_end(size_type i) {return row_begin(i) + N;}
  template <typename E>
  matrix(const matrix_expr<E>& mat)
  {
    static_assert(E::rows == rows, "Incompatible matrix sizes.");
    static_assert(E::cols == cols, "Incompatible matrix sizes.");
    for (size_type i = 0; i < rows; ++i)
      for (size_type j = 0; j < cols; ++j)
        m_data[row_major_idx(i, j, cols)] = mat(i, j);
  }
  matrix<T,M,N>& operator=(const matrix<T,M,N>& rhs)
  {
    if (this != &rhs)
      std::copy(rhs.cbegin(), rhs.cend(), begin());
    return *this;
  }
  bool operator==(const matrix<T,M,N>& rhs) const { return std::equal(cbegin(), cend(), rhs.cbegin()); }
  bool operator!=(const matrix<T,M,N>& rhs) const { return !(*this == rhs);}
  void fill(const T& val) { std::fill(m_data.begin(), m_data.end(), val);};
};

template <typename T, std::size_t M, std::size_t N>
struct matrix_traits<matrix<T, M, N> > {
  using container_type = std::array<T, M * N>;
  using value_type = typename container_type::value_type;
  using reference = typename container_type::reference;
  using const_reference = typename container_type::const_reference;
  using size_type = typename container_type::size_type;
  using iterator = typename container_type::iterator;
  using const_iterator = typename container_type::const_iterator;
  using difference_type = typename container_type::difference_type;
  static const size_type rows = M;
  static const size_type cols = N;
};

template <typename E1, typename E2>
class matrix_diff : public matrix_expr<matrix_diff<E1, E2> > {
  const E1& m_u;
  const E2& m_v;
  public:
  using size_type = typename E1::size_type;
  using value_type = typename E1::value_type;
  static const size_type rows = E1::rows;
  static const size_type cols = E1::cols;
  matrix_diff(const matrix_expr<E1>& u, const matrix_expr<E2>& v)
  : m_u(u)
  , m_v(v)
  {}
  value_type operator()(size_type i, size_type j) const {return m_u(i, j) - m_v(i, j);}
};

template <typename E1, typename E2>
struct matrix_traits<matrix_diff<E1, E2> > {
  using value_type = typename E1::value_type;
  using size_type = typename E1::size_type;
  static const size_type rows = E1::rows;
  static const size_type cols = E1::cols;
};

template <typename E1, typename E2>
matrix_diff<E1, E2> const operator-(const matrix_expr<E1>& u, const matrix_expr<E2>& v)
{ return matrix_diff<E1, E2>(u, v); }

template <typename E1, typename E2>
class matrix_sum : public matrix_expr<matrix_sum<E1, E2> > {
  const E1& m_u;
  const E2& m_v;
  public:
  using size_type = typename E1::size_type;
  using value_type = typename E1::value_type;
  static const size_type rows = E1::rows;
  static const size_type cols = E1::cols;
  matrix_sum(const matrix_expr<E1>& u, const matrix_expr<E2>& v)
  : m_u(u)
  , m_v(v)
  {}
  value_type operator()(size_type i, size_type j) const {return m_u(i, j) + m_v(i, j);}
};

template <typename E1, typename E2>
struct matrix_traits<matrix_sum<E1, E2> > {
  using value_type = typename E1::value_type;
  using size_type = typename E1::size_type;
  static const size_type rows = E1::rows;
  static const size_type cols = E1::cols;
};

template <typename E1, typename E2>
matrix_sum<E1, E2> const operator+(const matrix_expr<E1>& u, const matrix_expr<E2>& v)
{ return matrix_sum<E1, E2>(u, v); }

template <typename E>
class matrix_scaled : public matrix_expr<matrix_scaled<E> > {
  public:
  using value_type = typename E::value_type; 
  using size_type = typename E::size_type; 
  static const size_type rows = E::rows;
  static const size_type cols = E::cols;
  private:
  value_type m_val; 
  const E& m_v;
  public:
  matrix_scaled(value_type val, const matrix_expr<E>& v)
  : m_val(val)
  , m_v(v)
  {}
  value_type operator()(size_type i, size_type j) const {return m_val * m_v(i, j);}
};

template <typename E>
struct matrix_traits<matrix_scaled<E> > {
  using value_type = typename E::value_type;
  using size_type = typename E::size_type;
  static const size_type rows = E::rows;
  static const size_type cols = E::cols;
};

template <typename E>
matrix_scaled<E> const operator*(typename E::value_type val, const matrix_expr<E>& v)
{ return matrix_scaled<E>(val, v); }

template <typename E>
matrix_scaled<E> const operator*(const matrix_expr<E>& v, typename E::value_type val)
{ return matrix_scaled<E>(val, v); }

template <typename E>
matrix_scaled<E> const operator/(const matrix_expr<E>& v, typename E::value_type val)
{
  using value_type = typename E::value_type;
  const double tmp = static_cast<double>(1) / val;
  return matrix_scaled<E>(tmp, v);
}

template <typename E1, typename E2>
class matrix_prod : public matrix_expr<matrix_prod<E1, E2> > {
  private:
  static_assert(E1::cols == E2::rows, "Incompatible matrix sizes.");
  const E1& m_u;
  const E2& m_v;
  public:
  using size_type = typename E1::size_type;
  using value_type = typename E1::value_type;
  static const size_type rows = E1::rows;
  static const size_type cols = E2::cols;
  matrix_prod(const matrix_expr<E1>& u, const matrix_expr<E2>& v)
  : m_u(u)
  , m_v(v)
  {}
  value_type operator()(size_type i, size_type j) const
  {
    value_type tmp = 0;
    for (size_type k = 0; k < E2::rows; ++k)
      tmp += m_u(i, k) * m_v(k, j);

    return tmp;
  }
};

template <typename E1, typename E2>
struct matrix_traits<matrix_prod<E1, E2> > {
  using value_type = typename E1::value_type;
  using size_type = typename E1::size_type;
  static const size_type rows = E1::rows;
  static const size_type cols = E2::cols;
};

template <typename E1, typename E2>
matrix_prod<E1, E2> const operator*(const matrix_expr<E1>& u, const matrix_expr<E2>& v)
{ return matrix_prod<E1, E2>(u, v); }

template <typename E>
void operator+=( matrix<typename E::value_type, E::rows, E::cols >& u
               , const matrix_expr<E>& v)
{
  const matrix<typename E::value_type, E::rows, E::cols> tmp = u + v;
  u = tmp;
}

template <typename E>
void operator-=( matrix< typename E::value_type, E::rows, E::cols>& u
               , const matrix_expr<E>& v)
{
  const matrix<typename E::value_type, E::rows, E::cols> tmp = u - v;
  u = tmp;
}

template <typename T1, std::size_t M, std::size_t N, typename T2>
void operator*=(matrix< T1, M, N>& u, T2 val)
{
  const matrix<T1, M, N> tmp = u * val;
  u = tmp;
}

template <typename T1, std::size_t M, std::size_t N, typename T2>
void operator/=(matrix< T1, M, N>& u, T2 val)
{
  const matrix<T1, M, N> tmp = u / val;
  u = tmp;
}

template <typename E>
void operator*=( matrix< typename E::value_type, E::rows, E::cols>& u
               , const matrix_expr<E>& v)
{
  const matrix<typename E::value_type, E::rows, E::cols> tmp = u * v;
  u = tmp;
}

template <typename T, std::size_t M, std::size_t N>
matrix<T, N, M> transpose(const matrix<T, M, N>& mat)
{
  matrix<T, N, M> tmp;
  for (std::size_t i = 0; i < M; ++i)
    for (std::size_t j = 0; j < N; ++j)
      tmp(j, i) = mat(i, j);

  return tmp;
}

template <typename T, std::size_t N>
T snorm(const matrix<T, N, 1>& mat) // Squared norm.
{ return snorm<N>(mat.cbegin());}

template <typename T, std::size_t N>
T snorm(const matrix<T, 1, N>& mat)
{ return snorm<N>(mat.cbegin());}

template <typename T, std::size_t M, std::size_t N>
std::ostream& operator<<(std::ostream& os, const matrix<T, M, N>& mat)
{
  for (std::size_t i = 0; i < M; ++i) {
    std::copy( mat.row_cbegin(i)
             , mat.row_cend(i)
             , std::ostream_iterator<T>(os, " "));
    os << "\n";
  }
  os << std::endl;
  return os;
}

}

