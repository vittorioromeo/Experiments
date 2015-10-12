namespace legacy
{
    int create_file() { return 10; }
    void delete_file(int x) { (void)x; }
}

namespace handle
{
    struct file
    {
        int id;
    };

    template <typename T>
    struct heap_pointer
    {
        T* ptr;
    };
}

namespace interface
{
    struct file
    {
    private:
        handle::file handle;

    public:
        void write() {}
        void read() {}
    };

    template <typename T>
    struct heap_pointer
    {
    private:
        handle::heap_pointer<T> handle;
    };
}

namespace behavior
{
    struct file
    {
        auto init() { return handle::file{legacy::create_file()}; }
        void deinit(handle::file f) { /* if(f.id != 0) */ legacy::delete_file(f.id); }
    };

    template <typename T>
    struct heap_pointer
    {
        auto init() { return handle::heap_pointer<T>{new T}; }
        void deinit(handle::heap_pointer<T> f) { /* if(f != nullptr) */ delete f.ptr; }
    };
}

namespace bind
{
    namespace impl
    {
        template <typename THandle, typename TInterface, typename TBehavior>
        struct resource_bind
        {
            using handle_type = THandle;
            using interface_type = TInterface;
            using behavior_type = TBehavior;
        };
    }

    using file =
        impl::resource_bind<handle::file, interface::file, behavior::file>;

    template <typename T>
    using heap_pointer = impl::resource_bind<handle::heap_pointer<T>,
        interface::heap_pointer<T>, behavior::heap_pointer<T>>;
}

namespace resource
{
    namespace impl
    {
        template <typename TBind>
        class resource_base
        {
        protected:
            using bind_type = TBind;

            using handle_type = typename bind_type::handle_type;
            using interface_type = typename bind_type::interface_type;
            using behavior_type = typename bind_type::behavior_type;

        private:
            // handle_type handle;
            interface_type interface;
            behavior_type behavior;

        protected:
            void init() {}
            void deinit() {}

            auto operator-> () { return &interface; }
            auto operator-> () const { return &interface; }

            auto& operator*() { return interface; }
            const auto& operator*() const { return interface; }
        };
    }

    template <typename TBind>
    struct unique : public impl::resource_base<TBind>
    {
        unique() {}
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

int main()
{

    resource::unique<bind::file> u_file;
    resource::shared<bind::file> s_file;
    resource::atomic_shared<bind::file> as_file;

    // auto x = make_unique_resource(file{...});
    // auto x = make_unique_resource<bind::file>(...);

    // auto x = make_resource(unique, file{...});
    // auto x = make_resource<bind::file>(unique, ...);

    // auto x = make_resource<unique, bind::file>(...);
    // auto x = make_resource<unique>(file{...});

    return 0;
}