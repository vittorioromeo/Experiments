// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#include "./shared.hpp"
#include "./legacy.hpp"
#include "./behavior.hpp"
#include "./unique_resource.hpp"
#include "./shared_resource.hpp"
#include "./make_resource.hpp"
#include "./access.hpp"
#include "./interface.hpp"
#include "./tests.hpp"
#include "./tests_runner.hpp"

template <typename TAccess>
struct vbo_interface : TAccess
{
    using TAccess::TAccess;

    void my_interface_method_0()
    {
        // something(_access.handle());
    }
};

using unmanaged_vbo = vbo_interface<access::unmanaged<behavior::vbo_b>>;
using unique_vbo = vbo_interface<access::unique<behavior::vbo_b>>;
using shared_vbo = vbo_interface<access::shared<behavior::vbo_b>>;

template <typename... Ts>
auto make_unmanaged_vbo(Ts&&... xs)
{
    return make_unmanaged_interface<behavior::vbo_b, vbo_interface>(FWD(xs)...);
}

template <typename... Ts>
auto make_unique_vbo(Ts&&... xs)
{
    return make_unique_interface<behavior::vbo_b, vbo_interface>(FWD(xs)...);
}

template <typename... Ts>
auto make_shared_vbo(Ts&&... xs)
{
    return make_shared_interface<behavior::vbo_b, vbo_interface>(FWD(xs)...);
}


int main()
{
    test::run_all();

    /*
    {
        auto x_vbo =
            make_unmanaged_interface<behavior::vbo_b, vbo_interface>(1);
        x_vbo.my_interface_method_0();
    }

    {
        auto u_vbo = make_unique_interface<behavior::vbo_b, vbo_interface>(1);
        u_vbo.my_interface_method_0();
    }

    {
        auto s_vbo = make_shared_interface<behavior::vbo_b, vbo_interface>(1);
        s_vbo.my_interface_method_0();
    }
    */

    // Unmanaged
    {
        auto x = make_unmanaged_vbo(1);
        x.my_interface_method_0();

        x.dispose();
    }

    // Unique
    {
        auto x = make_unique_vbo(1);
        x.my_interface_method_0();
        assert(x);

        // Does not compile, as intended:
        // auto y = x;

        // OK:
        auto y = std::move(x);
        y.my_interface_method_0();
        assert(y);
        assert(!x);
    }

    // Shared
    {
        auto x = make_shared_vbo(1);
        x.my_interface_method_0();
        assert(x);

        {
            // OK:
            auto y = x;
            assert(x);
            assert(y);

            auto z = x;
            assert(x);
            assert(y);
            assert(z);

            z.reset();

            assert(x);
            assert(y);
            assert(!z);
        }

        assert(x);
    }

    return 0;
}

// TODO:
// ??
// "interface" -> "wrapper"
// SFML RAII classes as resources
// does polymorphism work with unique and/or shared?
// sanitize tests with address and memory
// std::hash?
// thread-safe sptr
// weak_resource