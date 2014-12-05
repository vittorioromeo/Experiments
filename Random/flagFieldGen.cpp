#include <SSVUtils/Core/Core.hpp>
#include <SSVUtils/Json/Json.hpp>

#define DEFINE_SIMPLE_SYNCFIELDPROXY_MUTABLE_OPERATION(mOp) \
	template<typename T> \
	inline auto& operator mOp (T&& mX) \
	noexcept(noexcept(std::declval<SyncFieldProxy<TI, TObj>>().get() mOp ssvu::fwd<T>(mX))) \
	{ \
		this->syncObj.template setBitAt<TI>(); \
		this->get() mOp ssvu::fwd<T>(mX); \
		return *this; \
	}

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

namespace syn
{
	using Idx = ssvu::SizeT;
	using ID = int;

	class SyncObjBase
	{
		public:
			virtual ~SyncObjBase() { }
	};

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

	struct ManagerHelper
	{
		template<typename TManager, ssvu::SizeT>
		inline static void initManager(TManager&)
		{
		
		}

		template<typename TManager, ssvu::SizeT TI, typename T, typename... TTypes>
		inline static void initManager(TManager& mManager)
		{
			for(auto i(0u); i < 100; ++i) 
			{
				
			}

			mManager.funcsCreate[TI] = [&mManager](ID mID){ mManager.template createImpl<T>(mID); };
			mManager.funcsRemove[TI] = [&mManager](ID mID){ mManager.template removeImpl<T>(mID); };
			mManager.funcsUpdate[TI] = [&mManager](ID mID){ mManager.template updateImpl<T>(mID); };

			initManager<TManager, TI + 1, TTypes...>(mManager);
		}
	};

	/*enum class SyncAction : ssvu::SizeT
	{
		None = 0,
		Create = 1,
		Remove = 2,
		Update = 3
	};

	template<typename... TTypes> class SyncState
	{
		public:
			static constexpr ssvu::SizeT typeCount{sizeof...(TTypes)};
			template<typename> using Dependent = std::vector<SyncAction>;
 
		private:
			std::array<std::array<SyncAction, 100>, typeCount> actions;

		public:
	};*/

	constexpr ssvu::SizeT maxObjs{100};

	template<template<typename> class TLFManager, typename... TTypes> class SyncManager 
	{
		friend struct ManagerHelper;

		public:
			static constexpr ssvu::SizeT typeCount{sizeof...(TTypes)};

			template<typename T> using LFManagerFor = TLFManager<T>;
			template<typename T> using HandleFor = typename LFManagerFor<T>::Handle;
			template<typename T> using HandleMapFor = std::map<ID, HandleFor<T>>;

		private:
			using TplLFManagers = std::tuple<LFManagerFor<TTypes>...>;
			using TplHandleMaps = std::tuple<HandleMapFor<TTypes>...>;
			using TplIDs = ssvu::TplRepeat<ID, typeCount>;

			using ObjBitset = std::bitset<maxObjs>;
			//using TplBitset = ssvu::TplRepeat<ID, typeCount>;
			
			TplLFManagers lfManagers;
			TplHandleMaps handleMaps;
			TplIDs lastIDs;

			std::array<ssvu::Func<void(ID)>, typeCount> funcsCreate;
			std::array<ssvu::Func<void(ID)>, typeCount> funcsRemove;
			std::array<ssvu::Func<void(ID)>, typeCount> funcsUpdate;

			std::array<ObjBitset, typeCount> bitsetIDs;			

			template<typename T> inline bool isPresent(ID mID) const noexcept
			{
				return bitsetIDs[getTypeID<T>()][mID];
			}
			template<typename T> inline void setPresent(ID mID, bool mX) noexcept
			{
				bitsetIDs[getTypeID<T>()][mID] = mX;
			}

			template<typename T> inline void createImpl(ID mID)
			{
				SSVU_ASSERT(!isPresent<T>(mID));
				setPresent<T>(mID, true);

				auto& handle(getHandleFor<T>(mID));
				handle = getLFManagerFor<T>().create();

				getHandleMapFor<T>()[mID] = handle;
			}
			template<typename T> inline void removeImpl(ID mID)
			{
				SSVU_ASSERT(isPresent<T>(mID));
				setPresent<T>(mID, false);

				auto& handle(getHandleFor<T>(mID));
				getLFManagerFor<T>().remove(handle);
				handle = getNullHandleFor<T>();

				getHandleMapFor<T>().erase(mID);
			}
			template<typename T> inline void updateImpl(ID mID)
			{
				SSVU_ASSERT(isPresent<T>(mID));

				auto& handle(getHandleFor<T>(mID));
				getLFManagerFor<T>().update(handle);

				// ??
			}	

		public:
			inline SyncManager()
			{
				ManagerHelper::initManager<SyncManager<TLFManager, TTypes...>, 0, TTypes...>(*this);
			}

			template<typename T> inline static constexpr ID getTypeID() noexcept
			{
				return ssvu::TplIdxOf<T, std::tuple<TTypes...>>::value;
			}

			template<typename T> inline ID getFirstFreeID() noexcept
			{
				constexpr ID typeID{getTypeID<T>()};
				return std::get<typeID>(lastIDs)++;
			}

			template<typename T> inline auto getNullHandleFor() noexcept
			{
				return getLFManagerFor<T>().getNullHandle();
			}

			template<typename T> inline auto& getLFManagerFor() noexcept
			{
				return std::get<LFManagerFor<T>>(lfManagers);
			}
			template<typename T> inline auto& getHandleMapFor() noexcept
			{
				return std::get<HandleMapFor<T>>(handleMaps);
			}
			template<typename T> inline auto& getHandleFor(ID mID) noexcept
			{
				return getHandleMapFor<T>()[mID];
			}
			///template<typename T> inline auto& getLastIDOf() noexcept
			//{
				//return std::get<ssvu::TplIdxOf<T, decltype(handles)>>(lastIDs);
			//}

			template<typename T> inline auto serverCreate()
			{
				auto createID(getFirstFreeID<T>());
				funcsCreate[getTypeID<T>()](createID);
				return getHandleMapFor<T>()[createID];
			}

			inline auto getChanged(ID mIDType, ID mID)
			{

			}

			inline void onReceivedPacketCreate(ID mIDType, ID mID)
			{
				funcsCreate[mIDType](mID);
			}

			inline void onReceivedPacketRemove(ID mIDType, ID mID)
			{
				funcsRemove[mIDType](mID);
			}

			inline void onReceivedPacketUpdate(ID mIDType, ID mID)
			{
				funcsUpdate[mIDType](mID);
			}

			inline auto getDiffWith(const SyncManager& mX)
			{
				ssvj::Val result{ssvj::Arr{}};
				auto iType(0u);

				ssvu::tplFor(handleMaps, [this, iType, &result, &mX](auto& mI) mutable
				{
					result.emplace(ssvj::Obj{});
					auto& jObj(result[iType]);

					jObj["create"] = ssvj::Arr{};
					jObj["remove"] = ssvj::Arr{};
					jObj["update"] = ssvj::Obj{};

					const auto& myBitset(bitsetIDs[iType]);
					const auto& otherBitset(mX.bitsetIDs[iType]);

					auto otherBitsetToCreate((~otherBitset) & myBitset);
					auto otherBitsetToRemove((~myBitset) & otherBitset);
					auto otherBitsetToUpdate(myBitset & otherBitset);

					for(auto i(0u); i < maxObjs; ++i)
					{
						if(otherBitsetToCreate[i]) jObj["create"].emplace(i);
						if(otherBitsetToRemove[i]) jObj["remove"].emplace(i);
						if(otherBitsetToUpdate[i]) 
						{
							jObj["update"][ssvu::toStr(i)] = mI[i]->toJsonChanged();
						}
					}

					++iType;
				});

				return result;
			}
	};
}

