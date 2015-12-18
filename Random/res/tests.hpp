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

namespace test
{
    volatile int created{0};
    volatile int killed{0};

    struct test_behavior
    {
        using handle_type = int;

        test_behavior() = delete;

        static handle_type null_handle()
        {
            return -1;
        }

        static handle_type init()
        {
            ++created;

            static int next{0};
            return next++;
        }

        static void deinit(const handle_type& h)
        {
            if(h != null_handle())
                ++killed;
        }
    };
    static_assert(is_valid_behavior<test_behavior>{}, "");

    using unique_test = resource::unique<test_behavior>;
    using shared_test = resource::shared<test_behavior>;

    void assert_ck(int a_created, int a_killed)
    {
        bool failed = false;

        if(created != a_created)
            failed = true;
        
        if(killed != a_killed)
            failed = true;

        if(failed)
        {
            std::cout << "created: " << created << " (expected:" << a_created << ")\n";
            std::cout << "killed: " << killed << " (expected:" << a_killed << ")\n";
            std::terminate();
        }
    }

    template <typename TF>
    void run(const char* title, TF&& f)
    {
        std::cout << "Running test: " << title << "\n";

        created = killed = 0;
        f();
    }
}
