#include <cstddef>
#include <vector>
#include <iostream>
#include <utility>
#include <vrm/core/casts.hpp>

#if 0
#define ELOG(...) __VA_ARGS__
#else
#define ELOG(...)
#endif

int roundUp(int numToRound, int multiple)
{
    assert(multiple);
    return ((numToRound + multiple - 1) / multiple) * multiple;
}

template <typename TReturn, typename... TArgs>
using fn_ptr = TReturn (*)(TArgs...);

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
        return roundUp(x, alignment);
    }

    struct vtable
    {
        fn_ptr<TReturn, byte*, TArgs...> _call;
        fn_ptr<void, byte*> _destroy;
    };

    std::aligned_storage_t<buffer_size, alignment> _buffer;
    std::vector<vtable*> _vtable_ptrs;
    byte* _next;

    auto offset_from_beginning(byte* ptr) const noexcept
    {
        return ptr - (byte*)&_buffer;
    }

    template <typename T>
    auto buffer_ptr_from_offset(T x) const noexcept
    {
        return (byte*)&_buffer + x;
    }

    auto emplace_vtable_at(byte* ptr)
    {
        ELOG(                                     // .
            std::cout << "emplacing vtable...\n"; // .
            );

        ptr = get_next_aligned_ptr(ptr);
        new(ptr) vtable;
        return ptr;
    }

    template <typename TF>
    auto emplace_fn_at(byte* ptr, TF&& f)
    {
        ELOG(                                                              // .
            std::cout << "emplacing fn... (size: " << sizeof(TF) << ")\n"; // .
            );

        ptr = get_next_aligned_ptr(ptr);
        new(ptr) TF{FWD(f)};
        return ptr;
    }

    template <typename TF>
    void bind_vtable_to_fn(vtable& vt)
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
    {
        ptr = emplace_vtable_at(ptr);
        auto& vt = *(reinterpret_cast<vtable*>(ptr));

        ELOG( // .
            std::cout << "vtable offset: " << offset_from_beginning(ptr)
                      << "\n"; // .
            );


        auto fn_start_ptr = ptr + sizeof(vtable);

        ELOG( // .
            std::cout << "fn start offset: "
                      << offset_from_beginning(fn_start_ptr) << "\n"; // .
            );

        ptr = emplace_fn_at(fn_start_ptr, FWD(f));        

        ELOG( // .
            std::cout << "fn offset: " << offset_from_beginning(ptr)
                      << "\n"; // .
            );

        bind_vtable_to_fn<TF>(vt);
        subscribe_vtable(vt);

        _next = ptr;

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
    {
        for(auto vt_ptr : _vtable_ptrs)
        {
            auto fn_ptr = get_fn_ptr_from_vtable(vt_ptr);
            f(*vt_ptr, (byte*)fn_ptr);
        }
    }

    void destroy_all()
    {
        for_fns([](auto& vt, auto fn_ptr)
            {
                vt._destroy(fn_ptr);
            });
    }

public:
    fixed_function_queue() noexcept : _next{(byte*)&_buffer}
    {
    }

    ~fixed_function_queue()
    {
        destroy_all();
    }

    fixed_function_queue(const fixed_function_queue&) = delete;
    fixed_function_queue& operator=(const fixed_function_queue&) = delete;

    fixed_function_queue(fixed_function_queue&&) = delete;
    fixed_function_queue& operator=(fixed_function_queue&&) = delete;

    template <typename TF>
    void emplace(TF&& f)
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
    {
        for_fns([&xs...](auto& vt, auto fn_ptr)
            {
                vt._call(fn_ptr, xs...);
            });
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

int main()
{
    lmao xxx;

    fixed_function_queue<void(), 256> f0;


    f0.emplace([xxx]
        {
            std::cout << "hi!\n";
        });

    f0.emplace([xxx]
        {
            std::cout << "hi again!\n";
        });


    f0.call_all();
}