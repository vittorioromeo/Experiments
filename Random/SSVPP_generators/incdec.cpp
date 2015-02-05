#include <SSVUtils/Core/Core.hpp>

using namespace std;
using namespace ssvu;

constexpr SizeT rcount{256};
std::ostringstream output;

int main()
{
	// decrement
	output << "#define SSVPP_IMPL_DECREMENT_0 0\n";
	output << "#define SSVPP_IMPL_DECREMENT_1 0\n";
	output << "#define SSVPP_IMPL_DECREMENT_2 1\n";
	for(auto i(3u); i < rcount; ++i) output << "#define SSVPP_IMPL_DECREMENT_" << i << " " << i - 1 << "\n";
	output << "\n";

	// increment
	for(auto i(0u); i < rcount - 1; ++i) output << "#define SSVPP_IMPL_INCREMENT_" << i << " " << i + 1 << "\n";
	output << "#define SSVPP_IMPL_INCREMENT_" << rcount - 1 << " " << rcount << "\n";
	output << "#define SSVPP_IMPL_INCREMENT_" << rcount << " " << rcount  << "\n";
	output << "\n";

	// bool
	output << "#define SSVPP_IMPL_BOOL_0 0\n";
	for(auto i(1u); i < rcount; ++i) output << "#define SSVPP_IMPL_BOOL_" << i << " 1\n";
	output << "\n";

	std::cout << output.str() << std::endl;

	return 0;
}