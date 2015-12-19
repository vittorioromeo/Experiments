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
    // Null unique
    void unique_0()
    {
        assert_ck(0, 0);

        {
            unique_test u;
            assert_ck(0, 0);
        }

        assert_ck(0, 0);
    }

    // Null unique, to owning unique
    void unique_1()
    {
        assert_ck(0, 0);

        {
            unique_test u;
            assert_ck(0, 0);

            u.reset(test_behavior::init());
            assert_ck(1, 0);
        }

        assert_ck(1, 1);
    }

    // Owning unique
    void unique_2()
    {
        assert_ck(0, 0);

        {
            unique_test u{test_behavior::init()};
            assert_ck(1, 0);
        }

        assert_ck(1, 1);
    }

    // Owning unique, to null unique
    void unique_3()
    {
        assert_ck(0, 0);

        {
            unique_test u{test_behavior::init()};
            assert_ck(1, 0);

            u.reset();
            assert_ck(1, 1);
        }

        assert_ck(1, 1);
    }

    // Owning unique, to null unique (2)
    void unique_4()
    {
        assert_ck(0, 0);

        {
            unique_test u{test_behavior::init()};
            assert_ck(1, 0);

            u.reset(test_behavior::null_handle());
            assert_ck(1, 1);
        }

        assert_ck(1, 1);
    }

    // Owning unique, to owning unique
    void unique_5()
    {
        assert_ck(0, 0);

        {
            unique_test u{test_behavior::init()};
            assert_ck(1, 0);

            u.reset(test_behavior::init());
            assert_ck(2, 1);
        }

        assert_ck(2, 2);
    }

    // Random test
    void unique_6()
    {
        assert_ck(0, 0);

        {
            unique_test u{test_behavior::init()};
            assert_ck(1, 0);

            u.reset(test_behavior::init());
            assert_ck(2, 1);

            unique_test u2;
            assert_ck(2, 1);

            u2 = std::move(u);
            assert_ck(2, 1);
        }

        assert_ck(2, 2);
    }
}