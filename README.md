# Fundamental algorithms

I began this project to experiment with data structures and algorithms
in `C++`, as a result I ended up writing a book which is still ongoig
work. You can find it in doc/book.pdf or [Fundamental Algorithms](https://github.com/mzimbres/rtcpp/blob/a9634ab4fa3076d2ebfc4e34c22b324af5f99b16/doc/book.pdf)

Another topic that I touched is memory allocation in `C++`. Most
specifically how to improve allocation performance for node based
containers. The proposal can be found at [Splitting node and array allocations in allocators](http://open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0310r0.pdf)
Slides are also available in doc/p0310r0.pdf or [here](https://github.com/mzimbres/rtcpp/blob/a9634ab4fa3076d2ebfc4e34c22b324af5f99b16/doc/P0310R0.pdf)

# Samples

Implementation of some algorithms found in the book.

## Binary search tree

Assume a node in the form

```cpp
struct bst_node {
  int info;
  bst_node* left;
  bst_node* right;
};
```

### Insertion

```cpp
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
```

### Pre-order traversal (recursive)

```cpp
void preorder_recursive(bst_node* p)
{
  if (!p)
    return;

  visit(p);
  preorder_recursive(p->left);
  preorder_recursive(p->right);
}

```

### Pre-order traversal (iterative)

```cpp
void preorder_traversal(const bst_node* p)
{
  std::stack<bst_node const*> s;
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
```

### In-order traversal (recursive)

```cpp
void inorder_recursive(bst_node* p)
{
  if (!p)
    return;

  inorder_recursive(p->left);
  visit(p);
  inorder_recursive(p->right);
}
```

### In-order traversal (iterative)

```cpp
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
```

### Post-order traversal (recursive)

```cpp
void postorder_recursive(bst_node* p)
{
  if (!p)
    return;

  postorder_recursive(p->left);
  postorder_recursive(p->right);
  visit(p);
}
```

### Post-order traversal (iterative)

```cpp
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
```

### Copy

Copies a binary search tree to another location

```cpp
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
```

## Searching

Some searching algorithms.

### Binary search (recursive)

```cpp
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
```

### Binary search

```cpp
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
```

### Binary search (STL)

In the STL the binary search requires only forward iterator so the
implementation above has to be changed.

```cpp
template<class Iter, class T>
bool binary_search_stl(Iter begin, Iter end, const T& K)
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
```

### Binary search (rotated)

This algorithm is sometimes asked in interview questions

```cpp
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
```

## Combinatorics

Some combinatorics algorithms.

### Next tuple

Calculates the next tuple based on the current one.  The first element
in the array is used for convenience and does not belong in the tuple.
Minimum and maximum values for each element in the tuple are passed in
paramenter min and max.

```cpp
template <class Iter>
auto next_tuple(Iter begin, Iter end, Iter min, Iter max)
{
    auto j = end - begin - 1;
    while (begin[j] == max[j]) {
      begin[j] = min[j];
      --j;
    }
    ++begin[j];
    
    return j != 0;
}
```

Example usage

```cpp
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
```

### Next tuple (STL)

An implementation that is suitable for the STL can be seen below

```cpp
template <class Iter>
auto next_tuple(Iter begin, Iter end, Iter min, Iter max)
{
    auto j = end - begin - 1;
    while (begin[j] == max[j]) {
      begin[j] = min[j];
      --j;
    }
    ++begin[j];
    
    return j != 0;
}
```

### Next combination

```cpp
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
```

Example usage

```cpp
void all_combinations()
{
  int n = 5;
  std::vector<int> v {0, 1, 2, n, 0};

  do {
    visit_combination(v);
  } while (next_combination(v));
}
```
### Permutations

Some algorithms that deal with permutations

#### Next permutation

```cpp
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
```

Example usage

```cpp
void all_permutations()
{
  std::vector<int> v{0, 1, 2, 3, 4};
  do {
    visit_permutation(v);
  } while (rt::next_permutation(std::begin(v), std::end(v)));
}
```

#### Permute

```cpp
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
```

#### Unpermute

```cpp
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
```

#### Unpermute (on the fly)

```cpp

const auto index = [=](auto i)
{ return c - 1 - (i % c) + c * (i / c); };

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
```

#### Unpermute (on the fly with bit)

```cpp
constexpr auto bit = 1 << 30;
auto set_bit = [=](auto& o) {o |= bit;};
auto unset_bit = [=](auto& o) {o &= ~bit;};
auto is_set = [=](auto& o) {return o & bit;};

template <class Iter>
void unpermute_on_the_fly_bit(Iter begin, Iter end)
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
```

#### Random permutation

Generates a random permutation with elements in the range [1, n].

```cpp
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
```

#### Shuffle

Shuffles the input array. Expects a parameter rand that produces
random numbers uniformily distributed in the interval [0, 1).

```cpp
template <class Rand>
void shuffle(std::vector<int>& v, Rand& rand)
{
  auto j = v.size();
  do {
    auto k = static_cast<int>(j * rand());
    std::swap(v[k], v[--j]);
  } while (j != 0);
}
```

### Partition

The algorithm to calculate the next partition can be found in the
source code, here I will show how to calculate them in a loop

```cpp
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
```

## Sort

Some elementary sorting algorithms

## Bubble sort

```cpp
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
```

#### Comparison counting sort

```cpp
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
```

#### Comparison counting sort (inplace)

```cpp
template <class Iter>
void inplace_comparison_counting_sort(Iter begin, Iter end)
{
  auto table = calc_address_table(begin, end);
  unpermute(begin, end, std::begin(table));
}
```

#### Distribution counting sort

```cpp
template <class Iter>
void dist_counting_sort(Iter begin, Iter end, int min, int max)
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
```

#### Straight insertion sort

```cpp
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
```

#### Binary insertion sort

```cpp
template <class Iter>
void binary_insertion(Iter begin, Iter end)
{
  for (auto pos = begin; pos != end; ++pos)
    std::rotate( std::upper_bound(begin, pos, *pos)
               , pos
               , std::next(pos));
}
```

## Interview questions

### Inverval map

This data structure came up in an interview

```cpp
template<class K, class V>
struct interval_map {
  std::map<K,V> map;
  interval_map(V v = {}) : map({{std::numeric_limits<K>::min(), v}}) {}

  void assign(const K& a, const K& b, const V& v)
  {
    auto comp = map.key_comp();

    if (!comp(a, b))
      return;

    auto lbb = map.lower_bound(b);
    auto vb = lbb == std::end(map) ? map.rbegin()->second
            : comp(b, lbb->first)  ? std::prev(lbb)->second
            :                        lbb->second;

    if (v == vb)
      return;

    auto lba = map.lower_bound(a);
    auto va = lba == std::end(map)   ? map.rbegin()->second
            : lba == std::begin(map) ? lba->second
            :                          std::prev(lba)->second;

    if (v == va && lba != std::begin(map))
      return;

    map.erase( std::next(map.insert_or_assign(lba, a, v))
             , map.insert_or_assign(lbb, b, vb));
  }

  const V& operator[](K const& key) const
  { return (--map.upper_bound(key))->second; }
};
```

