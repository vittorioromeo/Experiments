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
#include "./tests_unique.hpp"
#include "./tests_shared.hpp"
#include "./tests_weak.hpp"

namespace test
{
    void run_all()
    {
#define RUN_T(x) run(#x, &x)

        RUN_T(unique_0);
        RUN_T(unique_1);
        RUN_T(unique_2);
        RUN_T(unique_3);
        RUN_T(unique_4);
        RUN_T(unique_5);
        RUN_T(unique_6);

        RUN_T(shared_0);
        RUN_T(shared_1);
        RUN_T(shared_2);
        RUN_T(shared_3);
        RUN_T(shared_4);
        RUN_T(shared_5);
        RUN_T(shared_6);
        RUN_T(shared_7);
        RUN_T(shared_8);
        RUN_T(shared_9);
        RUN_T(shared_10);
        RUN_T(shared_11);
        RUN_T(shared_12);
        RUN_T(shared_13);
        RUN_T(shared_14);
        RUN_T(shared_15);

        RUN_T(weak_0);
        RUN_T(weak_1);
        RUN_T(weak_2);
        RUN_T(weak_3);
        RUN_T(weak_4);
        RUN_T(weak_5);

#undef RUN_T
    }
}