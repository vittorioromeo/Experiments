#include <cstdio>
#include <type_traits>
#include <utility>
#include <exception>

struct s
{
    s()         { std::printf("    s()\n"); }
    s(const s&) { std::printf("    s(const s&)\n"); }
    s(s&&)      { std::printf("    s(s&&)\n"); }
};

template <typename F>
struct on_scope_success : F
{
    int _uncaught{std::uncaught_exceptions()};

    on_scope_success(F&& f) : F{std::forward<F>(f)} { }

    ~on_scope_success()
    {
        if(_uncaught < std::uncaught_exceptions()) {
            (*this)();
        }
    }
};

auto invoke_log_return = []<typename F>(F&& f) -> decltype(auto)
{
    decltype(auto) result{std::forward<F>(f)()};
    std::printf("    ...logging here...\n");

    if constexpr(std::is_reference_v<decltype(result)>)
    {
        return decltype(result)(result);
    }
    else
    {
        return result;
    }
};

auto invoke_log_return_scope = []<typename F>(F&& f) -> decltype(auto)
{
    on_scope_success _{[]{ std::printf("    ...logging here...\n"); }};
    return std::forward<F>(f)();
};

s s_instance;

auto make_prvalue =   []() -> s   { return {}; };
auto make_lvalue  =   []() -> s&  { return s_instance; };
auto make_xvalue  =   []() -> s&& { return std::move(s_instance); };
auto make_exception = []() -> s   { throw 0; };

int main()
{
    std::printf("in main\n\n");

    auto do_test = []<typename F>(auto name, F&& f)
    {
        std::printf("%s\n", name);
        try
        {
            [[maybe_unused]] auto result = std::forward<F>(f)();
        }
        catch(...)
        {
            std::printf("    caught exception");
        }
        std::printf("\n");
    };

    auto do_all_tests = [&](auto&& f)
    {
        do_test("prvalue",   [&]{ return f(make_prvalue); });
        do_test("lvalue",    [&]{ return f(make_lvalue); });
        do_test("xvalue",    [&]{ return f(make_xvalue); });
        do_test("exception", [&]{ return f(make_exception); });
    };

    do_all_tests(invoke_log_return);
    std::printf("\n");
    do_all_tests(invoke_log_return_scope);
}
