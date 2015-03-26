#include <vector>
#include <memory>
#include <iostream>
#include <SSVUtils/Core/Core.hpp>

struct XTBase
{
	inline virtual ~XTBase() { }
};
struct XTDer : public XTBase
{
	int n{1};
	inline int f(int mK) const { return mK + n; }
};

int main()
{	
	using namespace ssvu;

	std::vector<UPtr<XTBase>> v;
	v.emplace_back(mkUPtr<XTDer>());

	return 0;
}
