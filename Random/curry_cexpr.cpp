#include <cassert>
#include <type_traits>
#include <functional>
#include <experimental/tuple>

#define FWD(...) std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)

namespace impl
{
    namespace detail
    {
        template <typename T>
        class fwd_capture_tuple : private std::tuple<T>
        {
        private:
            using decay_element_type = std::decay_t<T>;
            using base_type = std::tuple<T>;

        protected:
            constexpr auto& as_tuple() noexcept
            {
                return static_cast<base_type&>(*this);
            }

            constexpr const auto& as_tuple() const noexcept
            {
                return static_cast<const base_type&>(*this);
            }

            template <typename TFwd>
            constexpr fwd_capture_tuple(TFwd&& x)
                noexcept(std::is_nothrow_constructible<base_type, decltype(x)>{})
                : base_type(FWD(x))
            {
            }

        public:
            constexpr auto& get() & noexcept
            {
                return std::get<0>(as_tuple());
            }

            constexpr const auto& get() const & noexcept
            {
                return std::get<0>(as_tuple());
            }

            constexpr auto get() && noexcept(std::is_move_constructible<decay_element_type>{})
            {
                return std::move(std::get<0>(as_tuple()));
            }
        };
    }

    template <typename T>
    class fwd_capture_wrapper : public detail::fwd_capture_tuple<T>
    {
    private:
        using base_type = detail::fwd_capture_tuple<T>;

    public:
        template <typename TFwd>
        constexpr fwd_capture_wrapper(TFwd&& x)
            noexcept(std::is_nothrow_constructible<base_type, decltype(x)>{})
            : base_type(FWD(x))
        {
        }
    };

    template <typename T>
    class fwd_copy_capture_wrapper : public detail::fwd_capture_tuple<T>
    {
    private:
        using base_type = detail::fwd_capture_tuple<T>;

    public:
        // No `FWD` is intentional, to force a copy if `T` is not an lvalue reference.
        template <typename TFwd>
        constexpr fwd_copy_capture_wrapper(TFwd&& x)
            noexcept(std::is_nothrow_constructible<base_type, decltype(x)>{})
            : base_type(x)
        {
        }
    };

    template <typename T>
    constexpr auto fwd_capture(T&& x)
        noexcept(noexcept(fwd_capture_wrapper<T>(FWD(x))))
    {
        return fwd_capture_wrapper<T>(FWD(x));
    }

    template <typename T>
    constexpr auto fwd_copy_capture(T&& x)
        noexcept(noexcept(fwd_copy_capture_wrapper<T>(FWD(x))))
    {
        return fwd_copy_capture_wrapper<T>(FWD(x));
    }
}

#define FWD_CAPTURE(...) impl::fwd_capture(FWD(__VA_ARGS__))

#define FWD_COPY_CAPTURE(...) \
    impl::fwd_copy_capture(FWD(__VA_ARGS__))

namespace impl
{
    template <typename... Ts>
    constexpr auto fwd_capture_as_tuple(Ts&&... xs)
    {
        return std::make_tuple(FWD_CAPTURE(xs)...);
    }

    template <typename... Ts>
    constexpr auto fwd_copy_capture_as_tuple(Ts&&... xs)
    {
        return std::make_tuple(FWD_COPY_CAPTURE(xs)...);
    }

    template <typename TF, typename TFwdCapture>
    constexpr decltype(auto) apply_fwd_capture(TF&& f, TFwdCapture&& fc)
    {
        return std::experimental::apply([&f](auto&&... xs) constexpr mutable -> decltype(
                                          auto) { return f(FWD(xs).get()...); },
            FWD(fc));
    }
}

#define FWD_CAPTURE_PACK_AS_TUPLE(...) \
    impl::fwd_capture_as_tuple(FWD(__VA_ARGS__)...)

#define FWD_COPY_CAPTURE_PACK_AS_TUPLE(...) \
    impl::fwd_copy_capture_as_tuple(FWD(__VA_ARGS__)...)


