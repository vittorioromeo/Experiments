#include <array>
#include <cstddef>
#include <functional>
#include <iostream>
#include <utility>
#include <vector>
#include <vrm/core/assert.hpp>
#include <vrm/core/casts.hpp>
#include <vrm/core/static_if.hpp>
#include <boost/hana.hpp>
#include <cstring>

// Debug logging.
#if 0
#define ELOG(...) __VA_ARGS__
#else
#define ELOG(...)
#endif

/// @brief Round up `x` to the nearest multiple of `multiple`.
template <typename T0, typename T1>
constexpr auto multiple_round_up(T0 x, T1 multiple) noexcept
{
    VRM_CORE_CONSTEXPR_ASSERT(multiple != 0);
    return ((x + multiple - 1) / multiple) * multiple;
}

/// @brief Alias for a function pointer.
template <typename TSignature>
using fn_ptr = TSignature*;

/// @brief Alias for `char`.
using byte = char;

/// @brief Alias for `boost::hana` namespace.
namespace bh = boost::hana;

/// @brief Alias for `vrm::core` namespace.
namespace vrmc = vrm::core;

/// @brief Namespace dealing with vtable options and construction.
namespace vtable
{
    namespace option
    {
        /// @brief Enables the vtable to store a call operator ptr.
        constexpr bh::int_<0> call{};

        /// @brief Enables the vtable to store a dtor ptr.
        constexpr bh::int_<1> dtor{};

        /// @brief Enables the vtable to store a copy ctor ptr.
        constexpr bh::int_<2> copy{};

        /// @brief Enables the vtable to store a move ctor ptr.
        constexpr bh::int_<3> move{};

        /// @brief Create a list of options that can be passed to
        /// `vtable::make`.
        template <typename... TOptions>
        auto make_list(TOptions... os) noexcept
        {
            // TODO: statically assert that `os...` are options.
            auto result = bh::make_basic_tuple(os...);

            VRM_CORE_STATIC_ASSERT_NM(
                bh::equal(bh::unique(bh::sort(result)), result));

            return result;
        }
    }

    namespace impl
    {
        /// @brief Helper struct that creates the vtable.
        template <typename TSignature>
        class maker;

        template <typename TReturn, typename... TArgs>
        class maker<TReturn(TArgs...)>
        {
        private:
            /// @brief Call function pointer type.
            /// @details The first parameter is the callable object.
            using call_fp = fn_ptr<TReturn(byte*, TArgs...)>;

            /// @brief Dtor function pointer type.
            /// @details The first parameter is the callable object.
            using dtor_fp = fn_ptr<void(byte*)>;

            /// @brief Copy ctor function pointer type.
            /// @details The first parameter is the vtable.
            /// The second parameter is the src callable object.
            /// The third parameter is the dst callable object.
            using copy_fp = fn_ptr<void(byte*, byte*, byte*)>;

            /// @brief Move ctor function pointer type.
            /// @details The first parameter is the vtable.
            /// The second parameter is the src callable object.
            /// The third parameter is the dst callable object.
            using move_fp = fn_ptr<void(byte*, byte*, byte*)>;

        public:
            template <typename TOptionsList>
            static auto make(TOptionsList ol) noexcept
            {
                // Map the options to their function pointer types.
                auto fp_tuple = bh::make_basic_tuple(       // .
                    bh::make_pair(option::call, call_fp{}), // .
                    bh::make_pair(option::dtor, dtor_fp{}), // .
                    bh::make_pair(option::copy, copy_fp{}), // .
                    bh::make_pair(option::move, move_fp{})  // .
                    );

                // Filter only the selected options (passed in `ol`).
                auto fp_filtered_tuple = bh::filter(fp_tuple, [ol](auto x)
                    {
                        return bh::contains(ol, bh::first(x));
                    });

                // Return the result as a `boost::hana::map`.
                return bh::to_map(fp_filtered_tuple);
            }

