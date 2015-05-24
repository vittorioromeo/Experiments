#include <cassert> 
#include <utility> 

constexpr auto f()
{
	assert(1 < 2);	
	return 1;
}

template<int> struct S { };

using X = S<f()>;

int main()
{
	return 0;
}