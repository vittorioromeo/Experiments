#include <iostream>
#include <memory>

#define FWD(...) ::std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)
#define TYPE_W(...) typename decltype(__VA_ARGS__)::type

template <typename T>
struct type_w
{
    using type = T;
};


namespace legacy
{
    using file_id = int;
    using vao_id = int;

    file_id null_file() { return 0; }
    file_id create_file() { return 10; }
    void delete_file(file_id x) { (void)x; }

    vao_id null_vao() { return 0; }
    void create_vao(vao_id* x) { *x = 10; }
    void delete_vao(vao_id x) { (void)x; }
}

namespace handle
{
    using file = legacy::file_id;
    using vao = legacy::vao_id;

    template <typename T>
    using heap_pointer = T*;
}

namespace interface
{
    /*namespace impl
    {
        template <typename THandle>
        struct interface_base
        {
        public:
            using handle_type = THandle;

        private:
            handle_type _handle;

        public:
            void set_handle(handle_type handle) { _handle = handle; }

            auto handle() { return _handle; }
            auto handle() const { return _handle; }
        };
    }*/

    struct file
    {
        handle::file _handle{legacy::null_file()};

        file() = default;
        file(handle::file handle) : _handle{handle} {}

        void write() { std::cout << "wrote to file\n"; }
        void read() { std::cout << "read from file\n"; }
    };

    struct vao
    {
        handle::vao _handle{legacy::null_vao()};

        vao() = default;
        vao(handle::vao handle) : _handle{handle} {}

        void bind() { std::cout << "vao bound\n"; }
        void unbind() { std::cout << "vao unbound\n"; }
    };

    template <typename T>
    struct heap_pointer
    {
        handle::heap_pointer<T> _handle{nullptr};

        heap_pointer() = default;
        heap_pointer(handle::heap_pointer<T> handle) : _handle{handle} {}

        auto operator-> () { return _handle; }
        auto operator-> () const { return _handle; }

        auto& operator*() { return *_handle; }
        const auto& operator*() const { return *_handle; }
    };
}

namespace behavior
{
    namespace impl
    {
        template <typename THandle, typename TInterface,
            bool TPropagatePtrOperators>
        struct behavior_data
        {
            using handle_type = THandle;
            using interface_type = TInterface;

            type_w<handle_type> _tw_handle;
            type_w<interface_type> _tw_interface;

            static constexpr bool _propagate_ptr_operators{
                TPropagatePtrOperators};
        };
    }

    struct file : impl::behavior_data<handle::file, interface::file, false>
    {
        auto null_handle() { return handle::file{0}; }

        auto init() { return handle::file{legacy::create_file()}; }

        template <typename T>
        void deinit(T& f)
        {
            // if(f.id != 0)
            legacy::delete_file(f._handle);
        }

        template <typename TI>
        void release(TI& f)
        {
            f._handle = null_handle();
        }
    };

    struct vao
    {
        static constexpr bool propagate_ptr_operators{false};
        using handle_type = handle::vao;
        using interface_type = interface::vao;

        auto null_handle() { return handle::vao{0}; }

        auto init()
        {
            handle::vao result;
            legacy::create_vao(&result);
            return result;
        }

        template <typename T>
        void deinit(T& f)
        {
            // if(f.id != 0)
            legacy::delete_vao(f._handle);
        }

        template <typename TI>
        void release(TI& f)
        {
            f._handle = null_handle();
        }
    };

    template <typename T>
    struct heap_pointer : behavior::impl::behavior_data<handle::heap_pointer<T>,
                              interface::heap_pointer<T>, true>
    {

        auto null_handle() { return nullptr; }


        // template<typename...Ts>
        // auto init(Ts&&... xs) { return handle::heap_pointer<T>{new
        // T(FWD(xs)...)}; }

        auto init(T* p) { return handle::heap_pointer<T>{p}; }

        template <typename TI>
        void deinit(TI& f)
        {
            // if(f._handle != nullptr)
            delete f._handle;
        }

        template <typename TI>
        void release(TI& f)
        {
            f._handle = null_handle();
        }
    };
}

namespace resource
{
    namespace impl
    {
        template <bool>
        struct ptr_operator_propagator;

