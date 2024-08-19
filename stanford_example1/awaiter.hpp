#ifndef __AWAITER_HPP
#define __AWAITER_HPP
/*
 * https://www.scs.stanford.edu/~dm/blog/c++-coroutines.html
 */

#include <coroutine>


typedef enum { DONT_WAIT = false,
         WAIT = true } WAIT_TYPE;

template<WAIT_TYPE wait>
class Awaiter {
  std::coroutine_handle<> *hp_;
public:
  Awaiter(std::coroutine_handle<> *h) : hp_(h) {}

  [[nodiscard]] constexpr bool await_ready() const noexcept { return false; }
  bool await_suspend(std::coroutine_handle<> h) { 
      if (hp_ ) {
          *hp_ = h;
          hp_ = nullptr;
      }
      return wait;
  }

  // returns the same type as the call to co_await!
  // in this case "void"
  constexpr void await_resume() const noexcept {}

  Awaiter(Awaiter &) = delete;  // copies are not allowed.
  Awaiter(Awaiter const &) = delete;
  Awaiter &operator=(Awaiter &) = delete;
  Awaiter &operator=(Awaiter const &) = delete;

  [[nodiscard]] constexpr Awaiter(Awaiter &&) noexcept = default; // move is allowed.
  [[nodiscard]] constexpr Awaiter& operator=(Awaiter &&) noexcept = default;
};

#endif  // AWAITER_HPP
