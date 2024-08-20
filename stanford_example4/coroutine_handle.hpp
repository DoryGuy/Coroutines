/*
 * https://www.scs.stanford.edu/~dm/blog/c++-coroutines.html
 */

#include <coroutine>

// guarantee destroy is called on the coroutine_handle
template<typename Promise>
class CoRoutineHandle {
    std::coroutine_handle<Promise> m_h;
    bool m_destroy = false;
public:
    CoRoutineHandle() = default;
    CoRoutineHandle(std::coroutine_handle<Promise> h) 
       : m_h(h)
       , m_destroy(true)
       {}
    ~CoRoutineHandle() {
        try {
            if ( m_destroy ) {
                m_h.destroy();
            }
        }
        catch(...) {} // don't terminate.
    }

    void operator()() { return m_h(); } 
    [[nodiscard]] Promise& promise() const { return m_h.promise(); }
    [[nodiscard]] bool done() const { return m_h.done(); } 

    // we assume if you got the pointer, this is attached to a coroutine;
    [[nodiscard]] std::coroutine_handle<Promise>* get() { m_destroy = true; return &m_h; }


    // no copies
    CoRoutineHandle(CoRoutineHandle &) = delete;
    CoRoutineHandle(CoRoutineHandle const &) = delete;
    CoRoutineHandle &operator=(CoRoutineHandle &) = delete;
    CoRoutineHandle &operator=(CoRoutineHandle const &) = delete;

    // move is allowed
    [[nodiscard]] constexpr CoRoutineHandle(CoRoutineHandle &&) = default;
    [[nodiscard]] constexpr CoRoutineHandle& operator=(CoRoutineHandle &&) = default;
};


template<>
class CoRoutineHandle<void> {
    std::coroutine_handle<> m_h;
    bool m_destroy = false;
public:
    CoRoutineHandle() = default;
    CoRoutineHandle(std::coroutine_handle<> h) 
       : m_h(h)
       , m_destroy(true)
       {}
    ~CoRoutineHandle() {
        try {
            if ( m_destroy ) {
                m_h.destroy();
            }
        }
        catch(...) {} // don't terminate.
    }

    void operator()() { return m_h(); } 
    [[nodiscard]] bool done() const { return m_h.done(); } 

    // we assume if you got the pointer, this is attached to a coroutine;
    [[nodiscard]] std::coroutine_handle<>* get() { m_destroy = true; return &m_h; }


    // no copies
    CoRoutineHandle(CoRoutineHandle<void> &) = delete;
    CoRoutineHandle(CoRoutineHandle<void> const &) = delete;
    CoRoutineHandle<void> &operator=(CoRoutineHandle<void> &) = delete;
    CoRoutineHandle<void> &operator=(CoRoutineHandle<void> const &) = delete;

    // move is allowed
    [[nodiscard]] constexpr CoRoutineHandle(CoRoutineHandle<void> &&) = default;
    [[nodiscard]] constexpr CoRoutineHandle& operator=(CoRoutineHandle<void> &&) = default;
};

template<>
class CoRoutineHandle<std::noop_coroutine_promise> {
    std::coroutine_handle<std::noop_coroutine_promise> m_h;
    bool m_destroy = false;
public:
    CoRoutineHandle() = default;
    CoRoutineHandle(std::coroutine_handle<std::noop_coroutine_promise> h) 
       : m_h(h)
       , m_destroy(true)
       {}
    ~CoRoutineHandle() {
        try {
            if ( m_destroy ) {
                m_h.destroy();
            }
        }
        catch(...) {} // don't terminate.
    }

    void operator()() { return m_h(); } 
    [[nodiscard]] auto & promise() const noexcept { return m_h.promise(); }
    [[nodiscard]] constexpr bool done() const noexcept { return m_h.done(); } 

    // we assume if you got the pointer, this is attached to a coroutine;
    [[nodiscard]] std::coroutine_handle<std::noop_coroutine_promise>* get() { m_destroy = true; return &m_h; }


    // no copies
    CoRoutineHandle(CoRoutineHandle<std::noop_coroutine_promise> &) = delete;
    CoRoutineHandle(CoRoutineHandle<std::noop_coroutine_promise> const &) = delete;
    CoRoutineHandle<std::noop_coroutine_promise> &operator=(CoRoutineHandle<std::noop_coroutine_promise> &) = delete;
    CoRoutineHandle<std::noop_coroutine_promise> &operator=(CoRoutineHandle<std::noop_coroutine_promise> const &) = delete;

    // move is allowed
    [[nodiscard]] constexpr CoRoutineHandle(CoRoutineHandle<std::noop_coroutine_promise> &&) = default;
    [[nodiscard]] constexpr CoRoutineHandle& operator=(CoRoutineHandle<std::noop_coroutine_promise> &&) = default;
};
