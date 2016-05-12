#include <cstddef>
#include <vector>
#include <iostream>
#include <utility>
#include <vrm/core/casts.hpp>

template <typename TSignature, std::size_t TBufferSize>
class fixed_function_queue;

template <typename TReturn, typename... TArgs, std::size_t TBufferSize>
class fixed_function_queue<TReturn(TArgs...), TBufferSize>
{
public:
    using return_type = TReturn;
    using fn_ptr_type = TReturn (*)(TArgs...);

    static constexpr auto buffer_size = TBufferSize;

private:
    using byte = char;

    std::aligned_storage_t<buffer_size, alignof(std::max_align_t)> _buffer;
    // std::vector<fn_ptr> _fn_ptrs;

    template <typename TF>
    void impl_store_fn(byte* p, TF&& f)
    {
    }

    template <typename TF>
    void store_caller(TF&& f)
    {
        // store caller in buffer
        // get fn ptr to caller
        // store fn ptr in some vector
    }

    template <typename TF>
    void store_destroyer(TF&& f)
    {
        // store destroyer in buffer
        // get fn ptr to destroyer
        // store fn ptr in some vector
    }

    template <typename TF>
    void store_callable(TF&& f)
    {
        // store callable in buffer
        // get fn ptr to callable
        // store fn ptr in some vector
    }

    auto get_caller_ptr_from_beginning_ptr(byte* p) noexcept
    {
        return p;
    }

    auto get_destroyer_ptr_from_beginning_ptr(byte* p) noexcept
    {
        constexpr auto caller_size = 1;
        return get_caller_ptr_from_beginning_ptr(p) + caller_size;
    }

    auto get_function_ptr_from_beginning_ptr(byte* p) noexcept
    {
        constexpr auto destroyer_size = 1;
        return get_destroyer_ptr_from_beginning_ptr(p) + destroyer_size;
    }

    void destroy_all()
    {
        // for every beginning fn_ptr...
        // get destroyer and call it after calculating real function address
    }

public:
    fixed_function_queue() noexcept
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
        using callable_type = std::decay_t<TF>;
        constexpr auto callable_size = sizeof(callable_type);

        auto f_caller = [](byte* ptr, TArgs... xs) -> return_type
        {
            return (reinterpret_cast<TF*>(ptr))->operator()(xs...);
        };

        auto f_destroyer = [](byte* ptr)
        {
            reinterpret_cast<TF*>(ptr)->~TF();
        };

        store_caller(std::move(f_caller));
        store_destroyer(std::move(f_destroyer));
        store_callable(std::move(f));
    }

    void call_all(TArgs... xs)
    {
        // for every beginning fn_ptr...
        // get caller and call it after calculating real function address
    }
};

int main()
{
}