#pragma once

#include "./dependencies.hpp"
#include "./aliases.hpp"
#include "./vtable.hpp"

// TODO:
namespace impl
{
    namespace storage
    {
        template <typename TSignature, typename TVTable, typename TAllocator>
        class dynamic_storage
        {
        private:
            using signature = TSignature;
            using vtable_type = TVTable;


            static constexpr auto alignment = alignof(std::max_align_t);

            template <typename T>
            constexpr auto round_up_to_alignment(T x) const noexcept
            {
                return multiple_round_up(x, alignment);
            }

        public:
        };
    }
}