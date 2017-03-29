#include "../visit_recursively_simple.hpp"
#include "../recursive_variant.hpp"
#include "./test_utils.hpp"
#include "./variant_test_utils.hpp"
#include <memory>

using base_case = int;
using pair_type = std::pair<vr::self, int>;
using recr_case = std::unique_ptr<pair_type>;

template <typename T>
struct test_case
{
    using builder = vr::recursive_variant_builder<T>;
    using variant = vr::variant_type<builder>;
    using base = vr::resolve<builder, base_case>;
    using ptyp = vr::resolve<builder, pair_type>;
    using recr = vr::resolve<builder, recr_case>;

    static void run()
    {
        auto vis = vr::make_recursive_visitor<int>( // .
            [](auto, int x) { return x; },          // .
            [&](auto recurse, recr& x) {
                return x->second + recurse(x->first);
            });

        {
            variant v{1};
            TEST_ASSERT_OP(vr::visit_recursively_simple(vis, v), ==, 1);
        }

        {
            variant v{std::make_unique<ptyp>(variant{2}, 1)};
            TEST_ASSERT_OP(vr::visit_recursively_simple(vis, v), ==, 1 + 2);
        }
    }
};

TEST_MAIN()
{
    using namespace test;

    with_all_variant_implementations(       // .
        alternatives<base_case, recr_case>, // .
        [](auto make)                       // .
        {
            using type = decltype(make(std::declval<base_case>()));
            test_case<type>::run();
        });
}