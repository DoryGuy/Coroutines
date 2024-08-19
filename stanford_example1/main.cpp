/*
 * https://www.scs.stanford.edu/~dm/blog/c++-coroutines.html
 */

#include "awaiter.hpp"
#include "return_object.hpp"

#include <exception>
#include <iostream>


[[nodiscard]] ReturnObject<>
counter(std::coroutine_handle<> *continuation_out)
{
  Awaiter<WAIT> a{continuation_out};
  for (unsigned i = 0;; ++i) {
    std::cout << "Counter i = " << i << std::endl;
    co_await a;
  }
}

int main()
{
  std::coroutine_handle<> h;
  [[maybe_unused]]  auto ro = counter(&h);
  for (int i = 0; i < 3; ++i) {
        std::cout << "Main i = " << i  << std::endl;
        h();
  }

  return 0;
}
