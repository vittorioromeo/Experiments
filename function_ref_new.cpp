#include <boost/callable_traits.hpp>
#include <functional>
#include <iostream>
#include <type_traits>

template <            //
    typename Derived, //
    bool IsConst,     //
    bool IsNoexcept,  //
    typename Return,  //
    typename... Args  //
    >
class function_ref_impl
{
private:
    using erased_fn_type = Return (*)(void*, Args...);

    using final_erased_fn_type = std::conditional_t<            //
        IsNoexcept,                                             //
        boost::callable_traits::add_noexcept_t<erased_fn_type>, //
        erased_fn_type                                          //
        >;

    void* _ptr;
    final_erased_fn_type _erased_fn;

    template <typename T>
    using propagate_const = std::conditional_t< //
        IsConst,                                //
        std::add_const_t<T>,                    //
        T                                       //
        >;

    template <typename... Xs>
    using invocable_check = std::conditional_t< //
        IsNoexcept,                             //
        std::is_nothrow_invocable_r<Xs...>,     //
        std::is_invocable_r<Xs...>              //
        >;

    template <typename T>
    using is_compatibly_invokable = invocable_check<     //
        Return,                                          //
        std::add_lvalue_reference_t<propagate_const<T>>, //
        Args...>;

    template <typename T>
    using enable_if_not_self = std::enable_if_t<     //
        is_compatibly_invokable<T>::value            //
        && !std::is_same_v<std::decay_t<T>, Derived> //
        >;

    template <typename T>
    using enable_if_compatible_const = std::enable_if_t< //
        boost::callable_traits::is_const_member_v<T>     //
        || !IsConst                                      //
        || std::is_function_v<std::remove_pointer_t<T>>  //
        >;

    template <typename T>
    using enable_if_compatible_noexcept = std::enable_if_t< //
        boost::callable_traits::is_noexcept_v<T>            //
        || !IsNoexcept                                      //
        >;

    template <typename T>
    using enable_if_valid = std::conjunction< //
        enable_if_not_self<T>,                //
        enable_if_compatible_const<T>,        //
        enable_if_compatible_noexcept<T>      //
        >;

    template <typename T>
    auto make_erased_fn() noexcept
    {
        return [](void* ptr, Args... xs) noexcept(IsNoexcept)->Return
        {
            return std::invoke(
                *reinterpret_cast<std::add_pointer_t<propagate_const<T>>>(ptr),
                std::forward<Args>(xs)...);
        };
    }

protected:
    Return call(Args... xs) const
    {
        return _erased_fn(_ptr, std::forward<Args>(xs)...);
    }

public:
    template <typename T, typename = enable_if_valid<T>>
    constexpr function_ref_impl(T&& x) noexcept
        : _ptr{std::addressof(x)}, _erased_fn{make_erased_fn<T>()}
    {
    }

    constexpr function_ref_impl(
        const function_ref_impl& rhs) noexcept = default;

    constexpr function_ref_impl& operator=(
        const function_ref_impl& rhs) noexcept = default;

    constexpr void swap(function_ref_impl& rhs) noexcept
    {
        std::swap(_ptr, rhs._ptr);
        std::swap(_erased_fn, rhs._erased_fn);
    }
};

template <typename Signature>
class function_ref;

#define DEPARENS(...) __VA_ARGS__

#define DEFINE_FUNCTION_REF_SPECIALIZATION(                                    \
    is_const, is_noexcept, kw_const, kw_noexcept)                              \
    template <typename Return, typename... Args>                               \
    class function_ref<Return(Args...) DEPARENS kw_const DEPARENS kw_noexcept> \
        : public function_ref_impl<function_ref<Return(Args...)>, is_const,    \
              is_noexcept, Return, Args...>                                    \
    {                                                                          \
    private:                                                                   \
        using base_type = function_ref_impl<function_ref<Return(Args...)>,     \
            is_const, is_noexcept, Return, Args...>;                           \
                                                                               \
    public:                                                                    \
        using base_type::base_type;                                            \
                                                                               \
        Return operator()(Args... xs) const DEPARENS kw_noexcept               \
        {                                                                      \
            return this->call(std::forward<Args>(xs)...);                      \
        }                                                                      \
    };

DEFINE_FUNCTION_REF_SPECIALIZATION(false, false, (), ());
DEFINE_FUNCTION_REF_SPECIALIZATION(true, false, (const), ());
DEFINE_FUNCTION_REF_SPECIALIZATION(false, true, (), (noexcept));
DEFINE_FUNCTION_REF_SPECIALIZATION(true, true, (const), (noexcept));
#undef DEFINE_FUNCTION_REF_SPECIALIZATION

template <typename Sig>
struct remove_first_arg;

template <typename R, typename A, typename... As>
struct remove_first_arg<R(A, As...)>
{
    using type = R(As...);
};

template <typename Sig>
using remove_first_arg_t = typename remove_first_arg<Sig>::type;



template <typename R, typename... Args>
function_ref(R (*)(Args...))->function_ref<R(Args...)>;

template <typename R, typename... Args>
function_ref(R (*)(Args...) noexcept)->function_ref<R(Args...) noexcept>;

// TODO: noexcept and const qualifier
template <typename F, typename S = decltype(&std::decay_t<F>::operator())>
function_ref(F &&)
    ->function_ref<
        remove_first_arg_t<boost::callable_traits::function_type_t<S>>>;

template <typename Signature>
constexpr void swap(
    function_ref<Signature>& lhs, function_ref<Signature>& rhs) noexcept
{
    lhs.swap(rhs);
}

