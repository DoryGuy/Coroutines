/*
 * https://www.scs.stanford.edu/~dm/blog/c++-coroutines.html
 */

#include "coroutine_handle.hpp"
#include "return_object.hpp"

#include <iostream>

ReturnObject<std::string>
counter()
{
  //for (double i = 0;; ++i)
  //  co_yield i;       // co yield i => co_await promise.yield_value(i)
  std::cout << "call counter 1" << std::endl;
  co_yield std::string("first call");
  std::cout << "call counter 2" << std::endl;
  co_yield std::string("second call");
  std::cout << "call counter 3" << std::endl;
  co_yield std::string("third call");
  std::cout << "call counter 4" << std::endl;
  co_yield std::string("fourth call");
}

int
main()
{
  CoRoutineHandle<ReturnObject<std::string>::promise_type>  h(counter().get());
  auto &promise = h.promise();
  for (int i = 0; i < 3  ; ++i) {
    std::cout << "counter4: " << promise.value_ << std::endl;
    h();
  }

  return 0;
}

