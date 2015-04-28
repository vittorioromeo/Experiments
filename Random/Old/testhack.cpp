#include <iostream>
#include <type_traits>
#include <vector>
#include <SSVUtils/Core/Core.hpp>
#include <SSVUtils/Benchmark/Benchmark.hpp>

template<typename L, typename R, typename... Args> struct LambdaHack
{	
	using Storage = ssvu::AlignedStorageFor<L>;
	using Target = R(*)(Storage&, Args...);
	Storage storage;
	Target target;

	template<typename T> void initData(T&& arg)  { new(&storage) L(FWD(arg)); }
	template<typename LL> inline static R callTarget(Storage& ms, Args... args) { return ssvu::castStorage<LL>(ms)(args...); }

	template<typename LL> LambdaHack(Storage& ms, LL&&) : storage(ms), target(callTarget<LL>) { }
	template<typename... TArgs>inline R operator()(TArgs&&... args) { return target(storage, FWD(args)...); }
};



struct BenchmarkGroupRAII
{
	std::string groupName;
	inline BenchmarkGroupRAII(const std::string& mGroupName) : groupName{mGroupName} 
	{ 
		ssvu::Benchmark::groupReset(groupName);
	}
	inline ~BenchmarkGroupRAII() 
	{ 
		ssvu::Benchmark::groupEndLo(groupName);
	}
};

constexpr int maxItrs = 10000000;
volatile bool state{false};

int main()
{
	BenchmarkGroupRAII bgr0("hack");
	BenchmarkGroupRAII bgr1("func");

	volatile int captured = 7;
	auto l0 = [&captured](int x){ state = !state; captured += x; };
	auto l1 = [&captured](int x){ state = !state; captured -= x; };
	auto l2 = [&captured](int x){ state = !state; captured = x + 1; };

	using lhack = LambdaHack<decltype(l0), void, int>;
	lhack::Storage st;
	
	std::vector<lhack> v;
	v.emplace_back(st, l0);
	v.emplace_back(st, l1);
	v.emplace_back(st, l2);
	v[0].initData(l0);
	v[1].initData(l0);
	v[2].initData(l0);
	std::vector<ssvu::Func<void(int)>> v2{l0, l1, l2};

	for(int j = 0; j < 5; ++j)
	{
		{
			SSVU_BENCHMARK_RUN_GROUP_SCOPE_EXIT("hack");

			for(int i = 0; i < maxItrs; ++i)
				for(auto& l : v) 
					l(i);
		}


		{
			SSVU_BENCHMARK_RUN_GROUP_SCOPE_EXIT("func");

			for(int i = 0; i < maxItrs; ++i)
				for(auto& l : v2) 
					l(i);
		}
	}
}

