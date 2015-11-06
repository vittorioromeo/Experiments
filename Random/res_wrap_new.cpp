#include <iostream>
#include <memory>
#include <vrm/core.hpp>

// The system is composed by the following elements
//
// * Legacy functions/types
// * Handle types
//     * Must be trivially copyable/movable
// * Behavior tyeps
// * Resource types
// * Interface types
// * Behavior+interface facade types

// Simulate legacy functions
namespace legacy
{
    using GLsizei = std::size_t;
    using GLuint = int;
    using file_id = int;

    // OpenGL VBO
    void glGenBuffers(GLsizei n, GLuint* buffers)
    {
        VRM_CORE_ASSERT(n == 1);
        *buffers = 1;

        std::cout << "VBO generated\n";
    }

    void glDeleteBuffers(GLsizei n, const GLuint* buffers)
    {
        VRM_CORE_ASSERT(n == 1);
        // VRM_CORE_ASSERT(*buffers == 1);

        if(*buffers == 1)
        {
            std::cout << "VBO deleted\n";
        }
        else
        {
            std::cout << "Null VBO silently deleted\n";
        }
    }

    // OpenGL VAO
    void glGenVertexArrays(GLsizei n, GLuint* arrays)
    {
        VRM_CORE_ASSERT(n == 1);
        *arrays = 1;

        std::cout << "VAO generated\n";
    }

    void glDeleteVertexArrays(GLsizei n, const GLuint* arrays)
    {
        VRM_CORE_ASSERT(n == 1);
        // VRM_CORE_ASSERT(*arrays == 1);

        if(*arrays == 1)
        {
            std::cout << "VAO deleted\n";
        }
        else
        {
            std::cout << "Null VAO silently deleted\n";
        }
    }

    // Free-store pointers
    /*template <typename T, typename... Ts>
    auto free_store_new(Ts&&... xs)
    {
        auto result(new T(FWD(xs)...));
        std::cout << "free_store_new\n";
        return result;
    }*/
    template <typename T>
    auto free_store_new(T* ptr)
    {
        //        assert(ptr != nullptr);
        return ptr;
    }

    template <typename T>
    void free_store_delete(T* ptr)
    {
        if(ptr != nullptr)
        {
            std::cout << "free_store_delete\n";
        }
        else
        {
            std::cout << "nullptr deleted\n";
        }

        delete ptr;
    }

    // SFML-like SoundBuffer
    struct SoundBuffer
    {
        SoundBuffer() { std::cout << "SoundBuffer created\n"; }
        ~SoundBuffer() { std::cout << "SoundBuffer destroyed\n"; }

        SoundBuffer(const SoundBuffer&) = default;
        SoundBuffer(SoundBuffer&&) = default;

        SoundBuffer& operator=(const SoundBuffer&) = default;
        SoundBuffer& operator=(SoundBuffer&&) = default;
    };

    auto make_sound_buffer() { return SoundBuffer{}; }

    // ID-based handle
    auto make_file()
    {
        std::cout << "File created\n";
        return file_id{1};
    }

    void destroy_file(file_id) { std::cout << "File destroyed\n"; }
}

// Barebone handle types
namespace handle
{
    using gl_vbo = legacy::GLuint;
    using gl_vao = legacy::GLuint;

    template <typename T>
    using free_store_ptr = T*;

    using sound_buffer = legacy::SoundBuffer;
    using file = legacy::file_id;
}

namespace behavior
{
    struct vbo_b
    {
        using handle_type = handle::gl_vbo;

        static auto null_handle() { return handle_type{0}; }

        auto init()
        {
            handle_type h;
            legacy::glGenBuffers(1, &h);
            return h;
        }

        void deinit(handle_type& h)
        {
            // The user is free to check for double deletion in here.

            legacy::glDeleteBuffers(1, &h);
        }
    };

    struct vao_b
    {
        using handle_type = handle::gl_vao;

        static auto null_handle() { return handle_type{0}; }

        auto init()
        {
            handle_type h;
            legacy::glGenVertexArrays(1, &h);
            return h;
        }

        void deinit(handle_type& h) { legacy::glDeleteVertexArrays(1, &h); }
    };

    template <typename T>
    struct free_store_b
    {
        using handle_type = handle::free_store_ptr<T>;

        static auto null_handle() { return handle_type{nullptr}; }

        template <typename... Ts>
        auto init(Ts&&... xs)
        {
            return legacy::free_store_new<T>(FWD(xs)...);
        }

        void deinit(handle_type& h) { legacy::free_store_delete<T>(h); }
    };

    struct sound_buffer_b
    {
        bool initialized{false};

        using handle_type = handle::sound_buffer;

        static auto null_handle() { return handle_type{}; }

