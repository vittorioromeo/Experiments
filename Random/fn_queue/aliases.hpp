#pragma once

#include "./dependencies.hpp"

/// @brief Alias for a function pointer.
template <typename TSignature>
using fn_ptr = TSignature*;

/// @brief Alias for `boost::hana` namespace.
namespace bh = boost::hana;

/// @brief Alias for `vrm::core` namespace.
namespace vrmc = vrm::core;
