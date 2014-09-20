#include <SSVUtils/Core/Core.hpp>
#include <SSVUtils/Benchmark/Benchmark.hpp>

struct TypeIdManager
{
    static std::size_t lastId;
    template<typename T> inline std::size_t get() noexcept
    {
        static id{lastId++};
        return id;
    }
};

std::size_t TypeIdManager::lastId{0u};