#pragma once

#include <experimental/type_traits>
#include <utility>
#include <iostream>

#define FWD(...) ::std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)