struct TestPlayer : syn::SyncObj
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

struct TestEnemy : syn::SyncObj
<
	float,			// X
	float,			// Y
	int			// Health
>
{
    public: 
        ProxyAt<0> x;
        ProxyAt<1> y;
        ProxyAt<2> health;

        inline TestEnemy() 
            :   x{get<0>()},
                y{get<1>()},
                health{get<2>()}
        {

        }
};

template<typename T> struct LifetimeManager;

template<> struct LifetimeManager<TestPlayer>
{
	using Handle = TestPlayer*;

	inline Handle getNullHandle() noexcept { return nullptr; }

	std::vector<ssvu::UPtr<TestPlayer>> storage;

	inline Handle create()
	{
		storage.emplace_back(ssvu::makeUPtr<TestPlayer>());
		return storage.back().get();
	}

	inline void remove(Handle mHandle)
	{
		ssvu::eraseRemoveIf(storage, [this, mHandle](const auto& mUPtr)
		{ 
			return mUPtr.get() == mHandle;
		});
	}

	inline void update(Handle mHandle)
	{

	}
};

template<> struct LifetimeManager<TestEnemy>
{
	using Handle = TestEnemy*;

	inline Handle getNullHandle() noexcept { return nullptr; }

	std::vector<ssvu::UPtr<TestEnemy>> storage;

	inline Handle create()
	{
		storage.emplace_back(ssvu::makeUPtr<TestEnemy>());
		return storage.back().get();
	}

	inline void remove(Handle mHandle)
	{
		ssvu::eraseRemoveIf(storage, [this, mHandle](const auto& mUPtr)
		{ 
			return mUPtr.get() == mHandle;
		});
	}

	inline void update(Handle mHandle)
	{

	}
};


int main()
{
	syn::SyncManager<LifetimeManager, TestPlayer, TestEnemy> server;
	syn::SyncManager<LifetimeManager, TestPlayer, TestEnemy> client;

	auto h1(server.serverCreate<TestPlayer>());

	ssvu::lo() << server.getDiffWith(client) << std::endl;
	//ssvu::lo() << client.getDiffWith(server) << std::endl;

	client.serverCreate<TestPlayer>();
	ssvu::lo() << server.getDiffWith(client) << std::endl;

	h1->x = 100;
	ssvu::lo() << server.getDiffWith(client) << std::endl;

	return 0;
}


	/*
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
	*/