            template <typename TF, typename TVTable>
            static void set_call_fp(TVTable& vt) noexcept
            {
                bh::at_key(vt, option::call) = [](byte* obj, TArgs... xs)
                {
                    return reinterpret_cast<TF*>(obj)->operator()(xs...);
                };
            }

            template <typename TF, typename TVTable>
            static void set_dtor_fp(TVTable& vt) noexcept
            {
                bh::at_key(vt, option::dtor) = [](byte* obj)
                {
                    reinterpret_cast<TF*>(obj)->~TF();
                };
            }

            template <typename TF, typename TVTable>
            static void set_copy_fp(TVTable& vt) noexcept
            {
                bh::at_key(vt, option::copy) = [](
                    byte* x_vt_ptr, byte* src, byte* dst)
                {
                    // Copy-construct `src` into `dst`.
                    new(dst) TF(*(reinterpret_cast<TF*>(src)));
                    (void)x_vt_ptr;
                };
            }

            template <typename TF, typename TVTable>
            static void set_move_fp(TVTable& vt) noexcept
            {
                bh::at_key(vt, option::move) = [](
                    byte* x_vt_ptr, byte* src, byte* dst)
                {
                    // Move-construct `src` into `dst`.
                    new(dst) TF(std::move(*(reinterpret_cast<TF*>(src))));
                    (void)x_vt_ptr;
                };
            }
        };
    }

    /// @brief Creates a vtable for callable objects with signature
    /// `TSignature`, using the passed `ol` options.
    /// @details Option lists can be created using `vtable::option::make_list`.
    template <typename TSignature, typename TOptionsList>
    auto make(TOptionsList ol) noexcept
    {
        return impl::maker<TSignature>::make(ol);
    }

    template <typename TVTable, typename... Ts>
    void exec_call_fp(TVTable& vt, Ts&&... xs)
    {
        auto& fp = bh::at_key(vt, option::call);
        (*fp)(FWD(xs)...);
    }

    template <typename TVTable, typename... Ts>
    void exec_dtor_fp(TVTable& vt, Ts&&... xs)
    {
        auto& fp = bh::at_key(vt, option::dtor);
        (*fp)(FWD(xs)...);
    }

    template <typename TVTable, typename... Ts>
    void exec_copy_fp(TVTable& vt, Ts&&... xs)
    {
        auto& fp = bh::at_key(vt, option::copy);
        (*fp)((byte*)&vt, FWD(xs)...);
    }

    template <typename TVTable, typename... Ts>
    void exec_move_fp(TVTable& vt, Ts&&... xs)
    {
        auto& fp = bh::at_key(vt, option::move);
        (*fp)((byte*)&vt, FWD(xs)...);
    }

    template <typename TF, typename TSignature, typename TVTable>
    void set_call_fp(TVTable& vt) noexcept
    {
        using sh = impl::maker<TSignature>;
        sh::template set_call_fp<TF>(vt);
    }

    template <typename TF, typename TSignature, typename TVTable>
    void set_dtor_fp(TVTable& vt) noexcept
    {
        using sh = impl::maker<TSignature>;
        sh::template set_dtor_fp<TF>(vt);
    }

    template <typename TF, typename TSignature, typename TVTable>
    void set_copy_fp(TVTable& vt) noexcept
    {
        using sh = impl::maker<TSignature>;
        sh::template set_copy_fp<TF>(vt);
    }

    template <typename TF, typename TSignature, typename TVTable>
    void set_move_fp(TVTable& vt) noexcept
    {
        using sh = impl::maker<TSignature>;
        sh::template set_move_fp<TF>(vt);
    }

    template <typename TVTable, typename TOption>
    constexpr auto has_option(const TVTable& vt, TOption o) noexcept
    {
        return bh::contains(bh::keys(vt), o);
    }

