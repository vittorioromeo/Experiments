// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vrm/sdl/internal.hpp>
#include <vrm/sdl/dependencies/standard.hpp>

#include <vrm/core.hpp>
namespace vrmc = vrm::core;

#ifdef __EMSCRIPTEN__
#include <vrm/sdl/dependencies/emscripten.hpp>
#else
#include <vrm/sdl/dependencies/regular.hpp>
#endif
