#include <functional>
#include <iostream>
#include <vector>
#include <memory>
#include <cstddef>
#include <new>
#include <chrono>

#define FWD(...) ::std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)

inline void escape(void* p)
{
    asm volatile("" : : "g"(p) : "memory");
}

using sz_t = std::size_t;
namespace ex
{
    using byte = char;

    template <typename TSignature>
    using fn_ptr = TSignature*;


    // Stores functions of `void(TArgs...)` signature.
    template <typename... TArgs>
    class fn_queue
    {
    private:
        using position = sz_t;
        // using move_fn_ptr = fn_ptr<void(byte*, byte*)>;
        using dtor_fn_ptr = fn_ptr<void(byte*)>;
        using call_fn_ptr = fn_ptr<void(byte*, TArgs...)>;

        // std::vector<move_fn_ptr> _move_fn_ptrs;
        std::vector<dtor_fn_ptr> _dtor_fn_ptrs;
        std::vector<call_fn_ptr> _call_fn_ptrs;
        std::vector<position> _positions;
        std::vector<byte> _buffer;
        sz_t _next{0}, _capacity{0};

        void exec_all_dtors()
        {
            for(sz_t i(0); i < _next; ++i)
            {
                const auto& ith_dtor_fn_ptr(_dtor_fn_ptrs[i]);
                const auto& ith_position(_positions[i]);

                (*ith_dtor_fn_ptr)(_buffer.data() + ith_position);
            }
        }

        template <typename... Ts>
        void exec_all_calls(Ts&&... xs)
        {
            for(sz_t i(0); i < _next; ++i)
            {
                const auto& ith_call_fn_ptr(_call_fn_ptrs[i]);
                const auto& ith_position(_positions[i]);

                (*ith_call_fn_ptr)(_buffer.data() + ith_position, FWD(xs)...);
            }
        }

        void grow()
        {
            _capacity = 10 + (_capacity * 2);

            _dtor_fn_ptrs.resize(_capacity);
            _call_fn_ptrs.resize(_capacity);
            _positions.resize(_capacity);
        }

    public:
        ~fn_queue()
        {
            clear();
        }

        void clear()
        {
            exec_all_dtors();
            _next = 0;

            // _move_fn_ptrs.clear();
            // _dtor_fn_ptrs.clear();
            // _call_fn_ptrs.clear();
            // _positions.clear();
            // _buffer.clear();
        }

        template <typename... Ts>
        void call_all(Ts&&... xs)
        {
            exec_all_calls(FWD(xs)...);
        }

        template <typename TF>
        void emplace_back(TF&& f)
        {
            if(_capacity <= _next)
            {
                grow();
            }

            auto itr = _buffer.insert(
                std::end(_buffer), (char*)&f, (char*)&f + sizeof(TF));

            auto offset = itr - std::begin(_buffer);


            _dtor_fn_ptrs[_next] = [](byte* pos)
            {
                reinterpret_cast<TF&>(*pos).~TF();
            };

            _call_fn_ptrs[_next] = [](byte* pos, TArgs... as)
            {
                reinterpret_cast<TF&> (*pos)(as...);
            };

            _positions[_next] = offset;

            ++_next;
        }
    };

    template <typename... TArgs>
    class fn_queue1
    {
    private:
        using position = sz_t;
        // using move_fn_ptr = fn_ptr<void(byte*, byte*)>;
        using dtor_fn_ptr = fn_ptr<void(byte*)>;
        using call_fn_ptr = fn_ptr<void(byte*, TArgs...)>;

        struct data
        {
            position _p;
            dtor_fn_ptr _dfp;
            call_fn_ptr _cfp;
        };

        // std::vector<move_fn_ptr> _move_fn_ptrs;
        std::vector<data> _datas;
        std::vector<byte> _buffer;
        sz_t _next{0}, _capacity{0};

        void exec_all_dtors()
        {
            for(sz_t i(0); i < _next; ++i)
            {
                const auto& ith_data(_datas[i]);
                const auto& ith_position(ith_data._p);
                const auto& ith_dtor_fn_ptr(ith_data._dfp);

                (*ith_dtor_fn_ptr)(_buffer.data() + ith_position);
            }
        }

