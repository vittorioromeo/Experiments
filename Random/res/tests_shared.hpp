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
}