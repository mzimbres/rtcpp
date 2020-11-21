#include <assert.h>
#include <map>
#include <utility>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <limits>
#include <stdexcept>
#include <algorithm>
#include <random>

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

template <class Key, class Value>
void print(const interval_map<Key, Value>& m)
{
  for (const auto& item : m.map)
    std::cout << std::setw(3) << item.first << " " << item.second << std::endl;
}

template <class K, class V>
auto check_valid_map(const std::map<K, V>& map)
{
  auto func = [](const auto& a, const auto& b)
  {return a.second == b.second;};

  return std::adjacent_find( std::cbegin(map), std::cend(map)
                           , func) == std::cend(map);
}
bool test1()
{
  std::mt19937 gen {};

  std::uniform_int_distribution<int> dis1(0, 50);
  std::uniform_int_distribution<int> dis2(0, 50);

  interval_map<unsigned, int> map;
  for (auto j = 0; j < 20; ++j) {
    map = {};
    for (auto i = 0; i < 10000; ++i) {
      const auto a = dis2(gen);
      const auto b = dis2(gen);
      const auto c = dis1(gen);
      auto map_copy = map.map;
      map.assign(a, b, c);
    }

    if (!check_valid_map(map.map)) {
      print(map);
      return false;
    }
  }
  print(map);
  return true;
}

bool test2()
{
  interval_map<unsigned, char> map {'a'};

  std::map<unsigned, char> m1 {{0, 'a'}};
  if (map.map != m1) {
    std::cout << "T1 failed." << std::endl;
    return false;
  }

  map.assign(3, 8, 'b');

  m1 = {{0, 'a'}, {3, 'b'}, {8, 'a'}};
  if (map.map != m1) {
    std::cout << "T2 failed." << std::endl;
    return false;
  }

  map.assign(5, 9, 'c');

  m1 = {{0, 'a'}, {3, 'b'}, {5, 'c'}, {9, 'a'}};
  if (map.map != m1) {
    std::cout << "T3 failed." << std::endl;
    return false;
  }

  map.assign(2, 10, 'd');

  m1 = {{0, 'a'}, {2, 'd'}, {10, 'a'}};
  if (map.map != m1) {
    std::cout << "T4 failed." << std::endl;
    return false;
  }
  return true;
}


int main()
{
  try {
    if (!test1())
      return 1;

    if (!test2())
      return 1;

    std::cout << "Tests succeed" << std::endl;
  } catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
  }
  return 0;
}

