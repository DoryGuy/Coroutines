/*
 * https://www.scs.stanford.edu/~dm/blog/c++-coroutines.html
 */

#include <concepts>
#include <coroutine>
#include <exception>
#include <utility>

// use this class for Generators.
template<typename T = void>
class ReturnObject {
public:
  struct Promise;
  using promise_type = Promise;
private:
  std::coroutine_handle<Promise> m_h;
  bool m_full = false;
public:

    struct Promise {
        T  value_;
        std::exception_ptr exception_;


        ReturnObject get_return_object() {
          return ReturnObject(std::coroutine_handle<Promise>::from_promise(*this));
        }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void unhandled_exception() {exception_ = std::current_exception();}
        
        template<std::convertible_to<T> From> // C++20 concept
        std::suspend_always yield_value(From &&value) {
          value_ = std::forward<From>(value);
          return {};
        }
        void return_void() {}
  };

   ReturnObject(std::coroutine_handle<Promise> p) 
    : m_h(p) {}

    ~ReturnObject() {
        m_h.destroy();
    }

    explicit operator bool() {
        fill();
        return !m_h.done();
    }

    T operator()() { 
        fill();
        m_full = false;
        return std::move(m_h.promise().value_);
    } 


  [[nodiscard]] operator std::coroutine_handle<Promise>() const { return m_h; }
  // A coroutine_handle<Promise> converts to coroutine_handle<>
  [[nodiscard]] operator std::coroutine_handle<>() const { return m_h; }

  [[nodiscard]] std::coroutine_handle<Promise> get() const { return m_h; }

  // no copies
  ReturnObject(ReturnObject &) = delete;
  ReturnObject(ReturnObject const &) = delete;
  [[nodiscard]] ReturnObject &operator=(ReturnObject &) = delete;
  [[nodiscard]] ReturnObject &operator=(ReturnObject const &) = delete;

  // move is allowed
  [[nodiscard]] constexpr ReturnObject(ReturnObject &&) = default;
  [[nodiscard]] constexpr ReturnObject &operator=(ReturnObject &&) = default;

private:
    void fill() { 
        if (!m_full) {
            m_h();
            if (m_h.promise().exception_ ) {
                std::rethrow_exception(m_h.promise().exception_);
            }
            m_full = true;
            return; 
        }
    } 
};


// Return Object for a general use case co_await;
//     eg:
//     co_await std::suspend_always{};
// destroys the coroutine handle when it goes out of scope.
template<>
class ReturnObject<void> {
public:
  struct Promise;
  using promise_type = Promise;
private:
  std::coroutine_handle<Promise> m_h;
public:

    struct Promise {
        std::exception_ptr exception_;

        ReturnObject<void> get_return_object(){
          return ReturnObject<void>(std::coroutine_handle<Promise>::from_promise(*this));
        }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void unhandled_exception() {exception_ = std::current_exception();}
        void return_void() {}
  };

  ReturnObject(std::coroutine_handle<Promise> p )
      : m_h(p) {}

  ~ReturnObject() { m_h.destroy(); }

  void operator()() { 
      m_h();
      if ( m_h.promise().exception_ ) {
          std::rethrow_exception(m_h.promise().exception_);
      } 
  }

  [[nodiscard]] operator bool(){
      return !m_h.done();
  }

  [[nodiscard]] operator std::coroutine_handle<Promise>() const { return m_h; }
  // A coroutine_handle<Promise> converts to coroutine_handle<>
  [[nodiscard]] operator std::coroutine_handle<>() const { return m_h; }

  // no copies
  ReturnObject(ReturnObject<void> &) = delete;
  ReturnObject(ReturnObject<void> const &) = delete;
  [[nodiscard]] ReturnObject &operator=(ReturnObject<void> &) = delete;
  [[nodiscard]] ReturnObject &operator=(ReturnObject<void> const &) = delete;

  // move is allowed
  [[nodiscard]] constexpr ReturnObject(ReturnObject<void> &&) = default;
  [[nodiscard]] constexpr ReturnObject &operator=(ReturnObject<void> &&) = default;
};