    template <typename TF, typename TSignature, typename TVTable>
    void setup(TVTable& vt) noexcept
    {
        vrmc::static_if(has_option(vt, option::call))
            .then([](auto& x_vt)
                {
                    set_call_fp<TF, TSignature>(x_vt);
                })(vt);

        vrmc::static_if(has_option(vt, option::call))
            .then([](auto& x_vt)
                {
                    set_dtor_fp<TF, TSignature>(x_vt);
                })(vt);

        vrmc::static_if(
            std::is_copy_constructible<TF>{} && has_option(vt, option::copy))
            .then([](auto& x_vt)
                {
                    set_copy_fp<TF, TSignature>(x_vt);
                })(vt);

        vrmc::static_if(
            std::is_move_constructible<TF>{} && has_option(vt, option::move))
            .then([](auto& x_vt)
                {
                    set_move_fp<TF, TSignature>(x_vt);
                })(vt);
    }
}

// TODO:
namespace impl
{
    namespace storage
    {
        template <typename TSignature, std::size_t TBufferSize>
        class fixed_storage
        {
        };

        template <typename TSignature, typename TAllocator>
        class dynamic_storage
        {
        };
    }

    template <typename TStoragePolicy>
    class base_function_queue;
}

template <typename TSignature, std::size_t TBufferSize>
class fixed_function_queue;

template <typename TReturn, typename... TArgs, std::size_t TBufferSize>
class fixed_function_queue<TReturn(TArgs...), TBufferSize>
{
private:
    using signature = TReturn(TArgs...);
    static constexpr auto buffer_size = TBufferSize;
    static constexpr auto alignment = alignof(std::max_align_t);

    template <typename T>
    constexpr auto round_up_to_alignment(T x) const noexcept
    {
        return multiple_round_up(x, alignment);
    }

    // TODO:
    // Support call, dtor and copy.
    using vtable_type = decltype(vtable::make<signature>( // .
        vtable::option::make_list(vtable::option::call, vtable::option::dtor,
            vtable::option::copy, vtable::option::move)));

    // TODO: use a preallocated fixed buffer instead of `std::vector`, we know
    // the max vtable ptr count
    static constexpr auto max_vtable_ptrs = sizeof(vtable_type*) / buffer_size;

    std::aligned_storage_t<buffer_size, alignment> _buffer;
    std::vector<vtable_type*> _vtable_ptrs;
    byte* _next;

    auto buffer_ptr() noexcept
    {
        return reinterpret_cast<byte*>(&_buffer);
    }

    auto buffer_ptr() const noexcept
    {
        return reinterpret_cast<const byte*>(&_buffer);
    }

    auto offset_from_beginning(byte* ptr) const noexcept
    {
        return ptr - buffer_ptr();
    }

    template <typename T>
    auto buffer_ptr_from_offset(T x) const noexcept
    {
        return buffer_ptr() + x;
    }

    template <typename T, typename... TNewArgs>
    auto aligned_placement_new(byte* ptr, TNewArgs&&... xs) // .
        noexcept(std::is_nothrow_constructible<T, TNewArgs...>{})
    {
        ptr = get_next_aligned_ptr(ptr);
        new(ptr) T{FWD(xs)...};
        return ptr;
    }

    auto emplace_vtable_at(byte* ptr) noexcept
    {
        ELOG( // .
            std::cout << "emplacing vtable... (size: " << sizeof(vtable_type)
                      << ")\n"; // .
            );

        return aligned_placement_new<vtable_type>(ptr);
    }

    template <typename TF>
    auto emplace_fn_at(byte* ptr, TF&& f) // .
        noexcept(std::is_nothrow_constructible<TF, decltype(FWD(f))>{})
    {
        ELOG(                                                              // .
            std::cout << "emplacing fn... (size: " << sizeof(TF) << ")\n"; // .
            );

        return aligned_placement_new<TF>(ptr, FWD(f));
    }

    template <typename TF>
    void bind_vtable_to_fn(vtable_type& vt) noexcept
    {
        ELOG(                                         // .
            std::cout << "binding vtable to fn...\n"; // .
            );

        vtable::template setup<TF, signature>(vt);
    }

    void subscribe_vtable(vtable_type& vt)
    {
        _vtable_ptrs.emplace_back(&vt);
    }

