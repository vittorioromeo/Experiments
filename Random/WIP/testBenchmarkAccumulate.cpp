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
		ssvu::Benchmark::resetGroup("Group1");
		ssvu::Benchmark::resetGroup("Group2");

		ssvu::Benchmark::resumeGroup("Group1");
		SLEEP_PLOX();
		ssvu::Benchmark::pauseGroup("Group1");

		ssvu::Benchmark::resumeGroup("Group2");
		SLEEP_PLOX();
		SLEEP_PLOX();
		SLEEP_PLOX();
		ssvu::Benchmark::pauseGroup("Group2");

		ssvu::Benchmark::resumeGroup("Group1");
		SLEEP_PLOX();
		ssvu::Benchmark::pauseGroup("Group1");

		ssvu::Benchmark::resumeGroup("Group2");
		SLEEP_PLOX();
		SLEEP_PLOX();
		SLEEP_PLOX();
		ssvu::Benchmark::pauseGroup("Group2");

		ssvu::Benchmark::endLoGroup("Group1");
		ssvu::Benchmark::endLoGroup("Group2");
	}

	{
		ssvu::Benchmark::resetGroup("GroupScope1");
		ssvu::Benchmark::resetGroup("GroupScope2");

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

		ssvu::Benchmark::endLoGroup("GroupScope1");
		ssvu::Benchmark::endLoGroup("GroupScope2");
	}

	return 0;
}