// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vrm/sdl/common.hpp>

VRM_SDL_NAMESPACE
{
    template <typename T, std::size_t TSize>
    class fixed_sparse_int_set
    {
    private:
        std::vector<T> _dense;
        std::vector<T*> _sparse;
        T* _end;

    public:
        fixed_sparse_int_set() noexcept
        {
            _dense.resize(TSize);
            _sparse.resize(TSize);
            clear();
        }

        fixed_sparse_int_set(const fixed_sparse_int_set& rhs)
            : _dense(rhs._dense), _sparse(rhs._sparse),
              _end(_dense.data() + rhs.size())
        {
        }

        fixed_sparse_int_set& operator=(const fixed_sparse_int_set& rhs)
        {
            _dense = rhs._dense;
            _sparse = rhs._sparse;
            _end = _dense.data() + rhs.size();

            return *this;
        }

        fixed_sparse_int_set(fixed_sparse_int_set&&) = default;
        fixed_sparse_int_set& operator=(fixed_sparse_int_set&&) = default;

        bool has(const T& x) const noexcept
        {
            assert(x < TSize);
            return _sparse[x] != nullptr;
        }

        bool add(const T& x) noexcept
        {
            assert(x < TSize);
            if(has(x)) return false;

            assert(size() < TSize);
            *_end = x;

            _sparse[x] = _end;
            ++_end;

            return true;
        }

        bool erase(const T& x) noexcept
        {
            assert(x < TSize);
            if(!has(x)) return false;

            auto& ptr(_sparse[x]);
            assert(size() > 0);

            auto last(back());
            assert(ptr != nullptr);

            if(*ptr != last)
            {
                *ptr = last;
                _sparse[last] = ptr;
            }

            assert(has(x));
            ptr = nullptr;

            assert(size() > 0);
            --_end;

            return true;
        }

        void clear() noexcept
        {
            for(auto& p : _sparse) p = nullptr;
            _end = _dense.data();
        }

        bool empty() const noexcept { return _end == _dense.data(); }

        void pop_back() noexcept
        {
            assert(size() > 0);
            erase(back());
        }

        auto back() const noexcept
        {
            assert(size() > 0);

            assert(has(*(_end - 1)));
            return *(_end - 1);
        }

        template <typename TF>
        void for_each(TF&& f) const noexcept
        {
            assert(size() <= TSize);

            for(auto p(_dense.data()); p != _end; ++p)
            {
                assert(has(*p));
                f(*p);
            }
        }

        auto operator[](std::size_t i) const noexcept
        {
            assert(i < size());

            assert(has(_dense[i]));
            return _dense[i];
        }

        auto size() const noexcept
        {
            return to_num<sdl::sz_t>(end() - begin());
        }

        decltype(auto) begin() noexcept { return _dense.data(); }
        decltype(auto) begin() const noexcept { return _dense.data(); }

        decltype(auto) end() noexcept { return _end; }
        decltype(auto) end() const noexcept { return _end; }
    };
}
VRM_SDL_NAMESPACE_END