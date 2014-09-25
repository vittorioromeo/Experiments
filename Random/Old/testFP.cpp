#include <future>
#include <SSVUtils/Core/Core.hpp>
#include <SSVUtils/Benchmark/Benchmark.hpp>

int main() 
{
	std::vector<std::future<void>> vf;

	for(int i = 0; i < 100; ++i)
	{

		/*
		vf.emplace_back(std::async(std::launch::async, [i]
		{
			ssvu::lo("t" + ssvu::toStr(i)) << "starting t" + ssvu::toStr(i) << "\n";
			std::this_thread::sleep_for(std::chrono::milliseconds(ssvu::getRnd(350, 1250)));
			ssvu::lo("t" + ssvu::toStr(i)) << "finished t" + ssvu::toStr(i) << "\n";
		}));
		*/

		vf.emplace_back(std::async(std::launch::async, [i]
		{
			SSVU_BENCHMARK_LOG_SCOPE_EXIT("bench " + ssvu::toStr(i)); 
			std::this_thread::sleep_for(std::chrono::milliseconds(ssvu::getRnd(350, 1250)));
		}));

		vf.emplace_back(std::async(std::launch::async, [i]
		{
			SSVU_BENCHMARK_RUN_GROUP_SCOPE_EXIT("GroupScope" + ssvu::toStr(i % 5));
			std::this_thread::sleep_for(std::chrono::milliseconds(ssvu::getRnd(350, 1250)));
		}));
	}

	ssvu::lo().flush();

	for(int i = 0; i < 100; ++i)
	{
		vf[i].get();
	}

	for(int i = 0; i < 5; ++i)
	{
		ssvu::Benchmark::groupEndLo("GroupScope" + ssvu::toStr(i));
	}

	return 0;
}
