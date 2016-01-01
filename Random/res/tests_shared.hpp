// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./shared.hpp"
#include "./legacy.hpp"
#include "./behavior.hpp"
#include "./unique_resource.hpp"
#include "./shared_resource.hpp"
#include "./make_resource.hpp"
#include "./access.hpp"
#include "./interface.hpp"
#include "./tests.hpp"

namespace test
{
    // Null shared
    void shared_0()
    {
        assert_ck(0, 0);

        {
            shared_test u;
            assert_ck(0, 0);
        }

        assert_ck(0, 0);
    }

    // Null shared to owning
    void shared_1()
    {
        assert_ck(0, 0);

        {
            shared_test u;
            assert_ck(0, 0);

            u.reset(test_behavior::init());
            assert_ck(1, 0);
        }

        assert_ck(1, 1);
    }

    // Owning shared
    void shared_2()
    {
        assert_ck(0, 0);

        {
            shared_test u(test_behavior::init());
            assert_ck(1, 0);
        }

        assert_ck(1, 1);
    }

    // Owning shared to null shared
    void shared_3()
    {
        assert_ck(0, 0);

        {
            shared_test u(test_behavior::init());
            assert_ck(1, 0);

            u.reset();
            assert_ck(1, 1);
        }

        assert_ck(1, 1);
    }

    // Owning shared, to null unique (2)
    void shared_4()
    {
        assert_ck(0, 0);

        {
            shared_test u{test_behavior::init()};
            assert_ck(1, 0);

            u.reset(test_behavior::null_handle());
            assert_ck(1, 1);
        }

        assert_ck(1, 1);
    }

    // Owning shared to owning shared
    void shared_5()
    {
        assert_ck(0, 0);

        {
            shared_test u(test_behavior::init());
            assert_ck(1, 0);

            u.reset(test_behavior::init());
            assert_ck(2, 1);
        }

        assert_ck(2, 2);
    }

    // Shared ownership (1)
    void shared_6()
    {
        assert_ck(0, 0);

        {
            shared_test u(test_behavior::init());
            assert_ck(1, 0);

            shared_test u1{u};
            assert_ck(1, 0);

            shared_test u2{u};
            assert_ck(1, 0);
        }

        assert_ck(1, 1);
    }

    // Shared ownership (2)
    void shared_7()
    {
        assert_ck(0, 0);

        {
            shared_test u(test_behavior::init());
            assert_ck(1, 0);
            {
                shared_test u1{u};
                assert_ck(1, 0);
                {
                    shared_test u2{u};
                    assert_ck(1, 0);
                }
            }
        }

        assert_ck(1, 1);
    }

    // Random test
    void shared_8()
    {
        assert_ck(0, 0);

        {
            shared_test u(test_behavior::init());
            assert_ck(1, 0);

            {
                shared_test u1{u};
                assert_ck(1, 0);
            }
            assert_ck(1, 0);

            u.reset(test_behavior::init());
            assert_ck(2, 1);

            shared_test u2{u};
            assert_ck(2, 1);

            u.reset(test_behavior::init());
            assert_ck(3, 1);
        }

        assert_ck(3, 3);
    }

    // Random test
    void shared_9()
    {
        assert_ck(0, 0);

        {
            shared_test u(test_behavior::init());
            assert_ck(1, 0);

            {
                shared_test u1;
                u1 = u;
                assert_ck(1, 0);
            }
            assert_ck(1, 0);

            u.reset(test_behavior::init());
            assert_ck(2, 1);

            shared_test u2 = u;
            assert_ck(2, 1);

            shared_test u3 = u2;
            assert_ck(2, 1);

            u.reset(test_behavior::init());
            assert_ck(3, 1);
        }

        assert_ck(3, 3);
    }

    // TODO
    void shared_10()
    {
        {
            assert_ck(0, 0);

            auto h = test_behavior::init();
            assert_ck(1, 0);

            shared_test u(h);
            assert_ck(1, 0);

            assert(u.get() == h);
        }

        assert_ck(1, 1);
    }

    void shared_11()
    {
        {
            assert_ck(0, 0);

            auto u0 = shared_test(test_behavior::init());
            assert_ck(1, 0);

            auto u1(u0);
            assert_ck(1, 0);

            // TODO: fix
            // assert(u0 == u1);

            assert(u0.get() == u1.get());
        }

        assert_ck(1, 1);
    }

    void shared_12()
    {
        {
            assert_ck(0, 0);

            auto u0 = shared_test(test_behavior::init());
            assert_ck(1, 0);

            auto u1 = u0;
            assert_ck(1, 0);

            assert(u0.use_count() == 2);
            assert(u1.use_count() == 2);
        }

        assert_ck(1, 1);
    }

    void shared_13()
    {
        {
            assert_ck(0, 0);

            auto u0 = shared_test(test_behavior::init());
            assert_ck(1, 0);

            auto u1 = u0;
            assert_ck(1, 0);

            assert(u0.get() == u1.get());
        }

        assert_ck(1, 1);
    }

    void shared_14()
    {
        {
            assert_ck(0, 0);

            auto u0 = shared_test(test_behavior::init());
            assert_ck(1, 0);

            auto u1 = shared_test(test_behavior::init());
            assert_ck(2, 0);

            auto old0 = u0.get();
            auto old1 = u1.get();
            assert_ck(2, 0);

            u0.swap(u1);
            assert_ck(2, 0);

            assert(u0.get() == old1);
            assert(u1.get() == old0);
        }

        assert_ck(2, 2);
    }

    void shared_15()
    {
        {
            assert_ck(0, 0);

            auto u0 = shared_test(test_behavior::init());
            assert_ck(1, 0);            
            assert(u0.unique());

            auto u1 = u0;
            assert_ck(1, 0);            
            assert(!u0.unique());

            u1 = shared_test(test_behavior::init());
            assert_ck(2, 0);            
            assert(u0.unique());
            assert(u1.unique());
        }

        assert_ck(2, 2);
    }
}