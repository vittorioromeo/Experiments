import automem;

class Foo
{
    this() @nogc
    {
    }

    ~this() @nogc
    {
    }
}

void main() @nogc
{
    auto x = Unique!Foo();
}
