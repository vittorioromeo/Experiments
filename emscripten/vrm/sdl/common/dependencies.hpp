// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <cstddef>
#include <functional>
#include <chrono>
#include <memory>
#include <bitset>
#include <algorithm>
#include <unordered_map>
#include <string>
#include <cassert>
#include <vector>
#include <type_traits>

// #define GLEW_STATIC
// #include <GL/glew.h>

#ifdef __EMSCRIPTEN__

#include <emscripten.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_opengles2.h>

#else

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#endif