    template <typename TF>
    auto emplace_starting_at(byte* ptr, TF&& f)
    // TODO: noexcept
    {
        VRM_CORE_ASSERT_OP(
            offset_from_beginning(ptr) + sizeof(vtable_type) + sizeof(TF), <,
            buffer_size);

        // Emplace the vtable and get a reference to it.
        ptr = emplace_vtable_at(ptr);
        auto& vt = *(reinterpret_cast<vtable_type*>(ptr));

        ELOG( // .
            std::cout << "vtable offset: " << offset_from_beginning(ptr)
                      << "\n"; // .
            );

        // Get the aligned position where the callable object will be emplaced.
        auto fn_start_ptr = ptr + sizeof(vtable_type);

        ELOG(                                                         // .
            std::cout << "fn start offset: "                          // .
                      << offset_from_beginning(fn_start_ptr) << "\n"; // .
            );

        // Emplace the callable object.
        ptr = emplace_fn_at(fn_start_ptr, FWD(f));

        ELOG(                                                        // .
            std::cout << "fn offset: " << offset_from_beginning(ptr) // .
                      << "\n";                                       // .
            );

        // Bind the vtable to the callable object and add it to the vtable
        // vector.
        bind_vtable_to_fn<TF>(vt);
        subscribe_vtable(vt);

        // Move the "next emplacement pointer" forward.
        // (`ptr` contains the address of the emplaced callable object.)
        _next = ptr + sizeof(TF);

        ELOG( // .
            std::cout << "stored next offset: " << offset_from_beginning(_next)
                      << "\n"; // .
            );
    }

    auto get_next_aligned_ptr(byte* ptr) const noexcept
    {
        auto ofb = offset_from_beginning(ptr);

        ELOG(                                    // .
            std::cout << "ofb: " << ofb << "\n"; // .
            );

        auto next_ofb = round_up_to_alignment(ofb);

        ELOG(                                              // .
            std::cout << "next_ofb: " << next_ofb << "\n"; // .
            );

        return (byte*)buffer_ptr_from_offset(next_ofb);
    }

    auto get_fn_ptr_from_vtable(vtable_type* vt_ptr) const noexcept
    {
        return get_next_aligned_ptr((byte*)vt_ptr + sizeof(vtable_type));
    }

    // TODO: docs, repetition, asserts

    template <typename TF>
    void for_vts(TF&& f)
    // TODO: noexcept
    {
        for(auto vt_ptr : _vtable_ptrs)
        {
            f(vt_ptr);
        }
    }

    template <typename TF>
    void for_fns(TF&& f)
    // TODO: noexcept
    {
        for(auto vt_ptr : _vtable_ptrs)
        {
            auto fn_ptr = get_fn_ptr_from_vtable(vt_ptr);
            f(*vt_ptr, fn_ptr);
        }
    }

    template <typename TF>
    void for_vts(TF&& f) const
    // TODO: noexcept
    {
        for(auto vt_ptr : _vtable_ptrs)
        {
            f(vt_ptr);
        }
    }

    template <typename TF>
    void for_fns(TF&& f) const
    // TODO: noexcept
    {
        for(auto vt_ptr : _vtable_ptrs)
        {
            auto fn_ptr = get_fn_ptr_from_vtable(vt_ptr);
            f(*vt_ptr, fn_ptr);
        }
    }

    template <typename TF>
    void for_fns_reverse(TF&& f)
    // TODO: noexcept
    {
        for(auto itr = std::rbegin(_vtable_ptrs);
            itr != std::rend(_vtable_ptrs); ++itr)
        {
            auto vt_ptr = *itr;
            auto fn_ptr = get_fn_ptr_from_vtable(vt_ptr);
            f(*vt_ptr, fn_ptr);
        }
    }

