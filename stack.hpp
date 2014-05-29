#include <utility>
#include <iterator>

template <typename T, unsigned N>
class stack {
  static const unsigned static_size = N;
private:
  T pool[static_size];
  T* p;
public:
  int size() const {return p ? p - &pool[0] + 1 : 0;}
  bool push(const T& val)
  {
    if (!p) {
      p = &pool[0];
      *p = val;
      return true;
    }

    if (p == &pool[static_size - 1])
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
  T top() const {return *p;} // Undefined if stack is empty.
  stack()
  : p(0)
  {}
  stack(const stack<T,N>& rhs)
  {
    std::copy(&rhs.pool[0], &rhs.pool[0] + N, &pool[0]);
    const int d = std::distance(&rhs.pool[0], static_cast<const int*>(rhs.p));
    p = &pool[d];
  }
  const stack<T, N>& operator=(const stack<T,N>& rhs)
  {
    if (this != &rhs) {
      std::copy(&rhs.pool[0], &rhs.pool[0] + N, &pool[0]);
      const int d = std::distance(&rhs.pool[0], static_cast<const int*>(rhs.p));
      p = &pool[d];
    }
  }
};

