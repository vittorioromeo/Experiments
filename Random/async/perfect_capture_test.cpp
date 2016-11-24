#include "perfect_capture.hpp"

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
    auto testfwder = [](auto&&... xs)
    {
        return [args = fwd_capture_as_tuple(FWD(xs)...)]{

        };
    };

    nomove nm0;
    nomove nm1;
    nomove nm2;

    testfwder(nocopy{}, nocopy{}, nocopy{});
    testfwder(nm0, nm1, nm2);
    testfwder(nocopy{}, nm0, nm1, nocopy{}, nm2, nocopy{});

    {
        auto test = [](auto&& x){ return [x = FWD(x)]{}; };
        test(nocopy{});
        nocopy j;
        // test(j); // does not compile as intended
    }

    {
        auto test2 = [](auto&& x){ return [x = fwd_capture(x)]{}; };
        test2(nocopy{});
        nocopy j;
        test2(j); // compiles (stores a reference wrapper)
    }

    {
        // A copy of l is made

        auto l = [i = 0]() mutable { ++i; return i; };
        auto test = [](auto&& x){ return [x = FWD(x)]() mutable { return x(); }; };

        auto bl = test(l);
        assert(bl() == 1);
        assert(l() == 1);
        assert(bl() == 2);
        assert(l() == 2);
        assert(bl() == 3);
        assert(l() == 3);
    }

    {
        // Reference is preserved

        auto l = [i = 0]() mutable { ++i; return i; };
        auto test = [](auto&& x){ return [x = fwd_capture(FWD(x))]() mutable { return x.get()(); }; };

        auto bl = test(l);
        assert(bl() == 1);
        assert(l() == 2);
        assert(bl() == 3);
        assert(l() == 4);
        assert(bl() == 5);
        assert(l() == 6);
    }

    {
        auto copy = [](auto&& x) -> auto { return x; };

        // A copy is made

        auto l = [i = 0]() mutable { ++i; return i; };
        auto test = [](auto&& x){ return [x = fwd_capture(FWD(x))]() mutable { return x.get()(); }; };

        auto bl = test(copy(l));
        assert(bl() == 1);
        assert(l() == 1);
        assert(bl() == 2);
        assert(l() == 2);
        assert(bl() == 3);
        assert(l() == 3);
    }

    {
        // Constness
        int a = 0;
        const int& aref = a;
        auto test = [aa = fwd_capture(aref)]() mutable { /*++(aa.get());*/ };
        test();
        assert(a == 0);
    }
}
