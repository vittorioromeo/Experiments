#include <SSVUtils/Core/Core.hpp>

using namespace std;
using namespace ssvu;

constexpr SizeT tplCount{20};
std::ostringstream output;

void mkFill(SizeT i)
{
	output << "#define SSVPP_IMPL_TPL_FILL_" << i << "(m0)";
	
	if(i != 0) output << "	SSVPP_TPL_EXPLODE(m0),	";
	else output << "				";

	for(auto j(0u); j < tplCount - i - 1; ++j) output << "0, ";
	output << "0\n";
}

void mkFillReverse(SizeT i)
{
	output << "#define SSVPP_IMPL_TPL_FILL_REVERSE_" << i << "(m0)	";
	
	for(auto j(0u); j < tplCount - i - 1; ++j) output << "0, ";
	output << "0";
	
	if(i != 0) output << ", SSVPP_TPL_EXPLODE(m0)";
	
	output << "\n";
}


void mkTupleElem(SizeT i)
{
	output << "#define SSVPP_IMPL_TPL_ELEM_" << i << "(";
	
	for(auto j(0u); j < tplCount - 1; ++j) output << "m" << j << ", ";
	output << "m" << tplCount - 1 << ") m" << i << "\n";
}

void mkTupleElemReverse(SizeT i)
{
	output << "#define SSVPP_IMPL_TPL_ELEM_REVERSE_" << i << "(";
	
	for(auto j(0u); j < tplCount - 1; ++j) output << "m" << j << ", ";
	output << "m" << tplCount - 1 << ") m" << tplCount - i - 1 << "\n";
}

void mkCat(SizeT i)
{
	output << "#define SSVPP_IMPL_TPL_CAT_" << i << "(m0, m1, ...)	SSVPP_IMPL_TPL_CAT_" << i - 1 << "(SSVPP_TPL_MAKE(SSVPP_TPL_EXPLODE(m0), SSVPP_TPL_EXPLODE(m1)), __VA_ARGS__)\n";
}

void genTuple()
{
	for(auto i(0u); i < tplCount; ++i) mkFill(i);
	output << "#define SSVPP_IMPL_TPL_FILL_" << tplCount << "(m0)	SSVPP_TPL_EXPLODE(m0)\n";
	output << "\n";

	// tpl fill reverse
	for(auto i(0u); i < tplCount; ++i) mkFillReverse(i);
	output << "#define SSVPP_IMPL_TPL_FILL_REVERSE_" << tplCount << "(m0)	SSVPP_TPL_EXPLODE(m0)\n";
	output << "\n";

	// tpl elem
	for(auto i(0u); i < tplCount; ++i) mkTupleElem(i);	
	output << "\n";

	// tpl elem reverse
	for(auto i(0u); i < tplCount; ++i) mkTupleElemReverse(i);	
	output << "\n";

	// tpl cat
	output << "#define SSVPP_IMPL_TPL_CAT_0()			SSVPP_EMPTY()\n";
	output << "#define SSVPP_IMPL_TPL_CAT_1(m0)			m0\n";
	output << "#define SSVPP_IMPL_TPL_CAT_2(m0, m1)		SSVPP_TPL_MAKE(SSVPP_TPL_EXPLODE(m0), SSVPP_TPL_EXPLODE(m1))\n";
	for(auto i(3u); i < tplCount * 2 + 1; ++i) mkCat(i);	
	output << "\n";
}

int main()
{
	// tpl fill
	

	std::cout << output.str() << std::endl;

	return 0;
}