#include <functional>
#include <iostream>
#include <type_traits>

template <typename Signature>
class function_ref;

template <typename Return, typename... Args>
class function_ref<Return(Args...)>
{
private:
    void* _ptr;
    Return (*_erased_fn)(void*, Args...);

public:
    template <typename T, typename = std::enable_if_t<
                              std::is_invocable<T&, Args...>{} &&
                              !std::is_same<std::decay_t<T>, function_ref>{}>>
    function_ref(T&& x) noexcept : _ptr{(void*)std::addressof(x)}
    {
        _erased_fn = [](void* ptr, Args... xs) -> Return {
            return (*reinterpret_cast<std::add_pointer_t<T>>(ptr))(
                std::forward<Args>(xs)...);
        };
    }

    decltype(auto) operator()(Args... xs) const
        noexcept(noexcept(_erased_fn(_ptr, std::forward<Args>(xs)...)))
    {
        return _erased_fn(_ptr, std::forward<Args>(xs)...);
    }
};

void foo(function_ref<void()> f)
{
    f();
}

int main()
{
    auto l = [i = 0]() mutable { std::cout << i++ << "\n"; };
    foo(l);
    foo(l);
    foo(l);
}
