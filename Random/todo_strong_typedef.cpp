#pragma once

#define IMPL_STRONG_TYPEDEF_RHS_OP(underlying_type, typedef_name, op) \
    inline constexpr decltype(auto) operator op(                      \
        const typedef_name& rhs) noexcept                             \
    {                                                                 \
        return _underlying_value op rhs._underlying_value;            \
    }

    // TODO: mutating operators?
    // TODO operator=(typedef) or operator=(underlying)?

// Defines a "strong" type alias for `underlying_type`, called `typedef_name`.
#define STRONG_TYPEDEF(underlying_type, typedef_name)                          \
    struct typedef_name                                                        \
    {                                                                          \
        underlying_type _underlying_value;                                     \
                                                                               \
        inline typedef_name() = default;                                       \
                                                                               \
        inline typedef_name(const typedef_name& mX) = default;                 \
        inline typedef_name(typedef_name&& mX) = default;                      \
                                                                               \
        inline typedef_name& operator=(const typedef_name& rhs) = default;     \
        inline typedef_name& operator=(typedef_name&& rhs) = default;          \
                                                                               \
        inline constexpr explicit typedef_name(underlying_type mX) noexcept    \
            : _underlying_value{mX}                                            \
        {                                                                      \
        }                                                                      \
                                                                               \
        inline constexpr typedef_name& operator=(underlying_type rhs) noexcept \
        {                                                                      \
            _underlying_value = rhs;                                           \
            return *this;                                                      \
        }                                                                      \
                                                                               \
        inline constexpr operator const underlying_type&() const noexcept      \
        {                                                                      \
            return _underlying_value;                                          \
        }                                                                      \
                                                                               \
        inline constexpr operator underlying_type&() noexcept                  \
        {                                                                      \
            return _underlying_value;                                          \
        }                                                                      \
                                                                               \
        IMPL_STRONG_TYPEDEF_RHS_OP(underlying_type, typedef_name, == )         \
        IMPL_STRONG_TYPEDEF_RHS_OP(underlying_type, typedef_name, != )         \
        IMPL_STRONG_TYPEDEF_RHS_OP(underlying_type, typedef_name, < )          \
        IMPL_STRONG_TYPEDEF_RHS_OP(underlying_type, typedef_name, > )          \
        IMPL_STRONG_TYPEDEF_RHS_OP(underlying_type, typedef_name, <= )         \
        IMPL_STRONG_TYPEDEF_RHS_OP(underlying_type, typedef_name, >= )         \
    };                                                                         \
                                                                               \
    static_assert(std::is_literal_type<typedef_name>{}, "");                   \
    static_assert(std::is_standard_layout<typedef_name>{}, "")