    void destroy_all()
    {
        ELOG(                                             // .
            std::cout << "destroying all functions...\n"; // .
            for(auto vt_ptr
                : _vtable_ptrs) // .
            {
                std::cout << "    vt_ptr offset: "                         // .
                          << offset_from_beginning((byte*)vt_ptr) << "\n"; // .
            }                                                              // .
            );

        for_fns_reverse([this](auto& vt, auto fn_ptr)
            {
                ELOG(                                              // .
                    std::cout << "    vt_ptr offset: "             // .
                              << offset_from_beginning((byte*)&vt) // .
                              << "\n";                             // .
                    );

                vtable::exec_dtor_fp(vt, fn_ptr);
            });
    }

    void copy_all(const fixed_function_queue& rhs)
    {
        // TODO: optimize: traverse only once.

        VRM_CORE_STATIC_ASSERT_NM(
            vtable::has_option(vtable_type{}, vtable::option::copy));


        rhs.for_fns([this, &rhs](auto& vt, auto fn_ptr)
            {
                ELOG(                                              // .
                    std::cout << "    vt_ptr offset: "             // .
                              << offset_from_beginning((byte*)&vt) // .
                              << "\n";                             // .
                    );

                auto fn_offset = rhs.offset_from_beginning(fn_ptr);
                vtable::exec_copy_fp(vt, fn_ptr, buffer_ptr() + fn_offset);
            });

        rhs.for_vts([this, &rhs](auto vt_ptr)
            {
                auto vt_offset = rhs.offset_from_beginning((byte*)vt_ptr);
                std::memcpy(
                    buffer_ptr() + vt_offset, vt_ptr, sizeof(vtable_type));
            });
    }

    void move_all(fixed_function_queue& rhs)
    {
        // TODO: optimize: traverse only once.

        VRM_CORE_STATIC_ASSERT_NM(
            vtable::has_option(vtable_type{}, vtable::option::move));


        rhs.for_fns([this, &rhs](auto& vt, auto fn_ptr)
            {
                ELOG(                                              // .
                    std::cout << "    vt_ptr offset: "             // .
                              << offset_from_beginning((byte*)&vt) // .
                              << "\n";                             // .
                    );

                auto fn_offset = rhs.offset_from_beginning(fn_ptr);
                vtable::exec_move_fp(vt, fn_ptr, buffer_ptr() + fn_offset);
            });

        rhs.for_vts([this, &rhs](auto vt_ptr)
            {
                auto vt_offset = rhs.offset_from_beginning((byte*)vt_ptr);
                std::memcpy(
                    buffer_ptr() + vt_offset, vt_ptr, sizeof(vtable_type));
            });
    }

public:
    fixed_function_queue() noexcept : _next{buffer_ptr()}
    {
    }

    ~fixed_function_queue()
    // TODO: noexcept
    {
        destroy_all();
    }

    fixed_function_queue(const fixed_function_queue& rhs)
        : _vtable_ptrs{rhs._vtable_ptrs},
          _next{buffer_ptr() + offset_from_beginning(rhs._next)}
    {
        copy_all(rhs);
    }

    fixed_function_queue& operator=(const fixed_function_queue& rhs)
    {
        _vtable_ptrs = rhs._vtable_ptrs;
        _next = buffer_ptr() + offset_from_beginning(rhs._next);
        copy_all(rhs);

        return *this;
    }

    fixed_function_queue(fixed_function_queue&& rhs)
        : _vtable_ptrs{rhs._vtable_ptrs},
          _next{buffer_ptr() + offset_from_beginning(rhs._next)}
    {
        move_all(rhs);
    }

    fixed_function_queue& operator=(fixed_function_queue&& rhs)
    {
        _vtable_ptrs = rhs._vtable_ptrs;
        _next = buffer_ptr() + offset_from_beginning(rhs._next);
        move_all(rhs);

        return *this;
    }

    template <typename TF>
    void emplace(TF&& f)
    // TODO: noexcept
    {
        emplace_starting_at(_next, FWD(f));
    }

