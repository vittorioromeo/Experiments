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

	template<typename TKey, typename TValue, typename TComparer = std::less<TKey>> class DODMap
	{
		private:
			constexpr static std::size_t defaultCapacity{10u};
			TComparer comparer;
			TKey* keys;
			TValue* values;
			std::size_t size, capacity;

			inline void grow()
			{
				capacity *= 2;

				auto newKeys(new TKey[capacity]);
				auto newValues(new TValue[capacity]);

				std::copy(keys, keys + size, newKeys);
				std::copy(values, values + size, newValues);

				delete[] keys;
				delete[] values;

				keys = newKeys;
				values = newValues;
			}

		public:
			inline DODMap() : keys{new TKey[defaultCapacity]}, values{new TValue[defaultCapacity]}, size{0u}, capacity{defaultCapacity} { }	
			inline ~DODMap() { delete[] keys; delete[] values; }

			inline void add(TKey mKey, TValue mValue)
			{		
				++size;
				if(size > capacity) grow();

   				auto i(static_cast<int>(size - 1));
    			for (; i > 0 && keys[i - 1] > mKey; --i)
    			{    			
        			keys[i] = keys[i - 1];
        			values[i] = values[i - 1];        			
    			}

    			keys[i] = mKey;
    			values[i] = mValue;
			}

			inline TValue& lookup(const TKey& mKey)
			{
				auto lb(std::lower_bound(keys, keys + size, mKey, comparer));
				auto idx(lb - keys);

				return values[idx];
			}
	};	
}

int rndd() { return ssvu::getRnd<std::size_t>(0, 1000); }

int main()
{
	constexpr std::size_t kk = 1;
	constexpr std::size_t jj = 1000000;
	
	{
		SSVU_BENCHMARK_LOG_SCOPE_EXIT("sorted vec");
		std::vector<std::pair<std::size_t, BigObj>> map;

		for(auto k(0u); k < kk; ++k)
		{
			{
				SSVU_BENCHMARK_LOG_SCOPE_EXIT("insertion");
				for(auto j(0u); j < jj; ++j) map.emplace_back(std::make_pair<std::size_t, BigObj>(j, {}));
			}

			{
				SSVU_BENCHMARK_LOG_SCOPE_EXIT("lookup");
				ssvu::sort(map, [](const auto& mA, const auto& mB){ return mA.first < mB.first; });
				for(auto j(0u); j < jj; ++j) ssvu::lowerBound(map, j, [](const auto& mA, const auto& mB){ return mA.first < mB; })->second.e();
			}
		}
	}

	{
		SSVU_BENCHMARK_LOG_SCOPE_EXIT("std::map");
		std::map<std::size_t, BigObj> map;

		for(auto k(0u); k < kk; ++k)
		{
			{
				SSVU_BENCHMARK_LOG_SCOPE_EXIT("insertion");
				for(auto j(0u); j < jj; ++j) map[j] = {};				
			}

			{
				SSVU_BENCHMARK_LOG_SCOPE_EXIT("lookup");
				for(auto j(0u); j < jj; ++j) map.at(j).e();
			}
		}
	}

	{
		SSVU_BENCHMARK_LOG_SCOPE_EXIT("test::DODMap");
		test::DODMap<std::size_t, BigObj> map;

		for(auto k(0u); k < kk; ++k)
		{
			{
				SSVU_BENCHMARK_LOG_SCOPE_EXIT("insertion");
				for(auto j(0u); j < jj; ++j) map.add(j, {});				
			}

			{
				SSVU_BENCHMARK_LOG_SCOPE_EXIT("lookup");
				for(auto j(0u); j < jj; ++j) map.lookup(j).e();				
			}
		}
	}

	{
		SSVU_BENCHMARK_LOG_SCOPE_EXIT("boost::flat_map");
		boost::container::flat_map<std::size_t, BigObj> map;

		for(auto k(0u); k < kk; ++k)
		{
			{
				SSVU_BENCHMARK_LOG_SCOPE_EXIT("insertion");
				for(auto j(0u); j < jj; ++j) map[j] = {};				
			}

			{
				SSVU_BENCHMARK_LOG_SCOPE_EXIT("lookup");
				for(auto j(0u); j < jj; ++j) map.at(j).e();
			}
		}
	}

	{
		SSVU_BENCHMARK_LOG_SCOPE_EXIT("std::map");
		std::map<std::size_t, BigObj> map;

		for(auto k(0u); k < kk; ++k)
		{
			{
				SSVU_BENCHMARK_LOG_SCOPE_EXIT("random insertion");
				for(auto j(0u); j < jj; ++j) map[rndd()] = {};				
			}
		}
	}

	{
		SSVU_BENCHMARK_LOG_SCOPE_EXIT("test::DODMap");
		test::DODMap<std::size_t, BigObj> map;

		for(auto k(0u); k < kk; ++k)
		{
			{
				SSVU_BENCHMARK_LOG_SCOPE_EXIT("random insertion");
				for(auto j(0u); j < jj; ++j) map.add(rndd(), {});				
			}
		}
	}

	{
		SSVU_BENCHMARK_LOG_SCOPE_EXIT("boost::flat_map");
		boost::container::flat_map<std::size_t, BigObj> map;

		for(auto k(0u); k < kk; ++k)
		{
			{
				SSVU_BENCHMARK_LOG_SCOPE_EXIT("random insertion");
				for(auto j(0u); j < jj; ++j) map[rndd()] = {};				
			}
		}
	}

	return 0;
}