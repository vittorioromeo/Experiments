#include <cstdlib>
#include <new>
#include <utility>

struct MallocFreePolicy
{
    template <typename T, typename... Args>
    static auto alloc(Args&&... args)
    {
        auto ptr = (T*)std::malloc(sizeof(T));
        new(ptr) T(std::forward<Args>(args)...);
        return ptr;
    }

    template <typename T>
    static void dealloc(T* ptr)
    {
        ptr->~T();
        std::free(ptr);
    }
};

template <typename T, typename AllocPolicy = MallocFreePolicy>
struct UniquePtr
{
    T* _ptr;

    template <typename U>
    UniquePtr(U* ptr) : _ptr{ptr}
    {
    }

    ~UniquePtr()
    {
        if(_ptr != nullptr)
        {
            AllocPolicy::dealloc(_ptr);
            _ptr = nullptr;
        }
    }
};

struct Foo
{
    int _data;

    Foo(int data) : _data{data}
    {
    }
};

int main()
{
    UniquePtr<Foo> i = MallocFreePolicy::alloc<Foo>(100);
}
