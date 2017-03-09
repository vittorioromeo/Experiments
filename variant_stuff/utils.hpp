#pragma once

#include <experimental/type_traits>
#include <iostream>
#include <utility>

#ifndef FWD
#define FWD(...) ::std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)
#endif
