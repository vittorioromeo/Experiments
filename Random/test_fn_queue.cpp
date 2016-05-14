#include <cstddef>
#include <vector>
#include <array>
#include <iostream>
#include <utility>
#include <functional>
#include <vrm/core/assert.hpp>
#include <vrm/core/casts.hpp>

#if 1
#define ELOG(...) __VA_ARGS__
#else
#define ELOG(...)
#endif

template <typename T0, typename T1>
constexpr auto multiple_round_up(T0 x, T1 multiple) noexcept
{
    VRM_CORE_CONSTEXPR_ASSERT(multiple != 0);
    return ((x + multiple - 1) / multiple) * multiple;
}

template <typename TSignature>
using fn_ptr = TSignature*;

template <typename TSignature, std::size_t TBufferSize>
class fixed_function_queue;

template <typename TReturn, typename... TArgs, std::size_t TBufferSize>
class fixed_function_queue<TReturn(TArgs...), TBufferSize>
{
private:
    using byte = char;
    using return_type = TReturn;
    static constexpr auto buffer_size = TBufferSize;
    static constexpr auto alignment = alignof(std::max_align_t);

    template <typename T>
    constexpr auto round_up_to_alignment(T x) const noexcept
    {
        return multiple_round_up(x, alignment);
    }

    using call_fn_ptr = fn_ptr<return_type(byte*, TArgs...)>;
    using destroy_fn_ptr = fn_ptr<void(byte*)>;

    struct vtable
    {
        call_fn_ptr _call;
        destroy_fn_ptr _destroy;
    };

    // TODO: use this instead of vector
    static constexpr auto max_vtable_ptrs = 
        sizeof(vtable*) / buffer_size;

    std::aligned_storage_t<buffer_size, alignment> _buffer;
    std::vector<vtable*> _vtable_ptrs;
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
            std::cout << "emplacing vtable... (size: " << sizeof(vtable)
                      << ")\n"; // .
            );

        return aligned_placement_new<vtable>(ptr);
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
    void bind_vtable_to_fn(vtable& vt) noexcept
    {
        ELOG(                                         // .
            std::cout << "binding vtable to fn...\n"; // .
            );

        vt._call = [](byte* obj, TArgs... xs)
        {
            return reinterpret_cast<TF*>(obj)->operator()(xs...);
        };

        vt._destroy = [](byte* obj)
        {
            return reinterpret_cast<TF*>(obj)->~TF();
        };
    }

    void subscribe_vtable(vtable& vt)
    {
        _vtable_ptrs.emplace_back(&vt);
    }

    template <typename TF>
    auto emplace_starting_at(byte* ptr, TF&& f)
        // TODO: noexcept
    {
        ptr = emplace_vtable_at(ptr);
        auto& vt = *(reinterpret_cast<vtable*>(ptr));

        ELOG( // .
            std::cout << "vtable offset: " << offset_from_beginning(ptr)
                      << "\n"; // .
            );


        auto fn_start_ptr = ptr + sizeof(vtable);

        ELOG(                                                         // .
            std::cout << "fn start offset: "                          // .
                      << offset_from_beginning(fn_start_ptr) << "\n"; // .
            );

        ptr = emplace_fn_at(fn_start_ptr, FWD(f));

        ELOG(                                                        // .
            std::cout << "fn offset: " << offset_from_beginning(ptr) // .
                      << "\n";                                       // .
            );

        bind_vtable_to_fn<TF>(vt);
        subscribe_vtable(vt);

        _next = ptr + sizeof(TF);

        ELOG( // .
            std::cout << "stored next offset: " << offset_from_beginning(_next)
                      << "\n"; // .
            );
    }

    auto get_next_aligned_ptr(byte* ptr) noexcept
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

    auto get_fn_ptr_from_vtable(vtable* vt_ptr) noexcept
    {
        return get_next_aligned_ptr((byte*)vt_ptr + sizeof(vtable));
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
    void for_fns_reverse(TF&& f)
        // TODO: noexcept
    {
        for(auto itr = std::rbegin(_vtable_ptrs); itr != std::rend(_vtable_ptrs); ++itr)
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

                vt._destroy(fn_ptr);
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

    fixed_function_queue(const fixed_function_queue&) = delete;
    fixed_function_queue& operator=(const fixed_function_queue&) = delete;

    fixed_function_queue(fixed_function_queue&&) = delete;
    fixed_function_queue& operator=(fixed_function_queue&&) = delete;

    template <typename TF>
    void emplace(TF&& f)
        // TODO: noexcept
    {
        ELOG(                              // .
            std::cout << "emplacing...\n"; // .
            );

        emplace_starting_at(_next, FWD(f));

        ELOG(                                  // .
            std::cout << "done emplacing\n\n"; // .
            );
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
                vt._call(fn_ptr, xs...);
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

template<typename TSignature, std::size_t TBufferSize>
struct function_queue_test_adapter
{
private:
    std::vector<std::function<TSignature>> _fn_vec;
    fixed_function_queue<TSignature, TBufferSize> _fn_queue;

public:
    template<typename TF> 
    void emplace(TF&& f) 
    {  
        _fn_vec.emplace_back(FWD(f));
        _fn_queue.emplace(FWD(f));
    }

    template<typename... Ts>
    void call_all(Ts&&... xs)
    {
        for(auto& f : _fn_vec) 
        {
            f(FWD(xs)...);
        }

        _fn_queue.call_all(FWD(xs)...);
    }
};

void tests()
{
    #define TEST_ASSERT(...) \
        if(!(__VA_ARGS__)) \
        { \
            std::terminate(); \
        }

    {
        int acc = 0;
        int one = 1;

        function_queue_test_adapter<void(int), 128> ta;

        ta.emplace([&acc](int x){ acc += x; });
        ta.emplace([&acc, one](int){ acc += one; });
        ta.emplace([&acc](int x){ acc -= x; });
        ta.emplace([&acc, one](int x){ acc -= one; });

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

            tx(int& acc_ref) : _acc_ref(acc_ref) { ++_acc_ref; }
            tx(const tx& x) : _acc_ref(x._acc_ref), _dec{false}{ }

            ~tx() { if(_dec) { --_acc_ref; } }
        };

        {
            function_queue_test_adapter<void(int), 256> ta;
            tx c_tx(acc);

            ta.emplace([&acc](int x){ acc += x; });
            ta.emplace([&acc, my_tx = tx{acc}](int){ acc += 1; });
            ta.emplace([&acc](int x){ acc -= x; });
            ta.emplace([&acc, c_tx](int){ acc -= 1; });

            ta.call_all(5);
        }

        std::cout << acc << "\n";
        TEST_ASSERT(acc == 0);
    }   
}

int main()
{
tests();

    lmao xxx;

    fixed_function_queue<void(int), 512> f0;


    f0.emplace([ xxx, aaa = 100 ](int x)
        {
            std::cout << x <<  " hi!\n";
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