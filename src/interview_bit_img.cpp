#include <cmath>
#include <vector>
#include <iostream>
#include <algorithm>

template <class T>
constexpr auto get_bit(T o, int i) // i in [0, sizeof i)
{ return (o & (1 << i)) != 0; }

template <class T>
void set_bit(T& o, int i) { o |= 1 << i; }

template <class T>
void print_bits(T o)
{
  constexpr auto s = 8 * sizeof o;
  for (unsigned i = 0; i < s; ++i)
    std::cout << get_bit(o, i);
}

template <class T>
void print_img(const std::vector<T>& img, int n)
{
  constexpr auto s = 8 * sizeof T {};
  for (unsigned i = 0; i < img.size(); ++i) {
    if ((i * s) % (n * s) == 0) 
      std::cout << "\n";
    print_bits(img[i]);
    std::cout << "|";
  }
  std::cout << "\n";
}

template <class T>
void set_bits_slow( std::vector<T>& img, int cols
                  , int row_bit_begin, int row_bit_end
                  , int col_bit_begin, int col_bit_end)
{
  constexpr int s = 8 * sizeof T {};

  for (auto i = row_bit_begin; i < row_bit_end; ++i)
    for (auto j = col_bit_begin; j < col_bit_end; ++j)
      set_bit(img[i * cols + j / s], j % s);
}

template <class T>
void set_bits_fast( std::vector<T>& img, int cols
                  , int row_bit_begin, int row_bit_end
                  , int col_bit_begin, int col_bit_end)
{
  constexpr int s = 8 * sizeof T {};

  auto begin = col_bit_begin / s;
  T begin_mask = T(-1) << col_bit_begin % s;

  auto end = col_bit_end / s;
  T end_mask = (1 << col_bit_end % s) - 1;
  
  if (begin == end) {
    for (auto i = row_bit_begin; i < row_bit_end; ++i)
      img[i * cols + begin] |= begin_mask & end_mask;

    return;
  }

  for (auto i = row_bit_begin; i < row_bit_end; ++i) {
    img[cols * i + begin] |= begin_mask;

    for (auto j = begin + 1; j < end; ++j)
      img[i * cols + j] = T(-1);

    if (end != cols)
      img[i * cols + end] |= end_mask;
  }
}

int main()
{
  using type = std::uint16_t;

  constexpr int s = 8 * sizeof type {};

  constexpr auto rows = 16;
  constexpr auto cols = 6;

  std::vector<type> img1(rows * cols);
  std::vector<type> img2(rows * cols);

  auto height = 5;
  auto row_bit_begin = std::min(2, rows * s - 1);
  auto row_bit_end = std::min(row_bit_begin + height, rows);

  auto width = 50;
  auto col_bit_begin = std::min(12, cols * s - 1);
  auto col_bit_end = std::min(col_bit_begin + width, cols * s);

  set_bits_slow( img1, cols
               , row_bit_begin, row_bit_end
               , col_bit_begin, col_bit_end);

  print_img(img1, cols);

  set_bits_fast( img2, cols
               , row_bit_begin, row_bit_end
               , col_bit_begin, col_bit_end);

  print_img(img2, cols);
}

