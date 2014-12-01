#include <SSVUtils/Core/Core.hpp>
#include <SSVUtils/Json/Json.hpp>

using Idx = ssvu::SizeT;

class SyncObjBase
{
	public:
		virtual ~SyncObjBase() { }
};

#define DEFINE_SIMPLE_SYNCFIELDPROXY_MUTABLE_OPERATION(mOp) \
	template<typename T> \
	inline auto& operator mOp (T&& mX) \
	noexcept(noexcept(std::declval<SyncFieldProxy<TI, TObj>>().get() mOp ssvu::fwd<T>(mX))) \
	{ \
		this->syncObj.template setBitAt<TI>(); \
		this->get() mOp ssvu::fwd<T>(mX); \
		return *this; \
	} 

template<Idx TI, typename TObj> class SyncFieldProxy
{
	private:
		TObj& syncObj;
		
	public:
        using Type = typename ssvu::RemoveRef<decltype(syncObj)>::template TypeAt<TI>;

		inline SyncFieldProxy(TObj& mSyncObj) noexcept : syncObj{mSyncObj}
		{

		}

		template<typename T> inline auto& operator=(T&& mX) 
            noexcept(std::is_nothrow_assignable<Type, T>()) 
		{ 
            auto& field(syncObj.template getFieldAt<TI>());
            field = ssvu::fwd<T>(mX);
			syncObj.template setBitAt<TI>(); 
            return field;
		}

		DEFINE_SIMPLE_SYNCFIELDPROXY_MUTABLE_OPERATION(+=)
		DEFINE_SIMPLE_SYNCFIELDPROXY_MUTABLE_OPERATION(-=)
		DEFINE_SIMPLE_SYNCFIELDPROXY_MUTABLE_OPERATION(*=)
		DEFINE_SIMPLE_SYNCFIELDPROXY_MUTABLE_OPERATION(/=)
		DEFINE_SIMPLE_SYNCFIELDPROXY_MUTABLE_OPERATION(%=)

        inline auto& get() noexcept { return syncObj.template getFieldAt<TI>(); }
        inline const auto& get() const noexcept { return syncObj.template getFieldAt<TI>(); }

        inline auto& operator->() noexcept { return &get(); }
        inline const auto& operator->() const noexcept { return &get(); }
};

template<typename T> struct IsSyncFieldProxy
{
	using Type = std::false_type;
};
template<Idx TI, typename TObj> struct IsSyncFieldProxy<SyncFieldProxy<TI, TObj>>
{
	using Type = std::true_type;
};

#define ENABLEIF_IS_SYNCFIELDPROXY(mType) \
	ssvu::EnableIf<typename IsSyncFieldProxy<ssvu::RemoveAll<mType>>::Type{}>* = nullptr

#define SIMPLE_SYNCFIELDPROXY_OPERATION_TEMPLATE() \
	template<typename T, typename TP, ENABLEIF_IS_SYNCFIELDPROXY(TP)> 

#define SIMPLE_SYNCFIELDPROXY_OPERATION_BODY(mOp) \
	noexcept(noexcept(ssvu::fwd<TP>(mP).get() mOp ssvu::fwd<T>(mX))) \
	{ \
		return ssvu::fwd<TP>(mP).get() mOp ssvu::fwd<T>(mX); \
	} 

#define DEFINE_SIMPLE_SYNCFIELDPROXY_OPERATION(mOp) \
	SIMPLE_SYNCFIELDPROXY_OPERATION_TEMPLATE() \
	inline auto operator mOp (TP&& mP, T&& mX) \
	SIMPLE_SYNCFIELDPROXY_OPERATION_BODY(mOp) \
	\
	SIMPLE_SYNCFIELDPROXY_OPERATION_TEMPLATE() \
	inline auto operator mOp (T&& mX, TP&& mP) \
	SIMPLE_SYNCFIELDPROXY_OPERATION_BODY(mOp) 

DEFINE_SIMPLE_SYNCFIELDPROXY_OPERATION(+)
DEFINE_SIMPLE_SYNCFIELDPROXY_OPERATION(-)
DEFINE_SIMPLE_SYNCFIELDPROXY_OPERATION(*)
DEFINE_SIMPLE_SYNCFIELDPROXY_OPERATION(/)
DEFINE_SIMPLE_SYNCFIELDPROXY_OPERATION(%)

DEFINE_SIMPLE_SYNCFIELDPROXY_OPERATION(==)
DEFINE_SIMPLE_SYNCFIELDPROXY_OPERATION(!=)
DEFINE_SIMPLE_SYNCFIELDPROXY_OPERATION(>)
DEFINE_SIMPLE_SYNCFIELDPROXY_OPERATION(<)
DEFINE_SIMPLE_SYNCFIELDPROXY_OPERATION(>=)
DEFINE_SIMPLE_SYNCFIELDPROXY_OPERATION(<=)

template<typename... TArgs> class SyncObj : public SyncObjBase
{
	template<Idx, typename> friend class SyncFieldProxy;

	private:
		static constexpr ssvu::SizeT fieldCount{sizeof...(TArgs)};
		std::tuple<TArgs...> fields;
		std::bitset<fieldCount> fieldFlags;	

	public:
		template<Idx TI> using TypeAt = std::tuple_element_t<TI, decltype(fields)>;
        template<Idx TI> using ProxyAt = SyncFieldProxy<TI, SyncObj<TArgs...>>;

	private:
        template<Idx TI> inline auto& getFieldAt() noexcept { return std::get<TI>(fields); }
        template<Idx TI> inline void setBitAt() noexcept { fieldFlags[TI] = true; }

	public:
		template<Idx TI> inline auto get() noexcept { return ProxyAt<TI>{*this}; }	
		inline void resetFlags() noexcept { fieldFlags.reset(); }

		inline auto toJsonAll()
		{
			using namespace ssvj;
			
			Idx idx{0u};
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
			
			Idx idx{0u};
			Val v{Obj{}};
			
			ssvu::tplFor(fields, [this, idx, &v](auto&& mI) mutable
			{ 			
				if(fieldFlags[idx])	v[ssvu::toStr(idx)] = ssvu::fwd<decltype(mI)>(mI);								
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
        ProxyAt<0> x;
        ProxyAt<1> y;
        ProxyAt<2> health;
        ProxyAt<3> name;

        inline TestPlayer() 
            :   x{get<0>()},
                y{get<1>()},
                health{get<2>()},
                name{get<3>()}
        {

        }
};

int main()
{
	TestPlayer player;
	player.x = 10.f;
	player.y = 15.f + player.x;
	player.health = 100;
	player.name = "hello";

	ssvu::lo("JSON_ALL") << "\n" << player.toJsonAll() << "\n";
	ssvu::lo("JSON_CHANGED") << "\n" << player.toJsonChanged() << "\n";

	player.resetFlags();
	ssvu::lo("JSON_CHANGED") << "\n" << player.toJsonChanged() << "\n";

	player.resetFlags();
	player.y = 33.f;
	player.health -= 11;
	if(player.health > 40) player.health /= 2;
	ssvu::lo("JSON_CHANGED") << "\n" << player.toJsonChanged() << "\n";

	player.resetFlags();
	player.x = 11.f;
	player.name = "goodbye";
	ssvu::lo("JSON_CHANGED") << "\n" << player.toJsonChanged() << "\n";

	return 0;
}
