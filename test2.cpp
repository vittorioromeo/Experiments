#include <iostream>

struct foo
{
    int x{42};
};

template <typename F>
struct impl : F
{
    impl(F&& f) : F{std::move(f)}
    {
    }

    auto get()
    {
        return (*this)();
    }
};

template <typename X>
auto returner(X&& x)
{
    return impl{[&x] { return x; }};
}

int main()
{
    auto x = returner(foo{}).get();
}
