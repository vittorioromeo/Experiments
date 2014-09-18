#include <SSVUtils/Core/Core.hpp>
#include <SSVUtils/Benchmark/Benchmark.hpp>
#include <boost/container/flat_map.hpp>

volatile int vv{0};

struct BigObj
{
	volatile char abc[32];
	inline volatile void e() const { ++vv; }
};

namespace test
{
	template<typename T, typename TComparer> class SortedVector
	{
		private:
			std::vector<T> items;
			TComparer comparer;

		public:
			inline std::size_t insertAndGetIdx(T mValue)
			{
				auto lb(std::lower_bound(std::begin(items), std::end(items), mValue, comparer));
				auto idx(lb - std::begin(items));
				items.insert(lb, std::move(mValue));
				
				return idx;
			}				

			inline decltype(items)& getItems() noexcept { return items; }
			inline const decltype(items)& getItems() const noexcept { return items; }
	};	

	template<typename TKey, typename TValue> class DODMap
	{
		private:
			struct Comparer
			{
				inline bool operator()(const TKey& mA, const TKey& mB)
				{
					return mA < mB;
				}	
			};

			SortedVector<TKey, Comparer> keys;
			std::vector<TValue> values;


		public:
			inline void add(TKey mKey, TValue mValue)
			{
				auto idx(keys.insertAndGetIdx(std::move(mKey)));
				//values.reserve(keys.getItems().size());
				values.insert(std::begin(values) + idx, std::move(mValue));
			}

			inline TValue& lookup(const TKey& mKey)
			{
				auto lb(std::lower_bound(std::begin(keys.getItems()), std::end(keys.getItems()), mKey));
				auto idx(lb - std::begin(keys.getItems()));

				return values[mKey];
			}
	};	
}

int main()
{
	constexpr std::size_t kk = 1;
	constexpr std::size_t jj = 1000000;
	
	{
		SSVU_BENCHMARK_SCOPELOGBENCHMARK("std::map");
		std::map<std::size_t, BigObj> map;

		for(auto k(0u); k < kk; ++k)
		{
			{
				SSVU_BENCHMARK_SCOPELOGBENCHMARK("insertion");
				for(auto j(0u); j < jj; ++j) map[j] = {};				
			}

			{
				SSVU_BENCHMARK_SCOPELOGBENCHMARK("lookup");
				for(auto j(0u); j < jj; ++j) map.at(j).e();
			}
		}
	}

	{
		SSVU_BENCHMARK_SCOPELOGBENCHMARK("test::DODMap");
		test::DODMap<std::size_t, BigObj> map;

		for(auto k(0u); k < kk; ++k)
		{
			{
				SSVU_BENCHMARK_SCOPELOGBENCHMARK("insertion");
				for(auto j(0u); j < jj; ++j) map.add(j, {});				
			}

			{
				SSVU_BENCHMARK_SCOPELOGBENCHMARK("lookup");
				for(auto j(0u); j < jj; ++j) map.lookup(j).e();				
			}
		}
	}

	{
		SSVU_BENCHMARK_SCOPELOGBENCHMARK("boost::flat_map");
		boost::container::flat_map<std::size_t, BigObj> map;

		for(auto k(0u); k < kk; ++k)
		{
			{
				SSVU_BENCHMARK_SCOPELOGBENCHMARK("insertion");
				for(auto j(0u); j < jj; ++j) map[j] = {};				
			}

			{
				SSVU_BENCHMARK_SCOPELOGBENCHMARK("lookup");
				for(auto j(0u); j < jj; ++j) map.at(j).e();
			}
		}
	}

	return 0;
}