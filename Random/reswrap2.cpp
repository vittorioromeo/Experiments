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

namespace behavior
{
    namespace impl
    {
        template <typename THandle, typename TWrapper,
            bool TPropagatePtrOperators>
        struct behavior_data
        {
            using handle_type = THandle;
            using wrapper_type = TWrapper;

            type_w<handle_type> _tw_handle;
            type_w<wrapper_type> _tw_wrapper;

            static constexpr bool _propagate_ptr_operators{
                TPropagatePtrOperators};
        };

        template <typename THandle, bool TPropagatePtrOperators>
        struct behavior_base
        {
            using handle_type = THandle;
            static constexpr bool _propagate_ptr_operators{
                TPropagatePtrOperators};
        };
    }

    using impl::behavior_base;

    struct vao : behavior_base<handle::vao, false>
    {
        auto null() noexcept { return 0; }

        template <typename... Ts>
        auto init(Ts&&... xs) noexcept
        {
            handle::vao result;
            legacy::create_vao(&result, FWD(xs)...);
            return result;
        }

        void deinit(handle::vao& x) noexcept { legacy::delete_vao(x); }
    };

    struct file : behavior_base<handle::file, false>
    {
        auto null() noexcept { return 0; }

        template <typename... Ts>
        auto init(Ts&&... xs) noexcept
        {
            return handle::file{legacy::create_file(FWD(xs)...)};
        }

        void deinit(handle::file& x) noexcept { legacy::delete_file(x); }
    };

    template <typename T>
    struct heap_pointer : behavior_base<handle::heap_pointer<T>, true>
    {
        auto null() noexcept { return handle::heap_pointer<T>{nullptr}; }

        template <typename... Ts>
        auto init(Ts&&... xs) noexcept
        {
            return new T(FWD(xs)...);
        }

        void deinit(handle::heap_pointer<T>& x) noexcept { delete x; }
    };
}

namespace wrapper
{
    /*
    namespace impl
    {
        template <typename THandle>
        struct wrapper_base
        {
        private:
            THandle _handle;

        public:
            wrapper_base(const THandle& handle) noexcept : _handle(handle) {}

            auto& handle() noexcept { return _handle; }
            const auto& handle() const noexcept { return _handle; }
        };
    }
    */

    namespace impl
    {
        template <typename TBehavior>
        struct wrapper_base
        {
        public:
            using behavior_type = TBehavior;
            using handle_type = typename behavior_type::handle_type;

        private:
            handle_type _handle;

        public:
            wrapper_base() noexcept : _handle(behavior_type{}.null()) {}
            wrapper_base(const handle_type& handle) noexcept : _handle(handle) {}

            auto& handle() noexcept { return _handle; }
            const auto& handle() const noexcept { return _handle; }
        };
    }

    using impl::wrapper_base;

    struct file : wrapper_base<behavior::file>
    {
        void write() { std::cout << "wrote to file\n"; }
        void read() { std::cout << "read from file\n"; }
    };

    struct vao : wrapper_base<behavior::vao>
    {
        void bind() { std::cout << "vao bound\n"; }
        void unbind() { std::cout << "vao unbound\n"; }
    };

    template <typename T>
    struct heap_pointer : wrapper_base<behavior::heap_pointer<T>>
    {
        using base_type = wrapper_base<behavior::heap_pointer<T>>;
        using base_type::base_type;

        auto operator-> () { return this->handle(); }
        auto operator-> () const { return this->handle(); }

        auto& operator*() { return *this->handle(); }
        const auto& operator*() const { return *this->handle(); }
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
                return *resource._wrapper;
            }
        };

        template <>
        struct ptr_operator_propagator<false>
        {
            template <typename T>
            auto& operator()(T&& resource)
            {
                return resource._wrapper;
            }
        };

        template <typename TWrapper>
        class resource_base
        {
            template <bool>
            friend struct ptr_operator_propagator;

        protected:
            using wrapper_type = TWrapper;
            using behavior_type = typename wrapper_type::behavior_type;
            using handle_type = typename wrapper_type::handle_type;

            using propagator = ptr_operator_propagator<
                behavior_type::_propagate_ptr_operators>;

        protected:
            // handle_type _handle;
            behavior_type _behavior;
            wrapper_type _wrapper;

        protected:
            // void init() { _behavior = wrapper_type(behavior.init()); }
            void deinit() { _behavior.deinit(_wrapper.handle()); }

        public:
            resource_base() : _wrapper(_behavior.null()) {}

            template <typename... Ts>
            resource_base(Ts&&... xs)
                : _wrapper(_behavior.init(FWD(xs)...))
            {
            }

            resource_base(resource_base&& x)
                : _behavior(std::move(x._behavior)),
                  _wrapper(std::move(x._wrapper))
            {
            }

            resource_base& operator=(resource_base&& x)
            {
                _behavior = std::move(x._behavior);
                _wrapper = std::move(x._wrapper);
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
        using wrapper_type = typename base_type::wrapper_type;

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

        void reset(wrapper_type&& i)
        {
            reset();
            this->_wrapper = std::move(i);
        }

        void release() 
        { 
        //    this->_behavior.release(this->_wrapper); 
        }
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

struct test_behavior : behavior::behavior_base<test_handle, false>
{
    auto null() { return test_handle{}; }

    auto init() { return test_handle{}; }

    template <typename T>
    void deinit(T&)
    {
    }
};

struct test : wrapper::wrapper_base<test_behavior>
{
    using base_type = wrapper::wrapper_base<test_behavior>;
    using base_type::base_type;

    ~test() { std::cout << "test dtor\n\n"; }

    int v;
};



template <typename T>
using my_unique_ptr = resource::unique<wrapper::heap_pointer<T>>;

using my_unique_test = resource::unique<test>;

int main()
{

    // Traditional RAII.
    {
        test t{};
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
        test* t{new test{}};
        std::cout << t->v << "\n";
        std::cout << (*t).v << "\n";
        delete t;
    }

    // `std::unique_ptr` heap-allocation.
    {
        std::unique_ptr<test> t{new test{}};
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
        my_unique_ptr<test> t{new test{}};
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