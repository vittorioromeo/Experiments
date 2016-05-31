#pragma once

#include "./dependencies.hpp"

// Debug logging.
#if 0
#define ELOG(...) __VA_ARGS__
#else
#define ELOG(...)
#endif
