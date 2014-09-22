#include <SSVUtils/Core/Core.hpp>
#include <SSVUtils/Benchmark/Benchmark.hpp>

namespace tmi
{
	using CoutType = decltype(std::cout);
	using StdEndLine = CoutType&(CoutType&);

	enum class LCacheType : int {Fmt, Str, FOS, Commit, Manip};
	
	class LCache;

	class LCacheItem
	{
		friend class LCache;

		private:
			LCacheType type;
			union
			{
				std::string impl;
				StdEndLine manip;
			} u;
	};

	class LCache
	{	
		private:
			std::vector<LCacheItem> items;

			template<LCacheType TType> inline void append(const std::string& mImpl)
			{
				LCacheItem item;
				item.type = TType;
				item.impl = mImpl;
				items.emplace_back(item);
			}

		public:
			inline void fmt(const std::string& mImpl)	{ append<LCacheType::Fmt>(mImpl); }
			inline void str(const std::string& mImpl)	{ append<LCacheType::Str>(mImpl); }
			inline void fos(const std::string& mImpl)	{ append<LCacheType::FOS>(mImpl); }
			inline void commit()						{ append<LCacheType::Commit>(""); }
			inline void manip(StdEndLine mManip)		{ append<LCacheType::Commit>(""); }

			inline void exec(std::ostream& mStream)
			{
				auto iC(0u);
				while(iC < items.size() && items[iC].type != LCacheType::Commit) ++iC;
				

			}

			/*cache:
			M   left		|1
			M   setw		|2
			F   blue		|0
			S   number		|3
			FOS postfix		|4
			C 				|5*/
	};

	template<typename T> struct Stringifier;
	template<> struct Stringifier<unsigned int>
	{
		inline void impl(LCache& mCache, const unsigned int& mValue)
		{
			mCache.fmt(ssvu::Console::setColorFG(ssvu::Console::Color::Blue));
			mCache.str(std::to_string(mValue));
			mCache.fos("u");
			mCache.commit();
		}
	};
	template<> struct Stringifier<float>
	{
		inline void impl(LCache& mCache, const unsigned int& mValue)
		{
			mCache.fmt(ssvu::Console::setColorFG(ssvu::Console::Color::Red));
			mCache.str(std::to_string(mValue));
			mCache.fos(".f");
			mCache.commit();
		}
	};



	struct LOut
	{

	};

	inline auto& lo() noexcept { static LOut result; return result; }

	template<typename T> inline auto& operator<<(LOut& mLOut, const T& mValue)
	{
		return mLOut;
	}

	inline auto& operator<<(LOut& mLOut, StdEndLine mManip)
	{
		return mLOut;
	}
}

int main()
{
	
	return 0;
}

/*

stringifier<long>(number, postfix, cache)
{
	cache.append<Fmt>(blue);
	cache.append<Str>(toStr(number));
	cache.append<FOS>(postfix);
	cache.append<Commit>();
}

left << setw << 10ul << 5.f << setw << 15ul

cache:
	M   left
	M   setw
	F   blue
	S   number
	FOS postfix
	C

	F   red
	S   number
	FOS postfix
	C

	M   setw
	F   blue
	S   number
	FOS postfix
	C

...then...

cache:
	M   left		|1
	M   setw		|2
	F   blue		|0
	S   number		|3
	FOS postfix		|4
	C 				|5

	F   red 		|0     
	S   number		|1
	FOS postfix		|2
	C 				|3

	M   setw		|1
	F   blue		|0
	S   number		|2
	FOS postfix		|3
	C 				|4
*/