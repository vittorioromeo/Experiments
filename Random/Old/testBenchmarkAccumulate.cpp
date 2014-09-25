// #define SSVU_BENCHMARK_DISABLE 

#include <SSVUtils/Benchmark/Benchmark.hpp>
#include <SSVUtils/Test/Test.hpp>
#include <chrono>
#include <future>

#define SLEEP_PLOX() std::this_thread::sleep_for(std::chrono::milliseconds(25))

int main()
{
	SSVUT_RUN();

	{
		ssvu::Benchmark::groupReset("Group1");
		ssvu::Benchmark::groupReset("Group2");

		ssvu::Benchmark::groupResume("Group1");
		SLEEP_PLOX();
		ssvu::Benchmark::groupPause("Group1");

		ssvu::Benchmark::groupResume("Group2");
		SLEEP_PLOX();
		SLEEP_PLOX();
		SLEEP_PLOX();
		ssvu::Benchmark::groupPause("Group2");

		ssvu::Benchmark::groupResume("Group1");
		SLEEP_PLOX();
		ssvu::Benchmark::groupPause("Group1");

		ssvu::Benchmark::groupResume("Group2");
		SLEEP_PLOX();
		SLEEP_PLOX();
		SLEEP_PLOX();
		ssvu::Benchmark::groupPause("Group2");

		ssvu::Benchmark::groupEndLo("Group1");
		ssvu::Benchmark::groupEndLo("Group2");
	}

	{
		ssvu::Benchmark::groupReset("GroupScope1");
		ssvu::Benchmark::groupReset("GroupScope2");

		{
			SSVU_BENCHMARK_RUN_GROUP_SCOPE_EXIT("GroupScope1");
			SLEEP_PLOX();
		}

		{
			SSVU_BENCHMARK_RUN_GROUP_SCOPE_EXIT("GroupScope2");
			SLEEP_PLOX();
			SLEEP_PLOX();
			SLEEP_PLOX();
		}

		{
			SSVU_BENCHMARK_RUN_GROUP_SCOPE_EXIT("GroupScope1");
			SLEEP_PLOX();
		}

		{
			SSVU_BENCHMARK_RUN_GROUP_SCOPE_EXIT("GroupScope2");
			SLEEP_PLOX();
			SLEEP_PLOX();
			SLEEP_PLOX();
		}

		ssvu::Benchmark::groupEndLo("GroupScope1");
		ssvu::Benchmark::groupEndLo("GroupScope2");
	}

	return 0;
}