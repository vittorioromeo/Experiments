#include "../recursive_variant.hpp"
#include "../visit_recursively.hpp"
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
        {
            variant v{1};
            vr::visit_recursively([](const auto&) {})(
                [](const auto&, const auto&) {})(v);

            // TODO: doesn't work
            vr::visit_recursively([](int) {}, [](char) {})([](auto, auto) {})(
                std::variant<int>{1});
            // TODO: visit
        }

        {
            variant v{std::make_unique<ptyp>(1, 2)};
            // TODO: visit
        }

        {
            variant v{std::make_unique<ptyp>(std::make_unique<ptyp>(1, 2), 3)};
            // TODO: visit
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