        auto init()
        {
            initialized = true;
            return legacy::make_sound_buffer();
        }
        void deinit(handle_type&)
        {
            if(initialized)
            {
                std::cout << "deleted initialized sb\n";
            }
            else
            {
                std::cout << "deleted null sb\n";
            }
        }
    };

    struct file_b
    {
        using handle_type = handle::file;

        static auto null_handle() { return handle_type{0}; }
        auto init() { return legacy::make_file(); }
        void deinit(handle_type& h) { legacy::destroy_file(h); }
    };
}

namespace resource
{
    namespace impl
    {
        template <typename TBehavior>
        using inner_handle_type = typename TBehavior::handle_type;

        template <typename TBehavior>
        class resource_base : TBehavior
        {
        protected:
            using behavior_type = TBehavior;
            using handle_type = typename behavior_type::handle_type;

        protected:
            // behavior_type _behavior;
            handle_type _handle;

        public:
            template <typename... Ts>
            void init(Ts&&... xs)
            {
                _handle = static_cast<behavior_type*>(this)->init(FWD(xs)...);
            }

            void deinit()
            {
                static_cast<behavior_type*>(this)->deinit(_handle);
            }

        public:
            resource_base() : _handle(behavior_type::null_handle())
            {

                std::cout << "resbase null ctor\n";
            }

            template <typename... Ts>
            resource_base(Ts&&... xs)
                : _handle(static_cast<behavior_type*>(this)->init(FWD(xs)...))
            {
                std::cout << "resbase init ctor\n";
            }

            resource_base(resource_base&& x)
                //: _behavior(std::move(x._behavior)),
                : _handle(std::move(x._handle))
            {
                std::cout << "resbase move ctor\n";
            }

            resource_base& operator=(resource_base&& x)
            {
                //_behavior = std::move(x._behavior);
                _handle = std::move(x._handle);
                return *this;
            }

            auto& handle() noexcept { return _handle; }
            const auto& handle() const noexcept { return _handle; }

            bool null() const noexcept { return _handle == this->null(); }
        };
    }

    template <typename TBehavior>
    struct unique : public impl::resource_base<TBehavior>
    {
    private:
        using base_type = impl::resource_base<TBehavior>;
        using behavior_type = typename base_type::behavior_type;
        using handle_type = typename base_type::handle_type;

    public:
        unique() = default;

        template <typename... Ts>
        unique(Ts&&... xs)
            : base_type(FWD(xs)...)
        {
            std::cout << "unique variadic ctor\n";
        }

        ~unique() { reset(); }

        unique(const unique&) = delete;
        unique& operator=(const unique&) = delete;

        unique(unique&& s) noexcept : base_type{static_cast<base_type&&>(s)}
        {
            // reset();
            std::cout << "unique move ctor\n";

            s.release();
        }

        unique& operator=(unique&& s) noexcept
        {
            std::cout << "unique move assign\n";

            reset();
            base_type::operator=(static_cast<base_type&&>(s));

            // this->interface = std::move(s.interface);
            s.release();
            return *this;
        }

        void reset(const handle_type& handle = behavior_type::null_handle())
        {
            this->deinit();
            this->_handle = handle;
        }


        auto release()
        {
            auto temp_handle(this->_handle);
            this->_handle = behavior_type::null_handle();
            return temp_handle;
        }
    };

    template <typename TBehavior>
    struct value
    {
    private:
        using behavior_type = TBehavior;
        using handle_type = typename behavior_type::handle_type;

        behavior_type _behavior;
        handle_type _handle;

    public:
        template <typename... Ts>
        value(Ts&&... xs)
            : _handle(_behavior.init(FWD(xs)...))
        {
        }

        ~value() { _behavior.deinit(_handle); }

        value(value&) {}
        value(const value&) {}
        value(value&&) {}
    };

    /*template <typename TBind>
    struct shared : public impl::resource_base<TBind>
    {
        shared() {}
        ~shared()
        {
            // if(counter-- == 0)
            // {
            //     this->deinit();
            // }
        }
    };

    template <typename TBind>
    struct atomic_shared
    {
        // ...
    };*/
}

namespace wrapper
{
    template <typename TResource>
    struct wrapper_base
    {
    protected:
        auto& handle() noexcept { return this->handle(); }
        const auto& handle() const noexcept { return this->handle(); }
    };

    template <typename TResource>
    struct vbo_w : public wrapper_base<TResource>
    {

        void do_something() { (void)this->handle(); }
    };

    template <typename TResource>
    struct vao_w : public wrapper_base<TResource>
    {

        void do_something() { (void)this->handle(); }
    };

    template <typename TResource>
    struct free_store_w : public wrapper_base<TResource>
    {
        auto& operator*() { return *this->handle(); }
        const auto& operator*() const { return *this->handle(); }

