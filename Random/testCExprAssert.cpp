#include <SSVUtils/Core/Core.hpp>

constexpr int test1(int k)
{
	static_assert(k < 100, "");

	return k;
}

int main()
{
	std::array<int, test1(50)> pppp;

	return 0;
}