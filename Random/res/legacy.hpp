// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./shared.hpp"

// Free store
namespace legacy
{
    template <typename T>
    auto free_store_new(T* ptr)
    {
        std::cout << "free_store_new\n";
        return ptr;
    }

    template <typename T>
    void free_store_delete(T* ptr)
    {
        if(ptr == nullptr)
        {
            // Do nothing.
        }
        else
        {
            std::cout << "free_store_delete\n";
        }

        delete ptr;
    }
}

// GL
namespace legacy
{
    using GLsizei = std::size_t;
    using GLuint = int;

    void glGenBuffers(GLsizei n, GLuint* ptr)
    {
        static GLuint next_id{1};

        std::cout << "glGenBuffers(" << n << ", ptr) -> " << next_id << "\n";

        *ptr = next_id++;
    }

    void glDeleteBuffers(GLsizei n, const GLuint* ptr)
    {
        if(*ptr == 0)
        {
            // Do nothing.
        }
        else
        {
            std::cout << "glDeleteBuffers(" << n << ", " << *ptr << ")\n";
        }
    }
}

// File
namespace legacy
{
    int open_file()
    {
        static int next_id(1);

        std::cout << "open_file() -> " << next_id << "\n";

        return next_id++;
    }

    void close_file(int id)
    {
        if(id == -1)
        {
            // Do nothing.
        }
        else
        {
            std::cout << "close_file(" << id << ")\n";
        }
    }
}