        template <>
        struct ptr_operator_propagator<true>
        {
            template <typename T>
            auto& operator()(T&& resource)
            {
                return *resource._interface;
            }
        };

        template <>
        struct ptr_operator_propagator<false>
        {
            template <typename T>
            auto& operator()(T&& resource)
            {
                return resource._interface;
            }
        };

        template <typename TBehavior>
        class resource_base
        {
            template <bool>
            friend struct ptr_operator_propagator;

        protected:
            using behavior_type = TBehavior;
            using handle_type = typename behavior_type::handle_type;
            using interface_type = typename behavior_type::interface_type;

            using propagator = ptr_operator_propagator<
                behavior_type::_propagate_ptr_operators>;

        protected:
            // handle_type _handle;
            behavior_type _behavior;
            interface_type _interface;

        protected:
            // void init() { _behavior = interface_type(behavior.init()); }
            void deinit() { _behavior.deinit(_interface); }

        public:
            resource_base() : _interface(_behavior.null_handle()) {}

            template <typename... Ts>
            resource_base(Ts&&... xs)
                : _interface(_behavior.init(FWD(xs)...))
            {
            }

            resource_base(resource_base&& x)
                : _behavior(std::move(x._behavior)),
                  _interface(std::move(x._interface))
            {
            }

            resource_base& operator=(resource_base&& x)
            {
                _behavior = std::move(x._behavior);
                _interface = std::move(x._interface);
                return *this;
            }

            decltype(auto) operator-> () { return &propagator{}(*this); }
            decltype(auto) operator-> () const { return &propagator{}(*this); }

            decltype(auto) operator*() { return propagator{}(*this); }
            decltype(auto) operator*() const { return propagator{}(*this); }
        };
    }

    template <typename TBehavior>
    struct unique : public impl::resource_base<TBehavior>
    {
    private:
        using base_type = impl::resource_base<TBehavior>;
        using interface_type = typename base_type::interface_type;

    public:
        unique() = default;

        template <typename... Ts>
        unique(Ts&&... xs)
            : base_type(FWD(xs)...)
        {
        }

        ~unique() { reset(); }

        unique(const unique&) = delete;
        unique& operator=(const unique&) = delete;

        unique(unique&& s) noexcept : base_type{static_cast<base_type&&>(s)}
        {
            // reset();
            s.release();
        }

        unique& operator=(unique&& s) noexcept
        {
            reset();
            base_type::operator=(static_cast<base_type&&>(s));

            // this->interface = std::move(s.interface);
            s.release();
            return *this;
        }


        void reset()
        {
            this->deinit();
            release();
        }

        void reset(interface_type&& i)
        {
            reset();
            this->_interface = std::move(i);
        }

        void release() { this->_behavior.release(this->_interface); }
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

struct test_handle
{
};

struct test
{
    test() = default;
    test(int x) : v{x} {}
    test(test_handle) {}

    ~test() { std::cout << "test dtor\n\n"; }

    int v;
};


struct test_behavior : behavior::impl::behavior_data<test_handle, test, false>
{
    auto null_handle() { return test_handle{}; }

    auto init() { return test_handle{}; }

    template <typename T>
    void deinit(T&)
    {
    }

