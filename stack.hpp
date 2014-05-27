#include <utility>

template <typename T, unsigned N>
class stack {
public:
  static const unsigned size = N;
private:
  T pool[size];
  T* p;
public:
  int in_use() const { return p ? p - &pool[0] + 1 : 0; }
  bool push(T val)
  {
    if (!p) {
      p = &pool[0];
      *p = val;
      return true;
    }

    if (p == &pool[size - 1])
      return false;

    *++p = val;
  }
  void pop()
  {
    if (p == &pool[0]) {
      p = 0;
      return;
    }
    --p;
  }
  T top() const
  {
    if (p)
      return *p;
  }
  stack()
  : p(0)
  {}
};

