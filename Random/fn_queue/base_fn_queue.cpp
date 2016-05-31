#pragma once

#include "./dependencies.hpp"
#include "./aliases.hpp"
#include "./vtable.hpp"
#include "./fixed_storage.hpp"
#include "./dynamic_storage.hpp"

// TODO:
namespace impl
{
    template <typename TStoragePolicy>
    class base_function_queue
    {
    private:
        using storage_type = TStoragePolicy;
        using signature = typename storage_type::signature;

    public:
    };
}