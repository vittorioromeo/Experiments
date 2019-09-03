#include <cmath>
#include <cstdint>
#include <type_traits>

namespace tinyecs::mp
{
    template <typename T>
    using t = std::type_identity<T>;

    template <typename T>
    inline constexpr t<T> t_v;
} // namespace tinyecs::mp

namespace tinyecs::impl
{
    using ui8 = std::uint8_t;
    using ui16 = std::uint16_t;
    using ui32 = std::uint32_t;
    using ui64 = std::uint64_t;

    template <typename T>
    constexpr ui64 epoch_bit_count() noexcept
    {
        if constexpr(std::is_same_v<T, ui8>)
        {
            return 2;
        }
        else if constexpr(std::is_same_v<T, ui16>)
        {
            return 4;
        }
        else if constexpr(std::is_same_v<T, ui32>)
        {
            return 6;
        }
        else
        {
            static_assert(std::is_same_v<T, ui64>);
            return 8;
        }
    }

    template <typename T>
    inline constexpr ui64 epoch_bit_count_v{epoch_bit_count<T>()};

    template <typename T>
    inline constexpr ui64 id_bit_count_v{
        (sizeof(T) * 8ul) - epoch_bit_count_v<T>};

    template <ui64 Power>
    inline constexpr ui64 pow2_v{1ul << Power};

    template <typename T>
    inline constexpr ui64 entity_limit_v{pow2_v<id_bit_count_v<T>>};

    template <ui64 EntityLimit>
    constexpr auto entity_id_primitive() noexcept
    {
        if constexpr(EntityLimit <= entity_limit_v<ui8>)
        {
            return mp::t_v<ui8>;
        }
        else if constexpr(EntityLimit <= entity_limit_v<ui16>)
        {
            return mp::t_v<ui16>;
        }
        else if constexpr(EntityLimit <= entity_limit_v<ui32>)
        {
            return mp::t_v<ui32>;
        }
        else
        {
            static_assert(EntityLimit <= entity_limit_v<ui64>);
            return mp::t_v<ui64>;
        }
    }

    template <ui64 EntityLimit>
    using entity_id_primitive_t =
        typename decltype(entity_id_primitive<EntityLimit>())::type;
} // namespace tinyecs::impl

namespace tinyecs
{
    template <impl::ui64 EntityLimit>
    class entity_id
    {
    private:
        using primitive = impl::entity_id_primitive_t<EntityLimit>;
        primitive _value;

    public:
        constexpr entity_id() = default;

        constexpr primitive id() const noexcept
        {
            // TODO: create mask, etc
        }
    };
} // namespace tinyecs

namespace tinyecs::cfg
{
    template <auto X>
    struct entity_limit
    {
    };

    template <auto X>
    inline constexpr entity_limit<X> entity_limit_v;

    template <auto EntityLimit>
    struct settings
    {
        constexpr settings(entity_limit<EntityLimit>) noexcept
        {
        }
    };
} // namespace tinyecs::cfg

namespace tinyecs::mp
{
    template <typename...>
    struct list
    {
    };
} // namespace tinyecs::mp

namespace example::c
{
    struct pos
    {
        float _x, _y;
    };

    struct vel
    {
        float _x, _y;
    };

    struct acc
    {
        float _x, _y;
    };
} // namespace example::c

int main()
{
    constexpr auto cfg =
        tinyecs::cfg::settings{tinyecs::cfg::entity_limit_v<2048>};
}
