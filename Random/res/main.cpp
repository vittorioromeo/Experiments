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

int main()
{
    test::run_all();
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