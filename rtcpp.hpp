#pragma once

#include <array>
#include <cmath>
#include <deque>
#include <stack>
#include <vector>
#include <random>
#include <chrono>
#include <numeric>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <functional>
#include <initializer_list>

namespace rt
{

template<class Iter>
void reverse(Iter begin, Iter end) noexcept
{
  auto i = 0;
  auto j = end - begin;
  while (i < --j)
    std::swap(begin[i++], begin[j]);
}

template <class Iter>
auto max_element(Iter begin, Iter end)
{
  if (begin == end)
    return end;

  auto max = begin;
  while (++begin != end)
    if (*max < *begin)
      max = begin;
   
  return max;
}
        
template <class Iter>
auto min_element(Iter begin, Iter end)
{
  if (begin == end)
    return end;

  auto min = begin;
  while (++begin != end)
    if (*begin < *min)
      min = begin;
   
  return min;
}

struct bst_node {
  int info;
  bst_node* left;
  bst_node* right;
};

void visit(bst_node const* p)
{
  std::cout << p->info << "\n";
}

//______________________________________________________
inline
void bst_insert(bst_node& head, int key)
{
  if (!head.left) {
    head.left = new bst_node {key, nullptr, nullptr};
    return;
  }

  auto* p = head.left;
  while (p) {
    if (key < p->info) {
      if (!p->left) {
        p->left = new bst_node {key, nullptr, nullptr};
        return;
      }
      p = p->left;
    } else if (p->info < key) {
      if (!p->right) {
        p->right = new bst_node {key, nullptr, nullptr};
        return;
      }
      p = p->right;
    } else {
      return;
    }
  }
}

//______________________________________________________
inline
void bst_insertion_sort_impl(bst_node& head, int key)
{
  if (!head.left) {
    head.left = new bst_node {key, nullptr, nullptr};
    return;
  }

  auto* p = head.left;
  while (p) {
    if (key < p->info) {
      if (!p->left) {
        p->left = new bst_node {key, nullptr, nullptr};
        return;
      }
      p = p->left;
    } else {
      if (!p->right) {
        p->right = new bst_node {key, nullptr, nullptr};
        return;
      }
      p = p->right;
    }
  }
}

//______________________________________________________
void preorder_recursive(bst_node* p)
{
  if (!p)
    return;

  visit(p);
  preorder_recursive(p->left);
  preorder_recursive(p->right);
}

void preorder_traversal(const bst_node* p)
{
  std::stack<const bst_node*> s;
  while (p) {
    visit(p);
    if (p->right)
      s.push(p->right);

    p = p->left;
    if (!p && !s.empty()) {
      p = s.top();
      s.pop();
    }
  }
}

struct preorder_successor {
  std::stack<bst_node*> s;
  bst_node* p;
  void next()
  {
    if (p->right)
      s.push(p->right);

    p = p->left;
    if (!p && !s.empty()) {
      p = s.top();
      s.pop();
    }
  }
  preorder_successor(bst_node* root) : p(root) {}
};

void preorder_traversal2(bst_node* root)
{
  preorder_successor obj(root);
  while (obj.p) {
    visit(obj.p);
    obj.next();
  }
}

template <class Successor>
class bst_iter {
  private:
  Successor s;
  public:
  using value_type = int;
  using pointer = bst_node*;
  using reference = bst_node&;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;
  bst_iter(bst_node* root = nullptr) noexcept
  : s(root) {}
  auto& operator++() noexcept { s.next(); return *this; }
  auto operator++(int) noexcept
  { auto tmp(*this); operator++(); return tmp; }

