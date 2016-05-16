constexpr auto compute_left()
{
    detail::array<std::size_t, left_size> indices{};
    std::size_t* left = &indices[0];

    for(std::size_t i = 0; i < sizeof...(B); ++i)
        if(results[i])
            *left++ = i;

    return indices;
}

constexpr auto left = compute_left();

template <typename TContainer, std::size_t... l>
constexpr auto apply(TContainer c, std::index_sequence<l...>)
{
    return hana::make_some_sequence(hana::at_c<left[l]>(c)...);
}

