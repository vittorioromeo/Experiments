#pragma once

#include <thread>
#include <mutex>
#include <mingw.thread.h>
#include <mingw.mutex.h>
#include <mingw.condition_variable.h>
#include <ecst/utils.hpp>
#include <utility>
#include <type_traits>

#define STATIC_ASSERT_SAME_TYPE(T0, T1) static_assert(std::is_same<T0, T1>{})
#define STATIC_ASSERT_SAME(a, T) STATIC_ASSERT_SAME_TYPE(decltype(a), T)

STATIC_ASSERT_SAME(FWD(std::declval<int>()), int&&);
STATIC_ASSERT_SAME(FWD(std::declval<int&>()), int&);
STATIC_ASSERT_SAME(FWD(std::declval<int&&>()), int&&);

template <typename T>
struct value_or_rvalue { using type = T; };

template <typename T>
struct value_or_rvalue<T&&> { using type = T; };

template <typename T>
using value_or_rvalue_t = typename value_or_rvalue<T>::type;

STATIC_ASSERT_SAME_TYPE(value_or_rvalue_t<int>, int);
STATIC_ASSERT_SAME_TYPE(value_or_rvalue_t<int&>, int&);
STATIC_ASSERT_SAME_TYPE(value_or_rvalue_t<int&&>, int);

template <typename T>
struct perfect_capture
{
    static_assert(!std::is_rvalue_reference<T>{});
    static_assert(!std::is_lvalue_reference<T>{});

private:
    T _x;

public:
    perfect_capture(T&& x) : _x{std::move(x)} { }

    perfect_capture(perfect_capture&& rhs) : _x{std::move(rhs._x)}
    {
    }

    perfect_capture& operator=(perfect_capture&& rhs)
    {
        _x = std::move(rhs._x);
        return *this;
    }

    // Prevent copies.
    perfect_capture(const perfect_capture&) = delete;
    perfect_capture& operator=(const perfect_capture&) = delete;

    operator T&&() { return std::move(_x); }
    operator T&&() const { return std::move(_x); }
};

template <typename T>
struct perfect_capture<T&>
{
    static_assert(!std::is_rvalue_reference<T>{});

private:
    T& _x;

public:
    perfect_capture(T& x) : _x{x} { }

    perfect_capture(perfect_capture&& rhs) : _x{rhs._x}
    {
    }

    perfect_capture& operator=(perfect_capture&& rhs)
    {
        _x = rhs._x;
        return *this;
    }

    // Prevent copies.
    perfect_capture(const perfect_capture&) = delete;
    perfect_capture& operator=(const perfect_capture&) = delete;

    operator T&() noexcept { return _x; }
    operator const T&() const noexcept { return _x; }
};

template <typename T>
auto pcap(T&& x)
{
    return perfect_capture<T>(FWD(x));
}

STATIC_ASSERT_SAME(pcap(1), perfect_capture<int>);
STATIC_ASSERT_SAME(pcap(std::declval<int>()), perfect_capture<int>);
STATIC_ASSERT_SAME(pcap(std::declval<int&>()), perfect_capture<int&>);
STATIC_ASSERT_SAME(pcap(std::declval<int&&>()), perfect_capture<int>);

#if defined(STATIC_TESTS)
namespace
{
    void  test0()[[maybe_unused]]
    {
        int x;
        auto p = pcap(x);
        STATIC_ASSERT_SAME(p, perfect_capture<int&>);
    }

    void test1()[[maybe_unused]]
    {
        int x;
        auto p = pcap(std::move(x));
        STATIC_ASSERT_SAME(p, perfect_capture<int>);
    }

    void test2()[[maybe_unused]]
    {
        auto p = pcap(1);
        STATIC_ASSERT_SAME(p, perfect_capture<int>);
    }
}
#endif