  const auto& operator*() const noexcept {return s.p->info;}
  friend auto operator==( const bst_iter& rhs
                        , const bst_iter& lhs) noexcept
  { return lhs.s.p == rhs.s.p; }
  friend auto operator!=( const bst_iter& rhs
                        , const bst_iter& lhs) noexcept
  { return !(lhs == rhs); }
};

void copy(bst_node* from, bst_node* to)
{
  preorder_successor p(from);
  preorder_successor q(to);

  for (;;) {
    if (p.p->left)
      q.p->left = new bst_node {{}, nullptr, nullptr};
    
    p.next();
    q.next();

    if (p.p == from)
      return;

    if (p.p->right)
      q.p->right = new bst_node {{}, nullptr, nullptr};

    q.p->info = p.p->info;
  }
}

//______________________________________________________
void inorder_recursive(bst_node* p)
{
  if (!p)
    return;

  inorder_recursive(p->left);
  visit(p);
  inorder_recursive(p->right);
}

void inorder_traversal(const bst_node* p)
{
  std::stack<const bst_node*> s;
  for (;;) {
    while (p) {
      s.push(p);
      p = p->left;
    }
    if (s.empty())
      return;
    p = s.top();
    s.pop();
    visit(p);
    p = p->right;
  }
}

struct inorder_successor {
  std::stack<const bst_node*> s;
  const bst_node* p;
  void left_most()
  {
    while (p) {
      s.push(p);
      p = p->left;
    }
    if (s.empty())
      return;

    p = s.top();
    s.pop();
  }
  void next()
  {
    p = p->right; 
    left_most();
  }
  inorder_successor(const bst_node* root)
  : p(root) {left_most();}
};

void inorder_traversal2(const bst_node* root)
{
  inorder_successor obj(root);
  while (obj.p) {
    visit(obj.p);
    obj.next();
  }
}

//______________________________________________________

void postorder_recursive(bst_node* p)
{
  if (!p)
    return;

  postorder_recursive(p->left);
  postorder_recursive(p->right);
  visit(p);
}

void postorder_traversal(const bst_node* p)
{
  std::stack<const bst_node*> s;
  const bst_node* q = nullptr;
  for (;;) {
    while (p) {
      s.push(p);
      p = p->left;
    }

    for (;;) {
      if (s.empty())
        return;

      p = s.top();
      s.pop();

      if (!p->right || p->right == q) {
        visit(p);
        q = p;
        continue;
      }
      s.push(p);
      p = p->right;
      break;
    }
  }
}

template <class Iter, class T>
auto find(Iter begin, Iter end, T const& k)
{
  while (begin != end && *begin != k)
    ++begin;

  return begin;
}

// Linear search with a sentinel. Assumes the searched for element
// in the last array element.
template <class Iter, class T>
auto find_with_sentinel(Iter begin, const T& v)
{
  while (*begin != v)
    ++begin;

  return begin;
}

template<class Iter, class T>
auto lower_bound(Iter begin, Iter end, const T& K)
{
  if (begin == end)
    return end;

  auto low = 0;
  auto high = end - begin - 1;

  while (low <= high) {
    int mid = (low + high) / 2;

    if (K < begin[mid])
      high = mid - 1;
    else if (begin[mid] < K)
      low = mid + 1;
    else
      return begin + mid;
  }

  return begin + high + 1;
}

template<class Iter, class T>
bool binary_search(Iter begin, Iter end, const T& K)
{
  if (begin == end)
    return false;

  auto low = 0;
  auto high = end - begin - 1;

  while (low <= high) {
    auto mid = (low + high) / 2;
    if (K < begin[mid])
      high = mid - 1;
    else if (begin[mid] < K)
      low = mid + 1;
    else
      return true;
  }
  return false;
}

template<class Iter, class T>
bool binary_search2(Iter begin, Iter end, const T& K)
{
  auto l = std::distance(begin, end);

  while (l > 0) {
    auto half = l / 2;
    auto mid = begin;
    std::advance(mid, half);

    if (K < *mid) {
      l = half;
    } else if (*mid < K) {
      begin = mid;
      ++begin;
      l = l - half - 1;
    } else {
      return true;
    }
  }
  return false;
}

template<class Iter, class T>
bool binary_search_recursive(Iter begin, Iter end, const T& K)
{
  if (begin == end)
    return false;

  auto mid = (end - begin) / 2;
  if (K < begin[mid])
    return binary_search_recursive(begin, begin + mid, K);
  else if (begin[mid] < K)
    return binary_search_recursive(begin + mid + 1, end, K);
  else
    return true;
}

template <class Iter, class T>
bool binary_search_rotated(Iter begin, Iter end, const T& K)
{
  auto is_sorted = [&](auto a, auto b)
  { return !(begin[b - 1] < begin[a]); };

  auto in_range = [&](auto a, auto b)
  { return !(K < begin[a] || begin[b - 1] < K); };

  auto low = 0;
  auto high = end - begin;
  while (low < high) {
    auto mid = (low + high) / 2;
    if (is_sorted(mid, high)) {
      if (in_range(mid, high))
        return rt::binary_search(begin + mid, begin + high, K);
      high = mid;
    } else {
      if (in_range(low, mid))
        return rt::binary_search(begin + low, begin + mid, K);
      low = mid;
    }
  }
  return false;
}

// ####
//_____________________________________________________________________

/*
  Calculates the next tuple based on the current one.  The first element in
  the array is used for convenience and does not belong in the tuple. Minimum
  and maximum values for each element in the tuple are passed in paramenter
  min and max.

  Example: To Generate all tuples with size 3.

  void all_tuples()
  {
    std::vector<int> min {0, 1, 1, 1};
    std::vector<int> max {min[0] + 1, 3, 2, 3};
    auto arr = min;

    do {
      visit(arr);
    } while (next_tuple( std::begin(arr), std::end(arr)
                       , std::begin(min), std::begin(max)));
  }

  It is important that max[0] != min[0].
*/

template <class Iter>
auto next_tuple( Iter begin, Iter end
               , Iter min, Iter max)
{
    auto j = end - begin - 1;
    while (begin[j] == max[j]) {
      begin[j] = min[j];
      --j;
    }
    ++begin[j];
    
    return j != 0;
}

// Same as next_tuple but requires only bidirectional iterators.
template <class Iter>
auto next_tuple_stl( Iter begin, Iter end
                   , Iter min, Iter max)
{
    auto size = std::distance(begin, end);
    std::advance(min, size);
    std::advance(max, size);

    while (*--end == *--max)
      *end = *--min;

    *end += 1;
    return end != begin;
}

// ####
//_____________________________________________________________________

auto next_combination(std::vector<int>& v)
{
  int t = v.size() - 2;
  auto i = 0;
  while ((v[i] + 1) == v[i + 1]) {
    v[i] = i;
    ++i;
  }
  if (i == t) return false;

  ++v[i];
  return true;
}

// ####
//_____________________________________________________________________

template <class Iter>
bool next_permutation(Iter begin, Iter end)
{
    auto b = end;
    auto a = std::prev(b);

    while (!(*--a < *--b));

    if (a == begin) {
      std::reverse(++a, end);
      return false;
    }

    b = end;
    while (!(*a < *--b));

    std::iter_swap(a, b);

    std::reverse(++a, end);

    return true;
}

template <class Iter>
void inplace_inverse_perm(Iter begin, Iter end, bool begin_at_zero)
{
  auto n = end - begin;
  auto m = n;
  auto j = -1;

  auto k = begin_at_zero ? 1 : 0;
  do {
    auto i = begin[m - 1] + k;
    if (i > -1) {
      do {
        begin[m - 1] = j - k;
        j = -m;
        m = i;
        i = begin[m - 1] + k;
      } while (i > 0);
      i = j;
    }
    begin[m - 1] = -i - k;
    --m;
  } while (m > 0);
}

template <class Iter>
void permute(Iter begin, Iter end, Iter perm)
{
  auto n = end - begin;
  for (auto i = 0; i < n; ++i) {
    if (perm[i] != i) {
      auto t = begin[i];
      auto j = i;
      do {
        auto k = perm[j];
        begin[j] = begin[k];
        perm[j] = j;
        j = k;
      } while (perm[j] != i);
      begin[j] = t;
      perm[j] = j;
    }
  }
}

template <class Iter>
void unpermute(Iter begin, Iter end, Iter table)
{
  auto n = end - begin;
  for (auto i = 0; i < n; ++i) {
      while (table[i] != i) {
          std::swap(begin[i], begin[table[i]]);
          std::swap(table[i], table[table[i]]);
      }
  }
}

template <class Iter, class Index>
void unpermute_on_the_fly(Iter begin, Iter end, Index index)
{
  const auto n = end - begin;
  for (auto i = 0; i < n; ++i) {
    auto k = index(i);
    while (k > i)
      k = index(k);
    if (k == i) {
      k = index(i);
      while (k != i) {
        std::swap(begin[i], begin[k]);
        k = index(k);
      }
    }
  }
}

template <class Iter>
void unpermute_on_the_fly_bit( Iter begin, Iter end, auto index
                             , auto set_bit, auto unset_bit, auto is_set)
{
  const auto n = end - begin;
  for (auto i = 0; i < n; ++i) {
    auto k = index(i);
    if (!is_set(begin[i])) {
      while (k != i) {
        std::swap(begin[i], begin[k]);
        set_bit(begin[k]);
        k = index(k);
      }
    }
    unset_bit(begin[i]);
  }
}

// Shuffles the input array. Expects a parameter rand that produces
// random numbers uniformily distributed in the interval [0, 1).
template <class Rand>
void shuffle(std::vector<int>& v, Rand& rand)
{
  auto j = v.size();
  do {
    auto k = static_cast<int>(j * rand());
    std::swap(v[k], v[--j]);
  } while (j != 0);
}

// Generates a random permutation with elements in the range [1, n].
template <class Rand>
auto random_permutation(int n, Rand& rand)
{
  std::vector<int> ret(n);
  for (auto j = 0; j < n; ++j) {
    auto k = static_cast<int>((j + 1) * rand());
    ret[j] = ret[k];
    ret[k] = j + 1;
  }

  return ret;
}

// ####
//_____________________________________________________________________

void visit_partition(std::vector<int> a, int m)
{
  for (int i = 1; i <= m; ++i)
    std::cout << a[i] << " ";
  std::cout << std::endl;
}

void all_partitions_loop(int n)
{
  std::vector<int> a(n + 1, 1);
  a[0] = 0;

  int m = 1;
  for (;;) {
    a[m] = n;
    int q = m - (n == 1 ? 1 : 0);

    for (;;) {
      visit_partition(a, m);
      if (a[q] != 2) break;
      a[q--] = 1;
      ++m;
    }

    if (q == 0) return;
       
    int x = a[q] - 1;
    a[q] = x;
    n = m - q + 1;
    m = q + 1;

    while (x < n) {
      a[m++] = x;
      n -= x;
    }
  }
}

auto kdelta(int a, int b) { return a == b ? 1 : 0; }

struct next_partition {
  int last, q;
  std::vector<int> a;

