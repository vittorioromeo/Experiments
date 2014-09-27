#include <SSVUtils/Core/Core.hpp>

struct A 
{
	std::map<int, A> other;
	
	union Hold
	{
		char a[8];
		float f;
	} fh;
};

int main() 
{
	A k;
	return 0;
}