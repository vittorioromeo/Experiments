#include "perfect_capture.hpp"
#include "nothing.hpp"

struct nocopy
{
    nocopy() = default;

    nocopy(const nocopy&) = delete;
    nocopy& operator=(const nocopy&) = delete;

    nocopy(nocopy&&) = default;
    nocopy& operator=(nocopy&&) = default;
};

struct nomove
{
    nomove() = default;

    nomove(const nomove&) = default;
    nomove& operator=(const nomove&) = default;

    nomove(nomove&&) = delete;
    nomove& operator=(nomove&&) = delete;
};

int main()
{
    using namespace ll;

    auto expects_int = [](int x){ return x; };
    assert( call_ignoring_nothing(expects_int, 0) == 0 );
    assert( call_ignoring_nothing(expects_int, nothing, 1) == 1 );
    assert( call_ignoring_nothing(expects_int, 2, nothing) == 2 );
    assert( call_ignoring_nothing(expects_int, nothing, 3, nothing) == 3 );
    assert( call_ignoring_nothing(expects_int, nothing, nothing, 4, nothing) == 4 );

    auto returns_void = [](auto&&...){};
    // assert( is_nothing(with_void_to_nothing(returns_void)) ); // TODO: gcc bug?
    assert( is_nothing(with_void_to_nothing(returns_void, 0)) );
    assert( is_nothing(with_void_to_nothing(returns_void, 0, 1)) );
    assert( is_nothing(with_void_to_nothing(returns_void, 0, 1, 2)) );
    // assert( is_nothing(with_void_to_nothing(returns_void, nothing)) ); // TODO: gcc bug?

    // should be perfectly-captured, so it's a reference to `returns_void`.
    auto wvtn_returs_void = bind_return_void_to_nothing(returns_void);
    // assert( is_nothing(wvtn_returs_void()) ); // TODO: as above?
    assert( is_nothing(wvtn_returs_void(0)) );
    assert( is_nothing(wvtn_returs_void(0, 1)) );
    assert( is_nothing(wvtn_returs_void(0, 1, 2)) );
    // assert( is_nothing(wvtn_returs_void(nothing)) ); // TODO: as above?
}
