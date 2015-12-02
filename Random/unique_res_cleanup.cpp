// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#include <iostream>
#include <memory>

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

namespace behavior
{
    template <typename T>
    struct free_store_b
    {
        using handle_type = T*;

        handle_type null_handle()
        {
            return nullptr;
        }

        handle_type init(T* ptr)
        {
            return legacy::free_store_new<T>(ptr);
        }

        void deinit(const handle_type& handle)
        {
            legacy::free_store_delete(handle);
        }
    };

    struct vbo_b
    {
        struct vbo_handle
        {
            legacy::GLuint _id;
            legacy::GLsizei _n;
        };

        using handle_type = vbo_handle;

        handle_type null_handle()
        {
            return {0, 0};
        }

        handle_type init(std::size_t n)
        {
            handle_type result;

            legacy::glGenBuffers(n, &result._id);
            result._n = n;

            return result;
        }

        void deinit(const handle_type& handle)
        {
            legacy::glDeleteBuffers(handle._n, &handle._id);
        }
    };

    struct file_b
    {
        using handle_type = int;

        handle_type null_handle()
        {
            return -1;
        }

        handle_type init()
        {
            return legacy::open_file();
        }

        void deinit(const handle_type& handle)
        {
            legacy::close_file(handle);
        }
    };
}

namespace resource
{
    template <typename TBehavior>
    class unique : TBehavior
    {
    public:
        using behavior_type = TBehavior;
        using handle_type = typename behavior_type::handle_type;

    private:
        handle_type _handle;

        auto& as_behavior() noexcept;
        const auto& as_behavior() const noexcept;

    public:
        unique() noexcept;
        ~unique() noexcept;

        unique(const unique&) = delete;
        unique& operator=(const unique&) = delete;

        explicit unique(const handle_type& handle) noexcept;

        unique(unique&& rhs) noexcept;
        auto& operator=(unique&&) noexcept;

        auto release() noexcept;

        void reset() noexcept;
        void reset(const handle_type& handle) noexcept;

        void swap(unique& rhs) noexcept;

        auto get() const noexcept;

        explicit operator bool() const noexcept;

        template<typename>
        friend bool operator==(const unique& lhs, const unique& rhs) noexcept;
        
        template<typename>
        friend bool operator!=(const unique& lhs, const unique& rhs) noexcept;
        
        template<typename>
        friend void swap(unique& lhs, unique& rhs) noexcept;
    };

    template <typename TBehavior>
    auto& unique<TBehavior>::as_behavior() noexcept
    {
        return static_cast<behavior_type&>(*this);
    }

    template <typename TBehavior>
    const auto& unique<TBehavior>::as_behavior() const noexcept
    {
        return static_cast<const behavior_type&>(*this);
    }

    template <typename TBehavior>
    unique<TBehavior>::unique() noexcept : _handle{as_behavior().null_handle()}
    {
    }

    template <typename TBehavior>
    unique<TBehavior>::~unique() noexcept
    {
        reset();
    }

    template <typename TBehavior>
    unique<TBehavior>::unique(const handle_type& handle) noexcept
        : _handle{handle}
    {
    }

    template <typename TBehavior>
    unique<TBehavior>::unique(unique&& rhs) noexcept : _handle{rhs.release()}
    {
    }

    template <typename TBehavior>
    auto& unique<TBehavior>::operator=(unique&& rhs) noexcept
    {
        reset(rhs.release());
        return *this;
    }

    template <typename TBehavior>
    auto unique<TBehavior>::release() noexcept
    {
        auto temp_handle(_handle);
        _handle = as_behavior().null_handle();
        return temp_handle;
    }

    template <typename TBehavior>
    void unique<TBehavior>::reset() noexcept
    {
        as_behavior().deinit(_handle);
        _handle = as_behavior().null_handle();
    }

    template <typename TBehavior>
    void unique<TBehavior>::reset(const handle_type& handle) noexcept
    {
        as_behavior().deinit(_handle);
        _handle = handle;
    }

    template <typename TBehavior>
    void unique<TBehavior>::swap(unique& rhs) noexcept
    {
        using std::swap;
        swap(_handle, rhs._handle);
    }

    template <typename TBehavior>
    auto unique<TBehavior>::get() const noexcept
    {
        return _handle;
    }

    template <typename TBehavior>
    unique<TBehavior>::operator bool() const noexcept
    {
        return _handle != as_behavior().null_handle();
    }

    template <typename TBehavior>
    bool operator==(
        const unique<TBehavior>& lhs, const unique<TBehavior>& rhs) noexcept
    {
        return lhs._handle == rhs._handle;
    }

    template <typename TBehavior>
    bool operator!=(
        const unique<TBehavior>& lhs, const unique<TBehavior>& rhs) noexcept
    {
        return !(lhs == rhs);
    }

    template <typename TBehavior>
    void swap(unique<TBehavior>& lhs, unique<TBehavior>& rhs) noexcept
    {
        lhs.swap(rhs);
    }
}

#define FWD(...) ::std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)

template<template<typename> class TResource, typename TBehavior, typename... Ts>
auto make_resource(Ts&&... xs)
    noexcept(noexcept(true))
{
    return TResource<TBehavior>{TBehavior{}.init(FWD(xs)...)};
}

template<typename TBehavior, typename... Ts>
auto make_unique_resource(Ts&&... xs)
    noexcept(noexcept(true))
{
    return make_resource<resource::unique, TBehavior>(FWD(xs)...);
}

struct vbo_raii_access
{
    vbo_b::vbo_handle _handle;

    auto handle() const noexcept { return _handle; }
};

template<typename TResource>
struct vbo_resource_access
{
    TResource _resource;

    auto handle() const noexcept { return _resource.get(); }
};

template<typename TAccess>
struct vbo_interface 
{
    TAccess _access;

    void my_interface_method_0()
    {
        // something(_access.handle());
    }
};

int main()
{
    auto test0 = make_unique_resource<behavior::vbo_b>(1);



    return 0;
}

// TODO: 
// * init() should probably not be part of behaviors