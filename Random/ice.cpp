template <typename TF>
constexpr void foo(TF f)
{
    f(0);
}

template <int S, class... Ts>
constexpr auto bar()
{
    constexpr int arr[] = {Ts{}...};
    foo([&](auto x) { arr[x]; });
}

int main()
{
    bar<1, int>();
}