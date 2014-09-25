#include <SSVUtils/Benchmark/Benchmark.hpp>
#include <SSVUtils/Test/Test.hpp>
#include <chrono>
#include <future>

int main()
{
	SSVUT_RUN();

	{
		ssvu::Benchmark::start("Test, 1 seconds (normal syntax)");
		std::this_thread::sleep_for(std::chrono::seconds(1));
		ssvu::Benchmark::endLo();
	}

	{
		SSVU_BENCHMARK_LOG_SCOPE_EXIT("Test, 1 second (scope syntax)"); 
		std::this_thread::sleep_for(std::chrono::seconds(1));		
	}

	{
		ssvu::Benchmark::start("Test nested, 1 seconds (normal syntax)");
		std::this_thread::sleep_for(std::chrono::seconds(1));
		{
			ssvu::Benchmark::start("Test nested, 1 seconds (normal syntax)");
			std::this_thread::sleep_for(std::chrono::seconds(1));
			ssvu::Benchmark::endLo();
		}
		ssvu::Benchmark::endLo();
	}

	{
		SSVU_BENCHMARK_LOG_SCOPE_EXIT("Test nested0, 1 second (scope syntax)"); 
		std::this_thread::sleep_for(std::chrono::seconds(1));		
	
		{
			SSVU_BENCHMARK_LOG_SCOPE_EXIT("Test nested1, 1 second (scope syntax)"); 
			std::this_thread::sleep_for(std::chrono::seconds(1));	

			{
				SSVU_BENCHMARK_LOG_SCOPE_EXIT("Test nested2, 1 second (scope syntax)"); 
				std::this_thread::sleep_for(std::chrono::seconds(1));		
			}	

			{
				SSVU_BENCHMARK_LOG_SCOPE_EXIT("Test nested2, 1 second (scope syntax)"); 
				std::this_thread::sleep_for(std::chrono::seconds(1));		
			}
		}

		{
			SSVU_BENCHMARK_LOG_SCOPE_EXIT("Test nested1, 1 second (scope syntax)"); 
			std::this_thread::sleep_for(std::chrono::seconds(1));		
		}
	}

	return 0;
}