namespace impl
{
    template< class, class = std::void_t<> >
    struct needs_unapply : std::true_type { };

    template< class T >
    struct needs_unapply<T, std::void_t<decltype(std::declval<T>()())>> : std::false_type { };
}

template <typename F>
constexpr decltype(auto) curry(F&& f) 
{
    if constexpr (impl::needs_unapply<decltype(f)>{}) 
    {
        return [f = FWD_CAPTURE(f)](auto&&... partials) constexpr 
        {
            return curry
            (
                [
                    partial_pack = FWD_CAPTURE_PACK_AS_TUPLE(partials), 
                    wrapped_f = std::move(f)
                ]
                (auto&&... xs) constexpr -> decltype(f.get()(FWD(partials)..., FWD(xs)...)) // ???
                {
                    return apply_fwd_capture([&wrapped_f](auto&&... js) constexpr -> decltype(wrapped_f.get()(FWD(js)...)) // ??? 
                    {
                        return wrapped_f.get()(FWD(js)...);
                    }, std::tuple_cat(partial_pack, FWD_CAPTURE_PACK_AS_TUPLE(xs)));
                }
            );
        };
    }
    else return f();
}

struct nocopy
{   
    nocopy() = default;
    nocopy(const nocopy&) = delete;
    nocopy(nocopy&&) = default;
};

int main()
{
    auto sum = [](auto a, auto b, auto c, auto d, auto e, auto f, auto g, auto h) constexpr 
    {
        return a + b + c + d + e + f + g + h;
    };
    
    constexpr auto cexpr_csum0 = curry(sum)(0,1,2,3,4,5,6,7);
    constexpr auto cexpr_csum1 = curry(sum)(0)(1,2,3,4,5,6,7);
    constexpr auto cexpr_csum2 = curry(sum)(0,1)(2,3,4,5,6,7);
    constexpr auto cexpr_csum3 = curry(sum)(0,1,2)(3,4,5,6,7);
    constexpr auto cexpr_csum4 = curry(sum)(0,1,2,3)(4,5,6,7);
    constexpr auto cexpr_csum5 = curry(sum)(0,1,2,3,4)(5,6,7);
    constexpr auto cexpr_csum6 = curry(sum)(0,1,2,3,4,5)(6,7);
    constexpr auto cexpr_csum7 = curry(sum)(0,1,2,3,4,5,6)(7);
    
    static_assert(cexpr_csum0 == sum(0,1,2,3,4,5,6,7));
    static_assert(cexpr_csum1 == sum(0,1,2,3,4,5,6,7));
    static_assert(cexpr_csum2 == sum(0,1,2,3,4,5,6,7));
    static_assert(cexpr_csum3 == sum(0,1,2,3,4,5,6,7));
    static_assert(cexpr_csum4 == sum(0,1,2,3,4,5,6,7));
    static_assert(cexpr_csum5 == sum(0,1,2,3,4,5,6,7));
    static_assert(cexpr_csum6 == sum(0,1,2,3,4,5,6,7));
    static_assert(cexpr_csum7 == sum(0,1,2,3,4,5,6,7));

    volatile auto vcexpr_csum1 = curry(sum)(0)(1,2,3,4,5,6,7);
    volatile auto vcexpr_csum0 = curry(sum)(0,1,2,3,4,5,6,7);
    volatile auto vcexpr_csum2 = curry(sum)(0,1)(2,3,4,5,6,7);
    volatile auto vcexpr_csum3 = curry(sum)(0,1,2)(3,4,5,6,7);
    volatile auto vcexpr_csum4 = curry(sum)(0,1,2,3)(4,5,6,7);
    volatile auto vcexpr_csum5 = curry(sum)(0,1,2,3,4)(5,6,7);
    volatile auto vcexpr_csum6 = curry(sum)(0,1,2,3,4,5)(6,7);
    volatile auto vcexpr_csum7 = curry(sum)(0,1,2,3,4,5,6)(7);
}