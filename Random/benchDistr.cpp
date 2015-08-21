#include <SSVUtils/Core/Core.hpp>
#include <SSVUtils/Benchmark/Benchmark.hpp>

using namespace ssvu;

constexpr int times{10000};
constexpr int times2{50000};

std::minstd_rand rng;
volatile int state{0};

void testNew(int min, int max)
{
	auto k = std::uniform_int_distribution<int>{min, max}(rng);
	state += k;
}

void testParams(int min, int max)
{
	auto k = std::uniform_int_distribution<int>{min, max}(rng);
	state += k;
}

int main()
{
	rng.seed(std::time(0));

	SSVU_BENCHMARK_INIT_GROUP_SCOPE_EXIT("New");
	SSVU_BENCHMARK_INIT_GROUP_SCOPE_EXIT("Params");

	for(int n = 0; n < times; ++n)
	{
		for(int i = 0; i < times2; ++i)
		{
			SSVU_BENCHMARK_RUN_GROUP_SCOPE_EXIT("New");
			//std::this_thread::sleep_for(std::chrono::milliseconds(1));
			testNew(0 + i, 100 + i);
		}

		for(int i = 0; i < times2; ++i)
		{
			SSVU_BENCHMARK_RUN_GROUP_SCOPE_EXIT("Params");
			testParams(0 + i, 100 + i);
		}
	}

	return 0;
}