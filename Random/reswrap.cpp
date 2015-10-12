#include <iostream>
#include <memory>

namespace legacy
{
    using file_id = int;
    using vao_id = int;

    file_id create_file() { return 10; }
    void delete_file(file_id x) { (void)x; }

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
    namespace impl
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
    }

    struct file : public impl::interface_base<handle::file>
    {
        void write() { std::cout << "wrote to file\n"; }
        void read() { std::cout << "read from file\n"; }
    };

    struct vao : public impl::interface_base<handle::vao>
    {
        void bind() { std::cout << "vao bound\n"; }
        void unbind() { std::cout << "vao unbound\n"; }
    };

    template <typename T>
    struct heap_pointer : public impl::interface_base<handle::heap_pointer<T>>
    {
        auto operator-> () { return this->handle(); }
        auto operator-> () const { return this->handle(); }

        auto& operator*() { return *this->handle(); }
        const auto& operator*() const { return *this->handle(); }
    };
}

namespace behavior
{
    struct file
    {
        auto init() { return handle::file{legacy::create_file()}; }
        void deinit(handle::file f)
        {
            // if(f.id != 0)
            legacy::delete_file(f);
        }
    };

    struct vao
    {
        auto init()
        {
            handle::vao result;
            legacy::create_vao(&result);
            return result;
        }

        void deinit(handle::file f)
        {
            // if(f.id != 0)
            legacy::delete_vao(f);
        }
    };

    template <typename T>
    struct heap_pointer
    {
        auto init() { return handle::heap_pointer<T>{new T}; }

        void deinit(handle::heap_pointer<T> f)
        {
            // if(f != nullptr)
            delete f;
        }
    };
}

namespace bind
{
    namespace impl
    {
        template <typename THandle, typename TInterface, typename TBehavior,
            bool TPropagatePtrOperators = false>
        struct resource_bind
        {
            static constexpr bool propagate_ptr_operators{
                TPropagatePtrOperators};

            using handle_type = THandle;
            using interface_type = TInterface;
            using behavior_type = TBehavior;
        };
    }

    using file =
        impl::resource_bind<handle::file, interface::file, behavior::file>;

    template <typename T>
    using heap_pointer = impl::resource_bind<handle::heap_pointer<T>,
        interface::heap_pointer<T>, behavior::heap_pointer<T>, true>;
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
                return *resource.interface;
            }
        };

        template <>
        struct ptr_operator_propagator<false>
        {
            template <typename T>
            auto& operator()(T&& resource)
            {
                return resource.interface;
            }
        };

        template <typename TBind>
        class resource_base
        {
            template <bool>
            friend struct ptr_operator_propagator;

        protected:
            using bind_type = TBind;

            using handle_type = typename bind_type::handle_type;
            using interface_type = typename bind_type::interface_type;
            using behavior_type = typename bind_type::behavior_type;

            using propagator =
                ptr_operator_propagator<bind_type::propagate_ptr_operators>;

        private:
            // handle_type handle;
            interface_type interface;
            behavior_type behavior;

        protected:
            void init() { interface.set_handle(behavior.init()); }
            void deinit() {}

        public:
            decltype(auto) operator-> () { return &propagator{}(*this); }
            decltype(auto) operator-> () const { return &propagator{}(*this); }

            decltype(auto) operator*() { return propagator{}(*this); }
            decltype(auto) operator*() const { return propagator{}(*this); }
        };
    }

    template <typename TBind>
    struct unique : public impl::resource_base<TBind>
    {
        unique() { this->init(); }
        ~unique() { this->deinit(); }
    };

    template <typename TBind>
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
    };
}

struct test_handle
{
};

struct test : public interface::impl::interface_base<test_handle>
{
    int v;
};


struct test_behavior
{
    auto init() { return test_handle{}; }
    void uninit() {}
};

template <typename T>
using my_unique_ptr = resource::unique<bind::heap_pointer<T>>;

using test_bind = bind::impl::resource_bind<test_handle, test, test_behavior>;

using my_unique_test = resource::unique<test_bind>;

int main()
{

    // Traditional RAII.
    {
        test t{0};
        std::cout << t.v << "\n";
    }

    // "Unique resource" RAII.
    {
        // TODO
        my_unique_test t;
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
    }

    // "Unique resource" (with pointer propagation) heap-allocation.
    {
        // TODO
        my_unique_ptr<test> t; //{new test};
        t->v = 4;
        std::cout << t->v << "\n";
        std::cout << (*t).v << "\n";
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
