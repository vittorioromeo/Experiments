#include <SSVUtils/Core/Core.hpp>
#include <SSVUtils/Benchmark/Benchmark.hpp>

constexpr int times{1000000};
constexpr int tests{3};

volatile int state{0};

struct SSmall
{
	char x[16];
	inline SSmall() noexcept { ++state; }
	inline ~SSmall() noexcept { --state; }
};

struct SBig
{
	char x[64];
	inline SBig() noexcept { ++state; }
	inline ~SBig() noexcept { --state; }
};

int main()
{
	SSVU_BENCHMARK_INIT_GROUP_SCOPE_EXIT("mkUPtr");
	SSVU_BENCHMARK_INIT_GROUP_SCOPE_EXIT("UPtr");
	SSVU_BENCHMARK_INIT_GROUP_SCOPE_EXIT("new/delete");
	

	for(auto t(0); t < tests; ++t)
	{
		{
			SSVU_BENCHMARK_RUN_GROUP_SCOPE_EXIT("mkUPtr");

			for(auto i(0); i < times; ++i)
				auto x = ssvu::mkUPtr<SSmall>();
		
			for(auto i(0); i < times; ++i)
				auto x = ssvu::mkUPtr<SBig>();
		}

		{
			SSVU_BENCHMARK_RUN_GROUP_SCOPE_EXIT("new/delete");

			for(auto i(0); i < times; ++i)
			{
				auto x = new SSmall{};
				delete x;
			}
		
			for(auto i(0); i < times; ++i)
			{
				auto x = new SBig{};
				delete x;
			}
		}		

		{
			SSVU_BENCHMARK_RUN_GROUP_SCOPE_EXIT("UPtr");

			for(auto i(0); i < times; ++i)
				auto x = ssvu::UPtr<SSmall>{new SSmall{}};
		
			for(auto i(0); i < times; ++i)
				auto x = ssvu::UPtr<SBig>{new SBig{}};
		}
	}

	return 0;
}