  next_partition(int n)
  : last(1), q(last - kdelta(n, 1)), a(n + 1, 1)
  {
    a[0] = 0;
    a[1] = n;
  }

  auto next()
  {
    if (a[q] == 2) {
      a[q--] = 1;
      ++last;
      return true;
    };

    if (q == 0) return false;
       
    int x = a[q] - 1;
    a[q] = x;
    auto n = last - q + 1;
    last = q + 1;

    while (x < n) {
      a[last++] = x;
      n -= x;
    }

    a[last] = n;
    q = last - kdelta(n, 1);
    return true;
  }
};

template <class Iter>
void tree_insertion_sort(Iter begin, Iter end)
{
  bst_node root {};
  auto tmp = begin;
  while (tmp != end)
    bst_insertion_sort_impl(root, *tmp++);

  using iter = rt::bst_iter<inorder_successor>;

  std::copy(iter {root.left}, iter {}, begin);
}

template <class Iter>
void bubble_sort(Iter begin, Iter end)
{
  if (begin == end) return;

  auto B = end - begin - 1;
  while (B != 0) {
    auto t = 0;
    for (auto j = 0; j < B; ++j) {
      if (begin[j] > begin[j + 1]) {
        std::swap(begin[j], begin[j + 1]);
        t = j + 1;
      }
    }
    B = t;
  }
}

template <class Iter>
auto calc_address_table(Iter begin, Iter end)
{
  const auto n = end - begin;
  std::vector<int> count(n, 0);

  for (auto i = 1; i < n; ++i)
    for (auto j = 0; j < i; ++j)
      if (begin[i] < begin[j])
        ++count[j];
      else
        ++count[i];

  return count;
}

template <class Iter>
void comparison_counting_sort(Iter begin, Iter end)
{
  auto n = end - begin;
  auto count = calc_address_table(begin, end);

  std::vector<int> tmp(n, 0);
  for (auto i = 0; i < n; ++i)
    tmp[count[i]] = begin[i];

  std::copy(std::begin(tmp), std::end(tmp), begin);
}

template <class Iter>
void inplace_comparison_counting_sort(Iter begin, Iter end)
{
  auto table = calc_address_table(begin, end);
  rt::unpermute(begin, end, std::begin(table));
}

template <class Iter>
void dist_counting_sort( Iter begin, Iter end
                       , int min, int max)
{
  auto n = end - begin;
  auto m = max - min + 1;

  std::vector<int> count(m, 0);
  for (auto i = 0; i < n; ++i)
    ++count[begin[i] - min];

  for (auto i = 1; i < m; ++i)
    count[i] += count[i - 1];

  std::vector<int> out(n, 0);
  for (auto i = 0; i < n; ++i)
    out[--count[begin[i] - min]] = begin[i];

  std::copy(std::begin(out), std::end(out), begin);
}

template <class Iter>
void straight_insertion(Iter begin, Iter end)
{
  if (begin == end) return;

  auto N = end - begin;
  for (auto j = 1; j < N; ++j) {
    auto i = j - 1;
    auto k = begin[j];
    while (k < begin[i]) {
      begin[i + 1] = begin[i];
      if (--i < 0) break;
    }

    begin[i + 1] = k;
  }
}

template <class Iter>
void binary_insertion(Iter begin, Iter end)
{
  for (auto pos = begin; pos != end; ++pos)
    std::rotate( std::upper_bound(begin, pos, *pos)
               , pos
               , std::next(pos));
}

template <class Iter>
void straight_selection(Iter begin, Iter end)
{
  if (begin == end)
    return;

  for (; begin != std::prev(end); ++begin)
    std::iter_swap( begin
                  , rt::min_element(begin, end));
}

template <class Iter, class Iter2>
auto merge( Iter begin1, Iter end1
          , Iter begin2, Iter end2
          , Iter2 output)
{
    if (begin1 == end1) return output;

    for (;;) {
        if (*begin1 <= *begin2) {
            *output++ = *begin1++;
            if (begin1 == end1) {
                std::copy(begin2, end2, output++);
                return output;
            }
        } else {
            *output++ = *begin2++;
            if (begin2 == end2) {
                std::copy(begin1, end1, output++);
                return output;
            }
        }
    }

    return output;
}

//____________________________________________________________________

constexpr auto is_power_of_two(std::size_t N) noexcept
{
  return N > 0 && (N & (N - 1)) == 0;
}

// Returns true if a is aligned with respect to N i.e. the remainder of a
// divided by b is zero. N must be a power of two.
constexpr auto is_aligned(std::size_t a, std::size_t N) noexcept
{ 
  return (a & (N - 1)) == 0;
}

constexpr auto align_previous(std::size_t a, std::size_t N) noexcept
{
  return (a & ~(N - 1));
}

constexpr auto align_next(std::size_t a, std::size_t N) noexcept
{
  return align_previous(a, N) + N;
}

// If p is not aligned on an N boundary, this function will align it and
// update s if alignment took place.
void align_if_needed(void*& p, std::size_t& s, std::size_t N) noexcept
{
  const auto a = reinterpret_cast<std::uintptr_t>(p);
  const auto c = is_aligned(a, N) ? a : align_next(a, N);
  p = reinterpret_cast<void*>(c);
  s -= c - a;
}

class timer {
private:
  std::chrono::time_point<std::chrono::system_clock> m_start;
public:
  timer() : m_start(std::chrono::system_clock::now()) {}
  auto get_count() const
  { 
    std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
    auto diff = end - m_start;
    auto diff2 = std::chrono::duration_cast<std::chrono::milliseconds>(diff);
    return diff2.count();
  }
  ~timer()
  {
    //std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
    //auto diff = end - m_start;
    //auto diff2 = std::chrono::duration_cast<std::chrono::milliseconds>(diff);
    //std::cout << diff2.count() << " ";
  }
};

template <std::size_t n>
struct dot_product_impl {
  template <typename RIter1, typename RIter2> // P is a random access iter.
  static typename std::iterator_traits<RIter1>::value_type apply(RIter1 p1, RIter2 p2)
  { return p1[n - 1] * p2[n - 1] + dot_product_impl<n - 1>::apply(p1, p2); }
};

template <>
struct dot_product_impl<1> {
  template <typename RIter1, typename RIter2>
  static typename std::iterator_traits<RIter2>::value_type apply(RIter1 p1, RIter2 p2)
  { return p1[0] * p2[0]; }
};

template <std::size_t n, typename RIter1, typename RIter2>
typename std::iterator_traits<RIter1>::value_type dot_product(RIter1 p1, RIter2 p2)
{ return dot_product_impl<n>::apply(p1, p2); }

template <std::size_t N, typename RandomAccessIter>
typename std::iterator_traits<RandomAccessIter>::value_type snorm(RandomAccessIter iter)
{ return dot_product<N>(iter, iter); }

inline
std::size_t row_major_idx(std::size_t i, std::size_t j,
  std::size_t n_cols) { return i * n_cols + j; }

template <typename Derived>
struct matrix_traits;

template <typename E>
class matrix_expr {
  public:
  using value_type = typename matrix_traits<E>::value_type;
  using size_type = typename matrix_traits<E>::size_type;
  static constexpr std::size_t rows = matrix_traits<E>::rows;
  static constexpr std::size_t cols = matrix_traits<E>::cols;
  value_type operator()(size_type i, size_type j) const
  {return static_cast<const E&>(*this)(i, j);}
  operator E&() {return static_cast<E&>(*this);}
  operator const E&() const {return static_cast<const E&>(*this);}
};

template <typename T, std::size_t M, std::size_t N>
class matrix : public matrix_expr<matrix<T, M, N> > {
public:
  static constexpr std::size_t rows = M;
  static constexpr std::size_t cols = N;
  static constexpr std::size_t data_size = rows * cols;
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
  constexpr matrix() : m_data() {}
  explicit matrix(const T& val) { fill(val);};
  reference operator[](size_type i) {return m_data[i];}
  const_reference operator[](size_type i) const {return m_data[i];}
  value_type operator()(size_type i, size_type j) const
  {return m_data[row_major_idx(i, j, N)];}
  reference operator()(size_type i, size_type j)
  {return m_data[row_major_idx(i, j, N)];}
  iterator begin() {return m_data.begin();}
  iterator end() {return m_data.end();}
  constexpr auto size() const {return M * N;}
  const_iterator begin() const {return m_data.begin();}
  const_iterator end() const {return m_data.end();}
  const_iterator cbegin() const {return m_data.begin();}
  const_iterator cend() const {return m_data.end();}
  const_iterator row_cbegin(size_type i) const
  {return m_data.begin() + row_major_idx(i, 0, N);}
  reference front() {return *begin();}
  const_reference front() const {return *cbegin();}
  reference back() {return *(begin() + data_size - 1);}
  const_reference back() const
  {return *(cbegin() + data_size - 1);}
  const_iterator row_cend(size_type i) const
  {return row_cbegin(i) + N;}
  iterator row_begin(size_type i)
  {return m_data.begin() + row_major_idx(i, 0, N);}
  iterator row_end(size_type i) {return row_begin(i) + N;}
  template <typename E>
  matrix(const matrix_expr<E>& mat)
  {
    static_assert(E::rows == rows, "Matrix with incompatible number of rows.");
    static_assert(E::cols == cols, "Matrix with incompatible number of columns.");
    for (size_type i = 0; i < rows; ++i)
      for (size_type j = 0; j < cols; ++j)
        m_data[row_major_idx(i, j, cols)] = mat(i, j);
  }
  matrix(std::initializer_list<T> init)
  { std::copy(std::begin(init), std::end(init), begin()); }
  matrix& operator=(const matrix<T,M,N>& rhs)
  {
    if (this != &rhs)
      std::copy(std::begin(rhs), std::end(rhs), begin());
    return *this;
  }
  matrix<T,M,N>& operator=(std::initializer_list<T> init)
  { std::copy(std::begin(init), std::end(init), begin()); }
  bool operator==(const matrix<T,M,N>& rhs) const
  { return std::equal(cbegin(), cend(), rhs.cbegin()); }
  bool operator!=(const matrix<T,M,N>& rhs) const
  { return !(*this == rhs);}
  void fill(const T& val)
  { std::fill(std::begin(m_data), std::end(m_data), val);};
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
  static constexpr size_type rows = M;
  static constexpr size_type cols = N;
};

template <typename E1, typename E2>
class matrix_diff : public matrix_expr<matrix_diff<E1, E2> > {
  const E1& m_u;
  const E2& m_v;
  public:
  using size_type = typename E1::size_type;
  using value_type = typename E1::value_type;
  static constexpr size_type rows = E1::rows;
  static constexpr size_type cols = E1::cols;
  matrix_diff(const matrix_expr<E1>& u, const matrix_expr<E2>& v)
  : m_u(u)
  , m_v(v)
  {}
  value_type operator()(size_type i, size_type j) const
  {return m_u(i, j) - m_v(i, j);}
};

template <typename E1, typename E2>
struct matrix_traits<matrix_diff<E1, E2> > {
  using value_type = typename E1::value_type;
  using size_type = typename E1::size_type;
  static constexpr size_type rows = E1::rows;
  static constexpr size_type cols = E1::cols;
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
  static constexpr size_type rows = E1::rows;
  static constexpr size_type cols = E1::cols;
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
  static constexpr size_type rows = E1::rows;
  static constexpr size_type cols = E1::cols;
};

template <typename E1, typename E2>
matrix_sum<E1, E2> const operator+(const matrix_expr<E1>& u, const matrix_expr<E2>& v)
{ return matrix_sum<E1, E2>(u, v); }

template <typename E>
class matrix_scaled : public matrix_expr<matrix_scaled<E> > {
  public:
  using value_type = typename E::value_type; 
  using size_type = typename E::size_type; 
  static constexpr size_type rows = E::rows;
  static constexpr size_type cols = E::cols;
  private:
  value_type m_val; 
  const E& m_v;
  public:
  matrix_scaled(value_type val, const matrix_expr<E>& v)
  : m_val(val)
  , m_v(v)
  {}
  value_type operator()(size_type i, size_type j) const
  {return m_val * m_v(i, j);}
};

template <typename E>
struct matrix_traits<matrix_scaled<E> > {
  using value_type = typename E::value_type;
  using size_type = typename E::size_type;
  static constexpr size_type rows = E::rows;
  static constexpr size_type cols = E::cols;
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
  const double tmp = static_cast<double>(1) / val;
  return matrix_scaled<E>(tmp, v);
}

template <typename E1, typename E2>
class matrix_prod : public matrix_expr<matrix_prod<E1, E2> > {
  private:
  static_assert(E1::cols == E2::rows, "Incompatible number of rows and columns.");
  const E1& m_u;
  const E2& m_v;
  public:
  using size_type = typename E1::size_type;
  using value_type = typename E1::value_type;
  static constexpr size_type rows = E1::rows;
  static constexpr size_type cols = E2::cols;
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
  static constexpr size_type rows = E1::rows;
  static constexpr size_type cols = E2::cols;
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

// Generates a uniform distribution of integers in the range
// [first, last] with size size.
// The following options for the argument type are available.
// 1 - The returned vector can have repeated elements.
// 2 - All elements appear only once. The vector can have size less the
//     "size".

auto make_rand_data(int size, int first, int last, int type = 2)
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dis(first, last);

  std::vector<int> data;
  std::generate_n( std::back_inserter(data), size,
    [&](){return dis(gen);});
  if (type == 1)
    return data;

  std::sort(std::begin(data), std::end(data));
  data.erase(std::unique(std::begin(data), std::end(data)),
             std::end(data));
  shuffle(std::begin(data), std::end(data), gen);
  return data;
}

} // rt