    auto call_all(TArgs... xs)
    // TODO: noexcept
    {
        ELOG(                                          // .
            std::cout << "calling all functions...\n"; // .
            // .
            for(auto vt_ptr
                : _vtable_ptrs) // .
            {
                std::cout << "    vt_ptr offset: "                         // .
                          << offset_from_beginning((byte*)vt_ptr) << "\n"; // .
            }                                                              // .
            );

        for_fns([&xs...](auto& vt, auto fn_ptr)
            {
                vtable::exec_call_fp(vt, fn_ptr, xs...);
            });
    }

    void clear()
    {
        destroy_all();
        _vtable_ptrs.clear();
        _next = buffer_ptr();
    }
};

struct lmao
{
    lmao()
    {
        std::cout << "ctor\n";
    }
    ~lmao()
    {
        std::cout << "dtor\n";
    }
};

template <typename TSignature, std::size_t TBufferSize>
struct function_queue_test_adapter
{
private:
    std::vector<std::function<TSignature>> _fn_vec;
    fixed_function_queue<TSignature, TBufferSize> _fn_queue;

public:
    template <typename TF>
    void emplace(TF&& f)
    {
        _fn_vec.emplace_back(FWD(f));
        _fn_queue.emplace(FWD(f));
    }

    template <typename... Ts>
    void call_all(Ts&&... xs)
    {
        for(auto& f : _fn_vec)
        {
            f(FWD(xs)...);
        }

        _fn_queue.call_all(FWD(xs)...);
    }
};

static volatile int ctors;
static volatile int copy_ctors;
static volatile int move_ctors;
static volatile int dtors;

struct cpyx
{
    cpyx()
    {
        ++ctors;
    }
    ~cpyx()
    {
        ++dtors;
    }

    cpyx(const cpyx&)
    {
        ++copy_ctors;
    }

    cpyx(cpyx&&) = delete;
};


struct cpyxm
{
    cpyxm()
    {
        ++ctors;
    }
    ~cpyxm()
    {
        ++dtors;
    }

    cpyxm(const cpyxm&) = delete;

    cpyxm(cpyxm&&)
    {
        ++move_ctors;
    }
};

#define TEST_ASSERT(...)                                                    \
    if(!(__VA_ARGS__))                                                      \
    {                                                                       \
        std::cout << __LINE__ << " " << #__VA_ARGS__ << "\n";               \
        std::cout << ctors << " " << copy_ctors << " " << move_ctors << " " \
                  << dtors << "\n";                                         \
        std::terminate();                                                   \
    }

void cypx_reset()
{
    ctors = dtors = copy_ctors = move_ctors = 0;
}

void cypx_test(int xctors, int xcopy_ctors, int xmove_ctors, int xdtors)
{
    TEST_ASSERT(ctors == xctors);
    TEST_ASSERT(copy_ctors == xcopy_ctors);
    TEST_ASSERT(move_ctors == xmove_ctors);
    TEST_ASSERT(dtors == xdtors);
}

void tests()
{

    {
        int acc = 0;
        int one = 1;

        function_queue_test_adapter<void(int), 512> ta;

        ta.emplace([&acc](int x)
            {
                acc += x;
            });
        ta.emplace([&acc, one](int)
            {
                acc += one;
            });
        ta.emplace([&acc](int x)
            {
                acc -= x;
            });
        ta.emplace([&acc, one](int)
            {
                acc -= one;
            });

        ta.call_all(5);
        std::cout << acc << "\n";
        TEST_ASSERT(acc == 0);
    }

    {
        int acc = 0;

        struct tx
        {
            int& _acc_ref;
            bool _dec{true};

            tx(int& acc_ref) : _acc_ref(acc_ref)
            {
                ++_acc_ref;
            }
            tx(const tx& x) : _acc_ref(x._acc_ref), _dec{false}
            {
            }

            ~tx()
            {
                if(_dec)
                {
                    --_acc_ref;
                }
            }
        };

        {
            function_queue_test_adapter<void(int), 512> ta;
            tx c_tx(acc);

            ta.emplace([&acc](int x)
                {
                    acc += x;
                });
            ta.emplace([&acc, my_tx = tx{acc} ](int)
                {
                    acc += 1;
                });
            ta.emplace([&acc](int x)
                {
                    acc -= x;
                });
            ta.emplace([&acc, c_tx](int)
                {
                    acc -= 1;
                });

            ta.call_all(5);
        }

        std::cout << acc << "\n";
        TEST_ASSERT(acc == 0);
    }
}

