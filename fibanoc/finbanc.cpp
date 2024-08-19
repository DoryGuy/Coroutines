/*
 * https://simontoth.substack.com/p/daily-bite-of-c-coroutines-step-by
 */

#include <generator>
#include <print>
#include <utility>

// The return type has to conform to coroutine requirements
std::generator<uint64_t> fibonacci(int cnt) {
    uint64_t a = 0;
    uint64_t b = 1;
    for (;cnt > 0; --cnt)
        // and the function must use at least one coroutine keyword
        co_yield std::exchange(a, std::exchange(b, a+b));
    // This coroutine naturally ends after the loop is exhausted
    // but we can also co_return to exit immediately.
}

int main() {
    int i{1};
    for (int v : fibonacci(49)) {
        std::println("{} {}", i, v);
        ++i;
    }
}
