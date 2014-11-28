#include <SSVUtils/Core/Core.hpp>
#include <SSVUtils/Json/Json.hpp>

class SyncObjBase
{
	public:
		virtual ~SyncObjBase() { }
};

template<ssvu::SizeT TI, typename TObj> class SyncFieldProxy
{
	private:
		TObj& syncObj;
		
	public:
		inline SyncFieldProxy(TObj& mSyncObj) noexcept : syncObj{mSyncObj}
		{

		}

		template<typename T> inline void operator=(T&& mX) 
		{ 
			syncObj.template setAt<TI>(ssvu::fwd<T>(mX)); 
		}
};

template<typename... TArgs> class SyncObj : public SyncObjBase
{
	template<ssvu::SizeT, typename> friend class SyncFieldProxy;

	private:
		static constexpr ssvu::SizeT fieldCount{sizeof...(TArgs)};
		std::tuple<TArgs...> fields;
		std::bitset<fieldCount> fieldFlags;	

	public:
        using TestType = SyncObj<TArgs...>;

		template<ssvu::SizeT TI> 
		using TypeAt = std::tuple_element_t<TI, decltype(fields)>;

	private:
		template<ssvu::SizeT TI, typename T> inline void setAt(T&& mX)
		{
			fieldFlags[TI] = true;
			std::get<TI>(fields) = ssvu::fwd<T>(mX);		
		}

	public:
		template<ssvu::SizeT TI> inline auto get() noexcept
		{
			return SyncFieldProxy<TI, TestType>(*this);
		}	

		inline void resetFlags() noexcept { fieldFlags.reset(); }

		inline auto toJsonAll()
		{
			using namespace ssvj;
			
			ssvu::SizeT idx{0u};
			Val v{Obj{}};
			
			ssvu::tplFor(fields, [idx, &v](auto&& mI) mutable
			{ 
				v[ssvu::toStr(idx)] = ssvu::fwd<decltype(mI)>(mI);				
				
				++idx;
			});

			return v;
		}
		
		inline auto toJsonChanged()
		{
			using namespace ssvj;
			
			ssvu::SizeT idx{0u};
			Val v{Obj{}};
			
			ssvu::tplFor(fields, [this, idx, &v](auto&& mI) mutable
			{ 			
				if(fieldFlags[idx])	
					v[ssvu::toStr(idx)] = ssvu::fwd<decltype(mI)>(mI);				
				
				++idx;
			});

			return v;
		}
};

struct TestPlayer : SyncObj
<
	float,			// X
	float,			// Y
	int,			// Health
	std::string		// Name
>
{
    public: 
        SyncFieldProxy<0, TestType> x;

        inline TestPlayer() : x{get<0>()} { }
};

int main()
{
	TestPlayer player;
	player.get<0>() = 10.f;
	player.get<1>() = 15.f;
	player.get<2>() = 100;
	player.get<3>() = "hello";

	ssvu::lo("JSON_ALL") << "\n" << player.toJsonAll() << "\n";
	ssvu::lo("JSON_CHANGED") << "\n" << player.toJsonChanged() << "\n";

	player.resetFlags();
	ssvu::lo("JSON_CHANGED") << "\n" << player.toJsonChanged() << "\n";

	player.resetFlags();
	player.get<2>() = 33.f;
	ssvu::lo("JSON_CHANGED") << "\n" << player.toJsonChanged() << "\n";

	player.resetFlags();
	player.x = 11.f;
	player.get<2>() = 33.f;
	ssvu::lo("JSON_CHANGED") << "\n" << player.toJsonChanged() << "\n";

	return 0;
}
