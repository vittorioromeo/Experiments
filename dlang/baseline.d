import core.stdc.stdlib;
import std.functional;
import std.conv;

struct MallocFreePolicy
{
    pragma(inline, true) static auto alloc(T, Args...)(auto ref Args args) @nogc
    {
        auto ptr = cast(T*) malloc(T.sizeof);
        emplace(ptr, forward!args);
        return ptr;
    }

    pragma(inline, true) static void dealloc(T)(T* ptr) @nogc
    {
        destroy(*ptr);
        free(ptr);
    }
}

struct Foo
{
    int _data;
}

void main()
{
    auto ptr = MallocFreePolicy.alloc!(Foo)(100);
    if (ptr != null)
    {
        MallocFreePolicy.dealloc(ptr);
        ptr = null;
    }
}
