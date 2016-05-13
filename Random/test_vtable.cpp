#include <cstddef>
#include <vector>
#include <iostream>
#include <utility>
#include <vrm/core/casts.hpp>

int roundUp(int numToRound, int multiple) 
{
    assert(multiple);
    return ((numToRound + multiple - 1) / multiple) * multiple;
}

template <typename TReturn, typename... TArgs>
using fn_ptr = TReturn (*)(TArgs...);

template <typename TSignature, std::size_t TBufferSize>
class fixed_function;

template <typename TReturn, typename... TArgs, std::size_t TBufferSize>
class fixed_function<TReturn(TArgs...), TBufferSize>
{
private:
    using byte = char;

    struct vtable
    {
        fn_ptr<TReturn, byte*, TArgs...> _call;
        fn_ptr<void, byte*> _destroy;
    };

    static constexpr auto buffer_size = TBufferSize;
    static constexpr auto alignment = alignof(std::max_align_t);
    std::aligned_storage_t<buffer_size, alignment> _buffer;

public:
    template <typename TF>
    void set(TF&& f)
    {
        new((char*)&_buffer) vtable;
        auto& vt = vrm::core::storage_cast<vtable>(_buffer);

        vt._call = [](byte* obj, TArgs... xs)
        {
            return reinterpret_cast<TF*>(obj)->operator()(xs...);
        };

        vt._destroy = [](byte* obj)
        {
            return reinterpret_cast<TF*>(obj)->~TF();
        };

        static_assert(sizeof(vtable) < alignment * 2, "");
        new((char*)&_buffer + alignment * 2) TF{FWD(f)};
    }

    auto call(TArgs... xs)
    {
        auto obj_ptr = &_buffer + alignment * 2;
        auto& vt = vrm::core::storage_cast<vtable>(_buffer);

        return (*vt._call)((char*)obj_ptr, xs...);
    }

    ~fixed_function()
    {
        auto& vt = vrm::core::storage_cast<vtable>(_buffer);
        auto obj_ptr = &_buffer + alignment * 2;

        (*vt._destroy)((char*)obj_ptr);
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

    auto l0 = [xxx]
    {
        std::cout << "hi!\n";
    };

    fixed_function<void(), 256> f0;

    std::cout << sizeof(l0) << "\n";

    f0.set(std::move(l0));
    f0.call();
}