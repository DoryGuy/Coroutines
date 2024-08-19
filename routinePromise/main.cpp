/*
 * https://simontoth.substack.com/p/daily-bite-of-c-coroutines-step-by
 */

#include <coroutine>
#include <exception>
#include <iostream>
#include <print>

struct Routine {

    struct Promise {
        std::exception_ptr exception_;

        // This function is used to create the instance
        // of the return type for the caller
        Routine get_return_object() { return {}; }

        // What should happen before the coroutine body starts
        std::suspend_never initial_suspend() noexcept { return {}; }
        // What should happen after the coroutine body has finished
        std::suspend_never final_suspend() noexcept { return {}; }
        // What should happen when the coroutine executes co_return;
        void return_void() {}
        // What should happen when there is an unhandled exception
        void unhandled_exception() {exception_ = std::current_exception();}
    };

    using promise_type = Promise;
};

int main() {
    auto coro = [] -> Routine {
        std::println("Running...");
        co_return;
    };

    auto x = coro(); // coroutine starts and runs to completion
    // decltype(x) == Routine
    static_assert(std::is_same_v<decltype(x), Routine>);

    coro(); // Because the return type is empty, this is the same as above
}