        template <typename... Ts>
        void exec_all_calls(Ts&&... xs)
        {
            for(sz_t i(0); i < _next; ++i)
            {
                const auto& ith_data(_datas[i]);
                const auto& ith_position(ith_data._p);
                const auto& ith_call_fn_ptr(ith_data._cfp);

                (*ith_call_fn_ptr)(_buffer.data() + ith_position, FWD(xs)...);
            }
        }

        void grow()
        {
            _capacity = 10 + (_capacity * 2);
            _datas.resize(_capacity);
        }

    public:
        ~fn_queue1()
        {
            clear();
        }

        void clear()
        {
            exec_all_dtors();
            _next = 0;

            // _move_fn_ptrs.clear();
            // _dtor_fn_ptrs.clear();
            // _call_fn_ptrs.clear();
            // _positions.clear();
            // _buffer.clear();
        }

        template <typename... Ts>
        void call_all(Ts&&... xs)
        {
            exec_all_calls(FWD(xs)...);
        }

        template <typename TF>
        void emplace_back(TF&& f)
        {
            if(_capacity <= _next)
            {
                grow();
                _buffer.reserve(100000);
            }

            auto itr = _buffer.insert(
                std::end(_buffer), (char*)&f, (char*)&f + sizeof(TF));

            auto offset = itr - std::begin(_buffer);

            auto& d = _datas[_next];

            d._p = offset;

            d._dfp = [](byte* pos)
            {
                reinterpret_cast<TF&>(*pos).~TF();
            };

            d._cfp = [](byte* pos, TArgs... as)
            {
                reinterpret_cast<TF&> (*pos)(as...);
            };

            ++_next;
        }
    };
}

using hr_clock = std::chrono::high_resolution_clock;

template <typename TF>
void bench(const std::string& title, sz_t times, TF&& f)
{
    std::vector<float> mss;

    for(sz_t i(0); i < times; ++i)
    {
        auto start = hr_clock::now();
        {
            f();
        }
        auto end = hr_clock::now();

        auto dur = end - start;
        auto cnt =
            std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();

        mss.emplace_back(cnt);
    }

    float mean = 0.f;
    for(auto x : mss)
    {
        mean += x;
    }
    mean /= mss.size();

    std::cout << title << " | " << mean << " ms\n";
}

template <typename... TArgs>
struct adapter
{

    std::vector<std::function<void(TArgs...)>> _v;

    template <typename TF>
    void emplace_back(TF&& f)
    {
        _v.emplace_back(FWD(f));
    }

    template <typename... TXs>
    void call_all(TXs&&... xs)
    {
        for(auto& f : _v)
        {
            f(FWD(xs)...);
        }
    }

    void clear()
    {
        _v.clear();
    }
};

template <typename TQ>
void test0()
{
    int state = 0;
    TQ q;

    constexpr sz_t loops0 = 100;
    constexpr sz_t loops1 = 100;
    constexpr sz_t loops2 = 100;

    for(sz_t i0 = 0; i0 < loops0; ++i0)
    {
        q.clear();

        for(sz_t i1 = 0; i1 < loops1; ++i1)
        {
            q.emplace_back([](int)
                {

                });

            q.emplace_back([&state](int x)
                {
                    state += x;
                });

            q.emplace_back([&state, k = 0 ](int x)
                {
                    state += x;
                });

            q.emplace_back([&state, k0 = 0, k1 = 1, k2 = 2 ](int x) mutable
                {
                    state += x + k0++;
                });

            escape(&state);
        }

        escape(&q);

        for(sz_t i2 = 0; i2 < loops2; ++i2)
        {
            q.call_all(0);
            q.call_all(1);
            q.call_all(2);
        }

        escape(&q);
    }

    escape(&q);
}

int main()
{
    using t0 = ex::fn_queue<int>;
    using t1 = ex::fn_queue1<int>;
    using t2 = adapter<int>;

    constexpr sz_t times = 70;

    bench("fn_queue0", times, []
        {
            test0<t0>();
        });

    bench("fn_queue1", times, []
        {
            test0<t1>();
        });

    bench("vector<_>", times, []
        {
            test0<t2>();
        });


    return 0;
}
