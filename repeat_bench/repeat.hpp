/*

statistics:

*) project: 1000 .cpp files with 20 DEFINE_ME_A_FUNC functions each
*) compiler used: VS 2017 15.6.5
*) FASTBuild

debug build, no optimizations, /Od
  for-loop:			3.674s
  repeat template:	4.927s

release build, optimizations, /O2 /Gm- /Zc:inline /Gd /Oy-
  for-loop:			3.735s
  repeat template:	5.770s

.obj file size:
  for-loop: 	8kb
  repeat loop: 	54kb

*/

#pragma once

#define CONCATENATE_HELPER_HELPER(_a, _b) _a##_b
#define CONCATENATE_HELPER(_a, _b) CONCATENATE_HELPER_HELPER(_a, _b)
#define CONCATENATE(_a, _b) CONCATENATE_HELPER(_a, _b)
#define IDENTIFIER(_identifier) CONCATENATE(_identifier, __LINE__)

inline void printf(const char*, ...)
{
}

#if 1

#define DEFINE_ME_A_FUNC                   \
    inline void IDENTIFIER(TestFunc)(void) \
    {                                      \
        for(int i = 0; i < 100; ++i)       \
        {                                  \
            printf("value: %d", i);        \
        }                                  \
    }

#else

template <typename N, typename F>
inline constexpr void repeat(N n, F&& f)
{
    for(N i = 0; i < n; ++i)
    {
        f(i);
    }
}

#define DEFINE_ME_A_FUNC                                    \
    inline void IDENTIFIER(TestFunc)(void)                  \
    {                                                       \
        repeat(100, [](int i) { printf("value: %d", i); }); \
    }

#endif
