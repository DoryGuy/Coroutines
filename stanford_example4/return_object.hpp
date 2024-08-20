/*
 * https://www.scs.stanford.edu/~dm/blog/c++-coroutines.html
 */

#include <utility>

template<typename valType>
class ReturnObject {
public:
  struct Promise;
  using promise_type = Promise;
private:
  std::coroutine_handle<Promise> h_;
public:

    struct Promise {
        valType  value_;

        ReturnObject get_return_object() {
          return ReturnObject(std::coroutine_handle<Promise>::from_promise(*this));
        }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void unhandled_exception() {}
        std::suspend_always yield_value(valType value) {
          value_ = value;
          return {};
        }
        void return_void() {}
  };

  ReturnObject(std::coroutine_handle<Promise> p) 
    : h_(p) {}

  [[nodiscard]] operator std::coroutine_handle<Promise>() const { return h_; }
  // A coroutine_handle<Promise> converts to coroutine_handle<>
  [[nodiscard]] operator std::coroutine_handle<>() const { return h_; }

  [[nodiscard]] std::coroutine_handle<Promise> get() const { return h_; }

  // no copies
  ReturnObject(ReturnObject &) = delete;
  ReturnObject(ReturnObject const &) = delete;
  [[nodiscard]] ReturnObject &operator=(ReturnObject &) = delete;
  [[nodiscard]] ReturnObject &operator=(ReturnObject const &) = delete;

  // move is allowed
  [[nodiscard]] constexpr ReturnObject(ReturnObject &&) = default;
  [[nodiscard]] constexpr ReturnObject &operator=(ReturnObject &&) = default;
};


template<>
class ReturnObject<void> {
public:
  struct Promise;
  using promise_type = Promise;
private:
  std::coroutine_handle<Promise> h_;
public:

    struct Promise {
        ReturnObject<void> get_return_object(){
          return ReturnObject<void>(std::coroutine_handle<Promise>::from_promise(*this));
        }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void unhandled_exception() {}
        void return_void() {}
  };

  ReturnObject(std::coroutine_handle<Promise> p )
      : h_(p) {}

  [[nodiscard]] operator std::coroutine_handle<Promise>() const { return h_; }
  // A coroutine_handle<Promise> converts to coroutine_handle<>
  [[nodiscard]] operator std::coroutine_handle<>() const { return h_; }

  // no copies
  ReturnObject(ReturnObject<void> &) = delete;
  ReturnObject(ReturnObject<void> const &) = delete;
  [[nodiscard]] ReturnObject &operator=(ReturnObject<void> &) = delete;
  [[nodiscard]] ReturnObject &operator=(ReturnObject<void> const &) = delete;

  // move is allowed
  [[nodiscard]] constexpr ReturnObject(ReturnObject<void> &&) = default;
  [[nodiscard]] constexpr ReturnObject &operator=(ReturnObject<void> &&) = default;
};
