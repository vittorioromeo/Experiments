import core.stdc.stdlib;
import std.functional;
import std.conv;

struct MallocFreePolicy
{
    static auto alloc(T, Args...)(auto ref Args args)
    {
        auto ptr = cast(T*) malloc(T.sizeof);
        emplace(ptr, forward!args);
        return ptr;
    }

    static void dealloc(T)(T* ptr)
    {
        destroy(*ptr);
        free(ptr);
    }
}

struct UniquePtr(T, AllocPolicy = MallocFreePolicy)
{
    T* _ptr;

    this(U : T)(U* ptr)
    {
        _ptr = ptr;
    }

    this(this) @disable;

    ~this()
    {
        if (_ptr != null)
        {
            AllocPolicy.dealloc(_ptr);
            _ptr = null;
        }
    }
}

struct Foo
{
    int _data;
}

void main()
{
    UniquePtr!Foo i = MallocFreePolicy.alloc!(Foo)(100);
}
