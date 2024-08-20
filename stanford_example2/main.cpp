/*
 * https://www.scs.stanford.edu/~dm/blog/c++-coroutines.html
 */

#include "return_object.hpp"

#include <iostream>
#include <memory>

[[nodiscard]] ReturnObject<> counter2()
{
  for (unsigned i = 0;; ++i) {
    co_await std::suspend_always{};
    std::cout << "counter: " << i << std::endl;
  }
}

int
main()
{
  auto h(counter2());
  for (int i = 0; i < 3; ++i) {
    std::cout << "In main function\n";
    h();
  }

  return 0;
}