    template <typename T>
    void release(T&)
    {
    }
};

template <typename T>
using my_unique_ptr = resource::unique<behavior::heap_pointer<T>>;

using my_unique_test = resource::unique<test_behavior>;

int main()
{

    // Traditional RAII.
    {
        test t{0};
        std::cout << t.v << "\n";
        std::cout << t.v << "\n";
    }

    // "Unique resource" RAII.
    {
        // TODO
        my_unique_test t;
        t->v = 1;
        std::cout << (*t).v << "\n";
        std::cout << t->v << "\n";
    }

    // Traditional heap-allocation.
    {
        test* t{new test{2}};
        std::cout << t->v << "\n";
        std::cout << (*t).v << "\n";
        delete t;
    }

    // `std::unique_ptr` heap-allocation.
    {
        std::unique_ptr<test> t{new test{3}};
        std::cout << t->v << "\n";
        std::cout << (*t).v << "\n";

        std::unique_ptr<test> t2;
        t2 = std::move(t);
        std::cout << t2->v << "\n";
        std::cout << (*t2).v << "\n";
    }

    // "Unique resource" (with pointer propagation) heap-allocation.
    {
        // TODO
        my_unique_ptr<test> t{new test{4}};
        std::cout << t->v << "\n";
        std::cout << (*t).v << "\n";

        my_unique_ptr<test> t2;
        t2 = std::move(t);
        std::cout << t2->v << "\n";
        std::cout << (*t2).v << "\n";
    }


    // resource::unique<bind::file> u_file;
    // resource::shared<bind::file> s_file;
    // resource::atomic_shared<bind::file> as_file;

    // auto x = make_unique_resource(file{...});
    // auto x = make_unique_resource<bind::file>(...);

    // auto x = make_resource(unique, file{...});
    // auto x = make_resource<bind::file>(unique, ...);

    // auto x = make_resource<unique, bind::file>(...);
    // auto x = make_resource<unique>(file{...});

    return 0;
}

template <typename... Ts>
void glGenVertexArrays(Ts...)
{
}

template <typename... Ts>
void glDeleteVertexArrays(Ts...)
{
}

/*
using vao_handle = int;
constexpr vao_handle null_vao_handle{0};

struct my_vao_interface
{
    vao_handle _vao{null_vao_handle};

    my_vao_interface(vao_handle vao) : _vao{vao} {}

    void bind() { }
    void unbind() { }
};

struct my_vao_behavior
{
    vao_handle init()
    {
        vao_handle vh;
        glGenVertexArrays(1, &vh);
        return vh;
    }

    void deinit(my_vao_interface& i)
    {
        glDeleteVertexArrays(1, &i._vao);
    }

    void release(my_vao_interface& i)
    {
        i._vao = null_vao_handle;
    }
};
*/


using vao_handle = int;
constexpr vao_handle null_vao_handle{0};

template <typename... Ts>
vao_handle getGlGenVertexArrays(Ts...)
{
    return {};
}

struct my_vao_interface
{
    vao_handle _vao{null_vao_handle};

    my_vao_interface(vao_handle vao) : _vao{vao} {}

    void bind() {}
    void unbind() {}
};

template <typename THandle, THandle TNullHandle, typename TFInit,
    typename TFDeinit, typename TFGetter>
struct generic_value_behavior
{
    template <typename... Ts>
    auto init(Ts&&... xs)
    {
        return TFInit{}(FWD(xs)...);
    }

    template <typename TInterface>
    void deinit(TInterface& i)
    {
        TFDeinit{}(TFGetter{}(i));
    }

    template <typename TInterface>
    void release(TInterface& i)
    {
        TFGetter{}(i) = TNullHandle;
    }
};

template <typename THandle, THandle TNullHandle, typename TFInit,
    typename TFDeinit, typename TFGetter>
constexpr auto make_generic_value_behavior(TFInit&&, TFDeinit&&, TFGetter&&)
{
    return generic_value_behavior<THandle, TNullHandle, TFInit, TFDeinit,
        TFGetter>{};
}

/*
struct my_vao_behavior
{
    auto init()
    {
        return getGlGenVertexArrays(1);
    }

    void deinit(my_vao_interface& i)
    {
        glDeleteVertexArrays(1, &i._vao);
    }

    void release(my_vao_interface& i)
    {
        i._vao = null_vao_handle;
    }
};
*/

static auto vao_behavior_variable =                           //.
    make_generic_value_behavior<vao_handle, null_vao_handle>( // .
        [](auto&&... xs)
        {
            return getGlGenVertexArrays(FWD(xs)...);
        },
        [](auto& h)
        {
            glDeleteVertexArrays(1, &h);
        },
        [](auto& i) -> auto&
        {
            return i._vao;
        });

using my_vao_behavior = decltype(vao_behavior_variable);

void desired_main() {}

// vlog: cppcon trip report (talks, experience, photos)
//       upcoming dive into c++ (twitter for code review)
//       undertale

// dicp: use case example (ptr*, file*, vao_id (same semantics, different
// syntax))
//       manually-made wrapper
//       unique_ptr
//       ...