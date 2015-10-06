#include <iostream>
#include <vector>
#include <iterator>
#include <utility>
#include <tuple>

template <typename TItr>
class Range
{
private:
    TItr itrBegin, itrEnd;

public:
    inline constexpr Range(const TItr& mItrBegin, const TItr& mItrEnd) noexcept
        : itrBegin{mItrBegin},
          itrEnd{mItrEnd}
    {
    }

    inline constexpr TItr begin() const noexcept { return itrBegin; }
    inline constexpr TItr end() const noexcept { return itrEnd; }
};

template <typename T, typename... TArgs>
class RangeZip
{
};

template <typename T>
inline constexpr auto getReverseRange(const T& mContainer) noexcept
    -> Range<decltype(mContainer.rbegin())>
{
    return {mContainer.rbegin(), mContainer.rend()};
}

template <typename T>
inline constexpr auto getHalfRange(const T& mContainer) noexcept
    -> Range<decltype(mContainer.begin())>
{
    return {mContainer.begin(),
        mContainer.begin() + (mContainer.end() - mContainer.begin()) / 2};
}

int main()
{
    std::vector<int> test{1, 2, 3, 4};
    for(const auto& i : getReverseRange(test)) std::cout << i << std::endl;
    for(const auto& i : getHalfRange(test)) std::cout << i << std::endl;
}