        auto operator-> () { return this->handle(); }
        auto operator-> () const { return this->handle(); }
    };

    template <typename TResource>
    struct sound_buffer_w : public wrapper_base<TResource>
    {

        void do_something() { (void)this->handle(); }
    };

    template <typename TResource>
    struct file_w : public wrapper_base<TResource>
    {


        void do_something() { (void)this->handle(); }
    };
}

namespace facade
{
    template <typename TBehavior, template <typename> class TWrapper>
    struct impl
    {
        using unique_res = resource::unique<TBehavior>;
        using unique_wrapper = TWrapper<unique_res>;

        // Constructor/destructor order (!)
        struct unique_result_type : unique_res, unique_wrapper
        {
            template <typename... Ts>
            unique_result_type(Ts&&... xs)
                : unique_res(FWD(xs)...)
            {
            }
        };

        using value_res = resource::value<TBehavior>;
        using value_wrapper = TWrapper<value_res>;

        struct value_result_type : value_res, value_wrapper
        {
            template <typename... Ts>
            value_result_type(Ts&&... xs)
                : value_res(FWD(xs)...)
            {
            }

            value_result_type(const value_result_type& rhs) {}

            value_result_type(value_result_type& rhs)
                : value_result_type(static_cast<const value_result_type&>(rhs))
            {
            }
        };
    };

    template <typename TBehavior, template <typename> class TWrapper>
    using bw = facade::impl<TBehavior, TWrapper>;

    template <typename TBehavior, template <typename> class TWrapper>
    using unique = typename bw<TBehavior, TWrapper>::unique_result_type;

    template <typename TBehavior, template <typename> class TWrapper>
    using value = typename bw<TBehavior, TWrapper>::value_result_type;
}

template <typename TFacade, typename... Ts>
auto makef_unique(Ts&&... xs)
{
    using result_type = typename TFacade::unique_result_type;
    return result_type(FWD(xs)...);
}

using unique_vbo = facade::unique<behavior::vbo_b, wrapper::vbo_w>;
using unique_vao = facade::unique<behavior::vao_b, wrapper::vao_w>;

template <typename T>
using unique_fs_ptr =
    facade::unique<behavior::free_store_b<T>, wrapper::free_store_w>;

using unique_sound_buffer =
    facade::unique<behavior::sound_buffer_b, wrapper::sound_buffer_w>;

using unique_file = facade::unique<behavior::file_b, wrapper::file_w>;

using value_sound_buffer =
    facade::value<behavior::sound_buffer_b, wrapper::sound_buffer_w>;




int main()
{
#define COUT_SIZE(...)                                                      \
    std::cout << "sizeof(" << #__VA_ARGS__ << ") = " << sizeof(__VA_ARGS__) \
              << "\n";

    COUT_SIZE(std::unique_ptr<int>);

    COUT_SIZE(behavior::free_store_b<int>);
    COUT_SIZE(behavior::free_store_b<int>::handle_type);
    COUT_SIZE(wrapper::free_store_w<int>);
    COUT_SIZE(resource::unique<behavior::free_store_b<int>>);
    COUT_SIZE(unique_fs_ptr<int>);

    struct ABC
    {
        ABC() { std::cout << "ABC()\n"; }
        ~ABC() { std::cout << "~ABC()\n"; }
    };

    using uptr_type = unique_fs_ptr<ABC>;

    {
        std::vector<uptr_type> v;
        v.emplace_back(new ABC);
        v.emplace_back(new ABC);
        v.emplace_back(new ABC);
    }

    return 0;
}

int main2()
{
    {
        // auto xd = unique_vbo{};
    }

    /*{
        auto xd = unique_vbo{};
        VRM_CORE_ASSERT(xd.null());

        xd.init();
        VRM_CORE_ASSERT(!xd.null());

        std::cout << "inited\n";
        auto xd2 = std::move(xd);

        VRM_CORE_ASSERT(xd.null());
        VRM_CORE_ASSERT(!xd2.null());
    }*/
    if(false)
    {
        unique_fs_ptr<legacy::SoundBuffer> x;
        // x.init();
    }

    if(true)
    {
        // auto x(makef_unique<facade::bw<behavior::sound_buffer_b,
        // wrapper::sound_buffer_w>>());
        unique_sound_buffer x;
        // x.init();
    }

    if(false)
    {
        std::unique_ptr<legacy::SoundBuffer> x;
        x.reset(new legacy::SoundBuffer);
    }

    if(true)
    {
        // resource::value<behavior::sound_buffer_b> sbv{};
    }

    if(false)
    {
        value_sound_buffer sbv{};
        // value_sound_buffer a2(sbv);
    }

    std::cout << "\n";

    if(true)
    {
        legacy::SoundBuffer sbv{};
        //&legacy::SoundBuffer a2(sbv);
    }
}
