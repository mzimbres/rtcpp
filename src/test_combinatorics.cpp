#include <iostream>
#include <vector>
#include <random>
#include <stack>
#include <deque>
#include <array>

#include "rtcpp.hpp"
#include "test.hpp"

void visit_tuple(std::vector<int> const& arr)
{
  if (arr.size() < 2)
    return;

  for (unsigned i = 1; i < arr.size(); ++i)
    std::cout << arr[i];
  std::cout << std::endl;
}

void all_tuples_stl()
{
  std::vector<int> min {1, 1, 1, 1};
  std::vector<int> max {2, 3, 2, 3};
  auto arr = min;

  do {
    visit_tuple(arr);
  } while (rt::next_tuple_stl( std::begin(arr)
                             , std::end(arr)
                             , std::begin(min)
                             , std::begin(max)));
}

void all_tuples()
{
  std::vector<int> min {0, 1, 1, 1};
  std::vector<int> max {2, 3, 2, 3};
  auto arr = min;

  do {
    visit_tuple(arr);
  } while (rt::next_tuple( std::begin(arr), std::end(arr)
                         , std::begin(min), std::begin(max)));
}

template <class T>
void visit2(const std::vector<T>& arr)
{
  for (unsigned i = 0; i < arr.size(); ++i)
    std::cout << arr[i];
  std::cout << std::endl;
}

void all_tuples_recursive(std::vector<int> arr, unsigned idx)
{
  static const std::vector<int> min {0, 1, 1, 1};
  static const std::vector<int> max {2, 3, 2, 3};

  if (idx == arr.size()) {
    visit2(arr);
    return;
  }

  for (auto i = min[idx]; i <= max[idx]; ++i) {
    arr[idx] = i;
    all_tuples_recursive(arr, idx + 1);
  }
}

void visit_binary_tuple(int v, const std::string& str)
{
      for (unsigned i = 0; i < str.size(); ++i)
        if (v & (1 << i))
          std::cout << str[str.size() - i - 1];
      std::cout << "\n";
}

void binary_tuples()
{
    std::string str {'z', 'r', 'k', 'g'};

    auto v = (1 << str.size()) - 1;
    do {
      visit_binary_tuple(v, str);
    } while (v-- != 0);
}

void visit_combination(const std::vector<int>& arr)
{
  if (arr.size() < 2)
    return;

  for (unsigned i = 0; i < arr.size() - 2; ++i)
    std::cout << arr[i];
  std::cout << std::endl;
}

void all_combinations()
{
  int n = 5;
  std::vector<int> v {0, 1, 2, n, 0};

  do {
    visit_combination(v);
  } while (rt::next_combination(v));
}

void all_partitions()
{
  rt::next_partition part(6);
  do {
    rt::visit_partition(part.a, part.last);
  } while (part.next());
}

//________________________________________________

void visit_permutation(const std::vector<int>& v, int b = 1)
{
  for (unsigned i = b; i < v.size(); ++i)
    std::cout << v[i] << " ";
  std::cout << std::endl;
}

void all_permutations()
{
  std::vector<int> v{0, 1, 2, 3, 4};
  do {
    visit_permutation(v);
  } while (rt::next_permutation(std::begin(v), std::end(v)));
}

void visit_permutation(const std::deque<int>& v)
{
  for (unsigned i = 0; i < v.size(); ++i)
    std::cout << v[i];
  std::cout << std::endl;
}

void perm_rec( std::stack<int> s
             , std::deque<int> d = {})
{
  if (s.empty()) {
    visit_permutation(d);
    return;
  }

  d.push_front(s.top());
  s.pop();

  unsigned i = 0, j = 1;
  for (;;) {
    perm_rec(s, d);
    if (j == d.size()) break;
    std::swap(d[i++], d[j++]);
  }
}

template <class Rand>
void shuffle_recursive( std::stack<int> s, Rand& rand
                      , std::deque<int> d = {})
{
  if (s.empty()) {
    visit_permutation(d);
    return;
  }

  d.push_front(s.top());
  s.pop();

  auto i = rand() % d.size();
  std::swap(d.front(), d[i]);
  shuffle_recursive(s, rand, d);
}

template <class Rand>
void shuffle_recursive2( std::vector<int>& v, int i
                       , Rand& rand)
{
  if (i == 0)
    return;

  shuffle_recursive2(v, i - 1, rand);

  auto j = rand() % (i + 1);
  std::swap(v[i], v[j]);
  visit_permutation(v, 0);
}

void test_inverse_perm( std::vector<int> a, std::vector<int> b
                      , bool begin_at_zero)
{
  rt::inplace_inverse_perm(std::begin(a), std::end(a), begin_at_zero);

  if (b != a)
    throw std::runtime_error("Inverse permutation.");
}

void test_unpermute()
{
  std::vector<int> input {4, 8, 2, 5, 1, 6, 7, 3, 9};
  std::vector<int> perm {3, 7, 1, 4, 0, 5, 6, 2, 8};
  rt::unpermute(std::begin(input), std::end(input), std::begin(perm));

  const auto b = std::is_sorted(std::begin(input), std::end(input));
  if (!b)
    throw std::runtime_error("test_unpermute");

  for (auto o : input)
    std::cout << o << " ";
  std::cout << std::endl;

  for (auto o : perm)
    std::cout << o << " ";
  std::cout << std::endl;
}

