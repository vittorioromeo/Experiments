#include <SSVUtils/Core/Core.hpp>

template <typename T, typename... TArgs>
inline void pold(void* mPtr, TArgs&&... mArgs) noexcept
{
    new(mPtr) T(FWD(mArgs)...);
}

template <typename T, typename... TArgs>
inline void pnew(void* mPtr, TArgs&&... mArgs)
{
    new(mPtr) T(FWD(mArgs)...);
}

int main()
{
    void* x;
    pnew<int>(x);

    return 0;
}