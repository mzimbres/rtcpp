#include <vector>
#include <iterator>

namespace rt {

template <typename Set, typename T>
std::vector<char*> heap_frag(std::size_t n_between, const std::vector<T>& data)
{
  // Cannot use smart pointers here.
  Set holes;
  std::vector<char*> data2;
  std::vector<char*> holes2;
  for (auto iter = std::begin(data); iter != std::end(data); ++iter) {
    for (std::size_t i = 0; i < n_between; ++i) {
      data2.push_back(new char);
      holes2.push_back(new char);
    }
    holes.insert(*iter);
  }

  for (auto iter = std::begin(holes2); iter != std::end(holes2); ++iter)
    delete *iter;

  return data2; // s2 are destructed leaving many holes in the heap.
}

}

