#include <SSVUtils/Core/Core.hpp>

template <typename T>
struct TypeWrapper
{
    using Type = T;
};

template <typename T>
using TypeOf = typename T::Type;

#define TW_TYPEOF(mX) TypeOf<decltype(tw)>

template <typename TF>
inline void forTypes(TF&&)
{
}

template <typename T, typename... Ts, typename TF>
inline void forTypes(TF&& mFn)
{
    mFn(TypeWrapper<T>{});
    forTypes<Ts...>(mFn);
}

int main()
{
    forTypes<int, char, float, double>([](auto tw)
        {
            ssvu::lo() << typeid(TW_TYPEOF(tw)).name() << "\n";
        });

    return 0;
}