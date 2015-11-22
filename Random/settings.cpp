#include <boost/hana.hpp>
#include <iostream>
#include <cassert>
#include <type_traits>

namespace hana = boost::hana;
using namespace hana::literals;

template <typename T>
constexpr auto is_unique(T x)
{
    return hana::unique(x) == x;
}

static_assert(is_unique(hana::tuple_t<>), "");
static_assert(is_unique(hana::tuple_t<int>), "");
static_assert(!is_unique(hana::tuple_t<int, int>), "");

enum class setting0 : int
{
    red = 0,
    blue = 1,
    green = 2
};

enum class setting1 : int
{
    single = 0,
    multiple = 1,
    mixed = 2
};

enum class setting2 : int
{
    on = 0,
    off = 1
};

template <typename... Ts>
struct settings_set : hana::tuple<Ts...>
{
    template <typename T>
    constexpr auto get()
    {
        return static_cast<T>(decltype(hana::make_map(

            hana::make_pair(hana::type_c<typename Ts::enum_type>,
                typename Ts::ic_value{})...)[hana::type_c<T>])::value);
        /*
        return hana::find_if(hana::tuple<Ts...>{}, [](auto a)
            {
                return hana::bool_c<
                    std::is_same<typename decltype(a)::enum_type, T>{}>;
            });
*/
    }
};

template <typename... Ts>
constexpr auto make_settings(Ts...)
{
    static_assert(is_unique(hana::tuple_t<Ts...>), "");
    return settings_set<Ts...>{};
}


template <typename T>
constexpr auto val(T t)
{
    return static_cast<std::underlying_type_t<T>>(t);
}

template <typename T, T TV>
struct enum_w
{
    using enum_type = T;
    using ic_value = std::integral_constant<std::underlying_type_t<T>,
        (std::underlying_type_t<T>)TV>;
};



int main()
{
// static_assert(setting1::single == setting1::single, "");
// static_assert((int)setting1::single == 0, "");
// static_assert(get_inner_value<setting1, setting1::single>() == 0, "");
// static_assert(
//    get_inner_value_helper(setting1::single) == 0, "");
/*
    static_assert(std::is_same<decltype(setting1::single), setting1>{}, "");

    using test_w = enum_w<setting1, setting1::single>;
    static_assert(std::is_same<test_w::enum_type, setting1>{}, "");
    static_assert(
        std::is_same<test_w::ic_value, std::integral_constant<int, 0>>{},
   "");
*/


#define FCK(x)             \
    enum_w<decltype(x), x> \
    {                      \
    }


    auto s = make_settings(
        FCK(setting1::single), FCK(setting0::red), FCK(setting2::off));

    static_assert(s.get<setting1>() == setting1::single, "");
    static_assert(s.get<setting1>() != setting1::mixed, "");
    // static_assert(s.get<setting1>() == hana::nothing, "");

    // using f2 = enum_ic2<setting1::single>;

    // auto s = make_settings(setting1::single, setting0::red,
    // setting2::off);
}