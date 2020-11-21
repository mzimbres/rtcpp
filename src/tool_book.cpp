#include <iostream>
#include <iomanip>
#include <cmath>

template <class Mean, class Sigma>
void print(int a, int b, int step, Mean mean, Sigma sigma)
{
  std::cout.precision(2);
  for (auto i = a; i < b; i += step)
    std::cout << "(" << std::setw(3) << i << ", "
              << std::fixed << std::right << mean(i) << ") +- (0, "
              << std::fixed << std::right << sigma(i) << ")"
              << std::endl;
}

auto harm_number = [](auto n)
{
  double sum = 1;
  for (auto i = 2; i <= n; ++i)
    sum += double(1) / i;

  return sum;
};

auto riemann_zeta2 = [] (auto n)
{
  double sum = 1;
  for (auto i = 2; i <= n; ++i)
    sum += double(1) / (i * i);

  return sum;
};

void linear_search_stat()
{
  // Equal prob.
  const auto mean_equal_prob = [](auto n)
  {
    return double(n + 1) / 2;
  };

  const auto sigma_equal_prob = [](auto n)
  {
    double v = n * n - 1;
    return std::sqrt(v / 12);
  };

  // Zipf
  const auto mean_zipf = [=](auto n)
  {
    return double(n) / harm_number(n);
  };

  const auto sigma_zipf = [=](auto n)
  {
    auto hn = harm_number(n);
    double v = double(n * (n + 1)) / (2 * hn) - n * n / (hn * hn);
    return std::sqrt(v);
  };

  auto a = 50;
  auto b = 151;
  auto step = 8;

  std::cout << "\nEqual Probs\n\n";
  print(a, b, step, mean_equal_prob, sigma_equal_prob);
  std::cout << "\nZipf law\n\n";
  print(a + 4, b + 4, step, mean_zipf, sigma_zipf);
}


void max_element_stat()
{
  auto a = 10;
  auto b = a + 151;
  auto step = 12;

  const auto sigma = [&] (auto n)
  {
    auto hn = harm_number(n);
    auto rz2 = riemann_zeta2(n);
    return std::sqrt(hn + rz2);
  };

  print(a, b, step, harm_number, sigma);
}

void inversions_stat()
{
  auto a = 10;
  auto b = a + 21;
  auto step = 2;

  const auto mean = [&] (auto n)
  {
    return n * (n - 1) / 4;
  };

  const auto sigma = [&] (auto n)
  {
    auto v = n * (n - 1) * (2 * n + 5) / 72;
    return std::sqrt(v);
  };

  print(a, b, step, mean, sigma);
}

int main()
{
  std::cout << "\nLinear search\n";
  linear_search_stat();
  std::cout << "\nMax element\n";
  max_element_stat();
  std::cout << "\nInversions\n";
  inversions_stat();
}

