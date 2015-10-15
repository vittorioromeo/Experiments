template <typename T, std::size_t TSize>
class sparse_int_set
{
private:
    static constexpr std::size_t null_value{TSize + 1};

    std::vector<T> _dense;
    std::vector<std::size_t> _sparse;
    std::size_t _size;

    // std::array<T, TSize> _dense;
    // std::array<std::size_t, TSize> _sparse;

public:
    sparse_int_set() noexcept
    {
        _dense.resize(TSize);
        _sparse.resize(TSize);
        clear();
    }

    sparse_int_set(const sparse_int_set& rhs)
        : _dense(rhs._dense), _sparse(rhs._sparse), _size(rhs._size)
    {
    }

    sparse_int_set& operator=(const sparse_int_set& rhs)
    {
        _dense = rhs._dense;
        _sparse = rhs._sparse;
        _size = rhs._size;

        return *this;
    }

    sparse_int_set(sparse_int_set&&) = default;
    sparse_int_set& operator=(sparse_int_set&&) = default;

    bool has(const T& x) const noexcept
    {
        assert(x < TSize);
        return _sparse[x] != null_value;
    }

    bool add(const T& x) noexcept
    {
        assert(x < TSize);
        if(has(x)) return false;

        assert(_size < TSize);
        _dense[_size] = x;

        _sparse[x] = _size;
        ++_size;

        return true;
    }

    bool erase(const T& x) noexcept
    {
        assert(x < TSize);
        if(!has(x)) return false;

        auto ptr(_sparse[x]);
        assert(_size > 0);

        assert(has(_dense[_size - 1]));
        auto last(_dense[_size - 1]);
        assert(ptr != null_value);

        if(_dense[ptr] != last)
        {
            _dense[ptr] = last;
            _sparse[last] = ptr;
        }

        assert(has(x));
        _sparse[x] = null_value;

        assert(_size > 0);
        --_size;

        return true;
    }

    void clear() noexcept
    {
        for(auto& p : _sparse) p = null_value;
        _size = 0;
    }

    bool empty() const noexcept { return _size == 0; }

    void pop_back() noexcept
    {
        assert(_size > 0);
        erase(back());
    }

    auto back() const noexcept
    {
        assert(_size > 0);

        assert(has(_dense[_size - 1]));
        return _dense[_size - 1];
    }

    template <typename TF>
    void for_each(TF&& f) const noexcept
    {
        assert(_size <= TSize);

        for(decltype(_size) i(0); i < _size; ++i)
        {
            assert(has(_dense[i]));
            f(_dense[i]);
        }
    }

    auto operator[](std::size_t i) const noexcept
    {
        assert(i < _size);

        assert(has(_dense[i]));
        return _dense[i];
    }

    auto size() const noexcept { return _size; }
};