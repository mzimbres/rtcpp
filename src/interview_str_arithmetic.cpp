#include <iostream>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <functional>
#include <stack>
#include <string>
#include <stack>

void visit(std::stack<int> stack)
{
  while (!stack.empty()) {
    std::cout << stack.top() << " ";
    stack.pop();
  }
  std::cout << "\n";
}

template <class C>
void handle_op(std::stack<int>& stack, C c)
{
  if (stack.size() < 2)
    return;

  auto a = stack.top();
  stack.pop();
  auto b = stack.top();
  stack.pop();
  stack.push(c(a, b));
}

void iterative(const std::string& s)
{
  using iter_type = std::istream_iterator<std::string>;

  std::stringstream ss(s);

  std::stack<int> stack;
  auto f = [&](const auto& o)
  {
    if (o == "+")
      handle_op(stack, std::plus<int>{});
    else if (o == "-")
      handle_op(stack, std::minus<int>{});
    else 
      stack.push(std::stoi(o));
  };

  std::for_each(iter_type{ss}, iter_type{}, f);
  visit(stack);
}

void recursive( std::stack<int> stack
              , std::istream_iterator<std::string> iter)
{
  using iter_type = std::istream_iterator<std::string>;

  if (iter == iter_type {}) {
    visit(stack);
    return;
  }

  auto s = *iter;
  if (s == "+")
    handle_op(stack, std::plus<int>{});
  else if (s == "-")
    handle_op(stack, std::minus<int>{});
  else
    stack.push(std::stoi(s));

  recursive(std::move(stack), ++iter);
}

int main()
{
  iterative("1 13 - 6 + 9 8 +");
  using iter_type = std::istream_iterator<std::string>;
  std::istringstream ss("1 13 - 6 + 9 8 +");
  iter_type iter(ss);
  recursive(std::stack<int>{}, iter);
}

