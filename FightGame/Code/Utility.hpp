#pragma once

constexpr float pxCrRatio{100.f};
template <typename T>
inline constexpr float toPx(T mValue)
{
    return mValue / pxCrRatio;
}
template <typename T>
inline constexpr int toCr(T mValue)
{
    return mValue * pxCrRatio;
}
template <typename T>
inline ssvs::Vec2f toPx(const ssvs::Vec2<T>& mValue)
{
    return {toPx(mValue.x), toPx(mValue.y)};
}
template <typename T>
inline ssvs::Vec2i toCr(const ssvs::Vec2<T>& mValue)
{
    return {toCr(mValue.x), toCr(mValue.y)};
}
template <typename T>
inline ssvs::Vec2f toPx(T mX, T mY)
{
    return {toPx(mX), toPx(mY)};
}
template <typename T>
inline ssvs::Vec2i toCr(T mX, T mY)
{
    return {toCr(mX), toCr(mY)};
}
