#include <cassert>

using GLenum = unsigned int;

template <typename T>
inline constexpr auto from_enum(const T& x) noexcept
{
    // Comment this line to prevent segmentation fault:
    assert(true);
    // ------------------------------------------------

    return (GLenum)x;
}

enum class buffer_target : GLenum
{
    array
};

struct vbo
{
    static constexpr GLenum target_value{from_enum(buffer_target::array)};
    GLenum x{target_value};
};

int main()
{
    return 0;
}