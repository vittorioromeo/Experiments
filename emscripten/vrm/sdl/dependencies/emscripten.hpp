// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#ifdef __EMSCRIPTEN__

#include <emscripten.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengles2.h>

#define GLM_FORCE_RADIANS
#define GLM_SWIZZLE
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/epsilon.hpp>
#include <gtc/constants.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/random.hpp>
#include <gtx/transform.hpp>
#include <gtx/matrix_interpolation.hpp>
#include <gtx/matrix_decompose.hpp>
#include <gtx/color_space.hpp>
#include <gtx/string_cast.hpp>

#endif