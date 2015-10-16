// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

// From:
// https://github.com/SFML/SFML/blob/master/src/SFML/Graphics/GLCheck.cpp

////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2015 Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the
// use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#pragma once

#include <vrm/sdl/common.hpp>

#ifndef NDEBUG

#define VRM_SDL_GLCHECK(...)                                                \
    do                                                                      \
    {                                                                       \
        __VA_ARGS__;                                                        \
        ::vrm::sdl::impl::gl_check_error(__FILE__, __LINE__, #__VA_ARGS__); \
    } while(false)

#else

#define VRM_SDL_GLCHECK(...) \
    do                       \
    {                        \
        __VA_ARGS__;         \
    } while(false)

#endif

VRM_SDL_NAMESPACE
{
    namespace impl
    {
        void gl_check_error(
            const char* file, unsigned int line, const char* expression)
        {
            auto errorCode(glGetError());
            if(errorCode == GL_NO_ERROR) return;

            std::string fileString = file;
            std::string error = "Unknown error";
            std::string description = "No description";

            // Decode the error code
            switch(errorCode)
            {
                case GL_INVALID_ENUM:
                {
                    error = "GL_INVALID_ENUM";
                    description =
                        "An unacceptable value has been specified for "
                        "an enumerated argument.";
                    break;
                }

                case GL_INVALID_VALUE:
                {
                    error = "GL_INVALID_VALUE";
                    description = "A numeric argument is out of range.";
                    break;
                }

                case GL_INVALID_OPERATION:
                {
                    error = "GL_INVALID_OPERATION";
                    description =
                        "The specified operation is not allowed in the "
                        "current state.";
                    break;
                }

                case GL_OUT_OF_MEMORY:
                {
                    error = "GL_OUT_OF_MEMORY";
                    description =
                        "There is not enough memory left to execute "
                        "the command.";
                    break;
                }
            }

            // Log the error
            std::cout << "An internal OpenGL call failed in "
                      << fileString.substr(fileString.find_last_of("\\/") + 1)
                      << "(" << line << ")."
                      << "\nExpression:\n   " << expression
                      << "\nError description:\n   " << error << "\n   "
                      << description << "\n" << std::endl;

            int temp;
            std::cin >> temp;
        }
    }
}
VRM_SDL_NAMESPACE_END