void test_permute()
{
  std::vector<int> input {4, 8, 2, 5, 1, 6, 7, 3, 9};
  std::vector<int> perm {4, 2, 7, 0, 3, 5, 6, 1, 8};
  rt::permute(std::begin(input), std::end(input), std::begin(perm));

  const auto b = std::is_sorted(std::begin(input), std::end(input));
  if (!b)
    throw std::runtime_error("test_unpermute");

  for (auto o : input)
    std::cout << o << " ";
  std::cout << std::endl;

  for (auto o : perm)
    std::cout << o << " ";
  std::cout << std::endl;
}

RT_TEST(test_transpose1)
{
  constexpr auto r = 2;
  constexpr auto c = 4;

  const std::vector<int> mat   {1, 2, 3, 4, 5, 6, 7, 8};
  const std::vector<int> trans {1, 5, 2, 6, 3, 7, 4, 8};

  auto m1 = mat;
  rt::print(m1);

  const auto index1 = [=](auto i)
  { return (r * i) % (r * c - 1); };

  rt::unpermute_on_the_fly(std::begin(m1), std::end(m1) - 1, index1);
  rt::print(m1);
  RT_CHECK(m1 == trans)

  const auto index2 = [=](auto i)
  { return (c * i) % (r * c - 1); };

  rt::unpermute_on_the_fly(std::begin(m1), std::end(m1) - 1, index2);
  rt::print(m1);

  RT_CHECK(m1 == mat)
}

RT_TEST(test_transpose3)
{
  constexpr auto r = 2;
  constexpr auto c = 4;

  const std::vector<int> mat {1, 2, 3, 4, 5, 6, 7, 8};
  const std::vector<int> trans {1, 5, 2, 6, 3, 7, 4, 8};

  constexpr auto bit = 1 << 30;
  const auto set_bit = [=](auto& o) {o |= bit;};
  const auto unset_bit = [=](auto& o) {o &= ~bit;};
  const auto is_set = [=](auto& o) {return o & bit;};

  auto m1 = mat;
  rt::print(m1);

  const auto index_a = [=](auto i)
  { return (r * i) % (r * c - 1); };

  rt::unpermute_on_the_fly_bit( std::begin(m1), std::end(m1) - 1
                              , index_a, set_bit, unset_bit, is_set);
  rt::print(m1);
  RT_CHECK(m1 == trans)

  const auto index_b = [=](auto i)
  { return (c * i) % (r * c - 1); };

  rt::unpermute_on_the_fly_bit( std::begin(m1), std::end(m1) - 1
                              , index_b, set_bit, unset_bit, is_set);
  rt::print(m1);

  RT_CHECK(m1 == mat)
}

RT_TEST(test_mirror_x)
{
  constexpr auto c = 4;

  const std::vector<int> mat   {1, 2, 3, 4, 5, 6, 7, 8};
  const std::vector<int> trans {4, 3, 2, 1, 8, 7, 6, 5};

  auto m1 = mat;
  rt::print(m1);

  const auto index = [=](auto i)
  { return c - 1 - (i % c) + c * (i / c); };

  rt::unpermute_on_the_fly(std::begin(m1), std::end(m1), index);
  rt::print(m1);
  RT_CHECK(m1 == trans)

  rt::unpermute_on_the_fly(std::begin(m1), std::end(m1), index);
  rt::print(m1);

  RT_CHECK(m1 == mat)
}

int main()
{
  try {
    std::cout << "Next Combination." << std::endl;
    all_combinations();
    std::cout << "All all_partitions." << std::endl;
    rt::all_partitions_loop(6);
    std::cout << "Next partition." << std::endl;
    all_partitions();
    std::cout << "All tuples rucursive" << std::endl;
    all_tuples_recursive(std::vector<int>{1, 1, 1}, 0);
    std::cout << "Next tuple." << std::endl;
    all_tuples();
    std::cout << "Next tuple stl." << std::endl;
    all_tuples_stl();
    std::cout << "Binary tuple." << std::endl;
    binary_tuples();
    std::cout << "All permutations." << std::endl;
    all_permutations();

    std::cout << "All permutations recursive." << std::endl;
    std::stack<int> s{{1, 2, 3}};
    perm_rec(s);

    std::cout << "Shuffle array recursive1." << std::endl;

    std::mt19937 gen {};
    std::uniform_int_distribution<int> dist(0, 51);

    const auto rand = [&](){return dist(gen);};

    std::stack<int> s2 {{1, 2, 3, 4, 5, 6, 7}};
    shuffle_recursive(s2, rand);

    std::cout << "Shuffle array recursive2." << std::endl;
    std::vector<int> vec {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
    shuffle_recursive2(vec, vec.size() - 1, rand);

    std::cout << "Shuffle array." << std::endl;
    std::vector<int> vec2 {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
    std::uniform_real_distribution<> real_dist {};
    const auto real_rand = [&](){ return real_dist(gen);};
    rt::shuffle(vec2, real_rand);
    visit_permutation(vec2, 0);
    std::cout << "Random permutation." << std::endl;
    visit_permutation(rt::random_permutation(30, real_rand), 0);

    test_inverse_perm({4, 1, 3, 2}, {2, 4, 3, 1}, false);
    test_inverse_perm({3, 0, 1, 2}, {1, 2, 3, 0}, true);
    std::cout << "Test unpermute." << std::endl;
    test_unpermute();
    std::cout << "Test permute." << std::endl;
    test_permute();
    std::cout << "Test unpermute_on_the_fly." << std::endl;
    test_transpose1();
    std::cout << "Test unpermute_on_the_fly_bit." << std::endl;
    test_transpose3();
    std::cout << "Test mirror_x." << std::endl;
    test_mirror_x();
  } catch (...) {
    return 1;
  }
  return 0;
}