void foo(function_ref<void()> f)
{
    f();
}

void fp_nonnoexcept()
{
}
void fp_noexcept() noexcept
{
}

struct c
{
    void ff()
    {
    }
    void tf() const
    {
    }
    void ft() noexcept
    {
    }
    void tt() const noexcept
    {
    }
};

void test_nonconst_nonnoexcept()
{
    using T = function_ref<void()>;
    using M = function_ref<void(c)>;

    static_assert(boost::callable_traits::is_const_member_v<T>);
    static_assert(boost::callable_traits::is_const_member_v<M>);
    static_assert(!boost::callable_traits::is_noexcept_v<T>);
    static_assert(!boost::callable_traits::is_noexcept_v<M>);

    // FPtrs:
    T{&fp_nonnoexcept}; // OK
    T{&fp_noexcept};    // OK

    // MPtrs:
    M{&c::ff}; // OK
    M{&c::tf}; // OK
    M{&c::ft}; // OK
    M{&c::tt}; // OK

    // Lambdas:
    T{[] {}};                    // OK
    T{[]() mutable {}};          // OK
    T{[]() noexcept {}};         // OK
    T{[]() mutable noexcept {}}; // OK
}

void test_const_nonnoexcept()
{
    using T = function_ref<void() const>;
    using M = function_ref<void(c) const>;

    static_assert(boost::callable_traits::is_const_member_v<T>);
    static_assert(boost::callable_traits::is_const_member_v<M>);
    static_assert(!boost::callable_traits::is_noexcept_v<T>);
    static_assert(!boost::callable_traits::is_noexcept_v<M>);

    // FPtrs:
    T{&fp_nonnoexcept}; // OK, const ignored for non-members
    T{&fp_noexcept};    // OK, const ignored for non-members

    // MPtrs:
    // M{&c::ff}; // OK - does not compile as intended
    M{&c::tf}; // OK
    // M{&c::ft}; // OK - does not compile as intended
    M{&c::tt}; // OK

    // Lambdas:
    // T{[]() mutable {}};          // OK - does not compile as intended
    T{[] {}}; // OK
    // T{[]() mutable noexcept {}}; // OK - does not compile as intended
    T{[]() noexcept {}}; // OK
}

void test_nonconst_noexcept()
{
    using T = function_ref<void() noexcept>;
    using M = function_ref<void(c) noexcept>;

    static_assert(boost::callable_traits::is_const_member_v<T>);
    static_assert(boost::callable_traits::is_const_member_v<M>);
    static_assert(boost::callable_traits::is_noexcept_v<T>);
    static_assert(boost::callable_traits::is_noexcept_v<M>);

    // FPtrs:
    // T{&fp_nonnoexcept}; // OK - does not compile as intended
    T{&fp_noexcept}; // OK

    // MPtrs:
    // M{&c::ff}; // OK - does not compile as intended
    // M{&c::tf}; // OK - does not compile as intended
    M{&c::ft}; // OK
    M{&c::tt}; // OK

    // Lambdas:
    // T{[]() mutable {}};       // OK - does not compile as intended
    // T{[] {}};                 // OK - does not compile as intended
    T{[]() mutable noexcept {}}; // OK
    T{[]() noexcept {}};         // OK
}

void test_const_noexcept()
{
    using T = function_ref<void() const noexcept>;
    using M = function_ref<void(c) const noexcept>;

    static_assert(boost::callable_traits::is_const_member_v<T>);
    static_assert(boost::callable_traits::is_const_member_v<M>);
    static_assert(boost::callable_traits::is_noexcept_v<T>);
    static_assert(boost::callable_traits::is_noexcept_v<M>);

    // FPtrs:
    // T{&fp_nonnoexcept}; // OK - does not compile as intended
    T{&fp_noexcept}; // OK

    // MPtrs:
    // M{&c::ff}; // OK - does not compile as intended
    // M{&c::tf}; // OK - does not compile as intended
    // M{&c::ft}; // OK - does not compile as intended
    M{&c::tt}; // OK

    // Lambdas:
    // T{[]() mutable {}};          // OK - does not compile as intended
    // T{[] {}};                    // OK - does not compile as intended
    // T{[]() mutable noexcept {}}; // OK - does not compile as intended
    T{[]() noexcept {}}; // OK
}

struct F
{
    int operator()() const noexcept
    {
        return 0;
    }
};

struct anything
{
    template <typename T>
    anything(T const&)
    {
    }
};



int main()
{
    auto l = [i = 0]() mutable { std::cout << i++ << "\n"; };
    foo(l);
    foo(l);
    foo(l);

    std::function<void()> avbxf{function_ref<void()>{[] {}}};

    // Should not compile:
    // function_ref<anything() noexcept> fun = F{};
    // function_ref f1{[] {}};
}



#include <cassert>

// `function_ref`: non-owning wrapper over generic `Callable`
// with a particular signature

int xfoo(function_ref<int()> f)
{
    return f();
}

int xbar()
{
    auto l = [i = 0]() mutable { return i++; };
    assert(xfoo(l) == 0); // <== reference semantics
    assert(xfoo(l) == 1);
    assert(xfoo(l) == 2);

    auto g = [](function_ref<int() const> k) { k(); };
    /* g(l); */ // <== does not compile  ^~~~~

    g([] { return 0; }); // <== works with temporaries
    return 0;
}

void aaa()
{
}

void fr_aaa()
{
    function_ref<void()> f{&aaa};
    f(); // undefined behavior
}