void copy_tests()
{
    {
        cypx_reset();
        std::cout << "a\n";
        cypx_test(0, 0, 0, 0);

        cpyx px;
        std::cout << "b\n";
        cypx_test(1, 0, 0, 0);

        int acc = 0;
        int one = 1;

        fixed_function_queue<void(int), 512> ta;

        ta.emplace([&acc, px](int x)
            {
                acc += x;
            });
        std::cout << "c\n";
        std::cout << copy_ctors << "\n";
        cypx_test(1, 2, 0, 1);


        ta.emplace([&acc, one, px](int)
            {
                acc += one;
            });
        std::cout << "d\n";
        cypx_test(1, 4, 0, 2);

        ta.emplace([&acc, lel = std::vector<int>{} ](int x)
            {
                acc -= x;
            });
        ta.emplace([&acc, one](int)
            {
                acc -= one;
            });



        ta.call_all(5);
        std::cout << acc << "\n";
        TEST_ASSERT(acc == 0);

        std::cout << "e\n";
        cypx_test(1, 4, 0, 2);
        acc = 0;
        one = 1;

        auto ta2 = ta;
        std::cout << "f\n";

        cypx_test(1, 6, 0, 2);
        ta2.call_all(5);
        std::cout << acc << "\n";
        TEST_ASSERT(acc == 0);
        std::cout << "g\n";
        cypx_test(1, 6, 0, 2);
    }

    cypx_test(1, 6, 0, 7);
}

void move_tests()
{
    {
        cypx_reset();
        std::cout << "a\n";
        cypx_test(0, 0, 0, 0);

        cpyxm px;
        std::cout << "b\n";
        cypx_test(1, 0, 0, 0);

        int acc = 0;
        int one = 1;

        fixed_function_queue<void(int), 512> ta;

        ta.emplace([&acc, px = std::move(px) ](int x)
            {
                acc += x;
            });
        std::cout << "c\n";
        std::cout << move_ctors << "\n";
        cypx_test(1, 0, 2, 1);


        ta.emplace([&acc, one, px = std::move(px) ](int)
            {
                acc += one;
            });
        std::cout << "d\n";
        cypx_test(1, 0, 4, 2);
        ta.emplace([&acc, lel = std::vector<int>{} ](int x)
            {
                acc -= x;
            });
        ta.emplace([&acc, one](int)
            {
                acc -= one;
            });



        ta.call_all(5);
        std::cout << acc << "\n";
        TEST_ASSERT(acc == 0);

        std::cout << "e\n";
        cypx_test(1, 0, 4, 2);
        acc = 0;
        one = 1;

        auto ta2 = std::move(ta);
        std::cout << "f\n";

        cypx_test(1, 0, 6, 2);
        ta2.call_all(5);
        std::cout << acc << "\n";
        TEST_ASSERT(acc == 0);
        std::cout << "g\n";

        cypx_test(1, 0, 6, 2);
    }

    cypx_test(1, 0, 6, 7);
}

int main()
{
    // std::cout << sizeof(void*) << "\n";
    // return 0;

    tests();
    copy_tests();
    move_tests();

    lmao xxx;

    fixed_function_queue<void(int), 512> f0;


    f0.emplace([ xxx, aaa = 100 ](int x)
        {
            std::cout << x << " hi!\n";
        });

    f0.emplace([ xxx, bbb = std::vector<int>{0} ](int x)
        {
            std::cout << x << " hi again!\n";
        });

    f0.emplace([&xxx](int x)
        {
            std::cout << x << " hi byref!\n";
        });


    f0.call_all(5);
}