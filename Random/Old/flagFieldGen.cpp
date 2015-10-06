#include <SSVUtils/Core/Core.hpp>
#include <SSVUtils/Json/Json.hpp>

#define DEFINE_SIMPLE_SYNCFIELDPROXY_MUTABLE_OPERATION(mOp)               \
    template <typename T>                                                 \
    inline auto& operator mOp(T&& mX) noexcept(noexcept(                  \
        std::declval<FieldProxy<TI, TObj>>().get() mOp ssvu::fwd<T>(mX))) \
    {                                                                     \
        this->syncObj.template setBitAt<TI>();                            \
        this->get() mOp ssvu::fwd<T>(mX);                                 \
        return *this;                                                     \
    }

#define ENABLEIF_IS_SYNCFIELDPROXY(mType)                                    \
    ssvu::EnableIf<typename IsFieldProxy<ssvu::RemoveAll<mType>>::Type{}>* = \
        nullptr

#define SIMPLE_SYNCFIELDPROXY_OPERATION_TEMPLATE() \
    template <typename T, typename TP, ENABLEIF_IS_SYNCFIELDPROXY(TP)>

#define SIMPLE_SYNCFIELDPROXY_OPERATION_BODY(mOp)                    \
    noexcept(noexcept(ssvu::fwd<TP>(mP).get() mOp ssvu::fwd<T>(mX))) \
    {                                                                \
        return ssvu::fwd<TP>(mP).get() mOp ssvu::fwd<T>(mX);         \
    }

#define DEFINE_SIMPLE_SYNCFIELDPROXY_OPERATION(mOp)                \
    SIMPLE_SYNCFIELDPROXY_OPERATION_TEMPLATE()                     \
    inline auto operator mOp(TP&& mP, T&& mX)                      \
        SIMPLE_SYNCFIELDPROXY_OPERATION_BODY(mOp)                  \
                                                                   \
            SIMPLE_SYNCFIELDPROXY_OPERATION_TEMPLATE() inline auto \
            operator mOp(T&& mX, TP&& mP)                          \
                SIMPLE_SYNCFIELDPROXY_OPERATION_BODY(mOp)

namespace syn
{
    using Idx = ssvu::SizeT;
    using ID = int;

    namespace Impl
    {
        class ObjBase
        {
        public:
            virtual ~ObjBase() {}
        };
    }

    template <Idx TI, typename TObj>
    class FieldProxy
    {
    private:
        TObj& syncObj;

    public:
        using Type =
            typename ssvu::RemoveRef<decltype(syncObj)>::template TypeAt<TI>;

        inline FieldProxy(TObj& mSyncObj) noexcept : syncObj{mSyncObj} {}

        template <typename T>
        inline auto& operator=(T&& mX) noexcept(
            std::is_nothrow_assignable<Type, T>())
        {
            auto& field(syncObj.template getFieldAt<TI>());
            field = ssvu::fwd<T>(mX);
            syncObj.template setBitAt<TI>();
            return field;
        }

        DEFINE_SIMPLE_SYNCFIELDPROXY_MUTABLE_OPERATION(+= )
        DEFINE_SIMPLE_SYNCFIELDPROXY_MUTABLE_OPERATION(-= )
        DEFINE_SIMPLE_SYNCFIELDPROXY_MUTABLE_OPERATION(*= )
        DEFINE_SIMPLE_SYNCFIELDPROXY_MUTABLE_OPERATION(/= )
        DEFINE_SIMPLE_SYNCFIELDPROXY_MUTABLE_OPERATION(%= )

        inline auto& get() noexcept
        {
            return syncObj.template getFieldAt<TI>();
        }
        inline const auto& get() const noexcept
        {
            return syncObj.template getFieldAt<TI>();
        }

        inline auto& operator-> () noexcept { return &get(); }
        inline const auto& operator-> () const noexcept { return &get(); }
    };

    template <typename T>
    struct IsFieldProxy
    {
        using Type = std::false_type;
    };

    template <Idx TI, typename TObj>
    struct IsFieldProxy<FieldProxy<TI, TObj>>
    {
        using Type = std::true_type;
    };

    DEFINE_SIMPLE_SYNCFIELDPROXY_OPERATION(+)
    DEFINE_SIMPLE_SYNCFIELDPROXY_OPERATION(-)
    DEFINE_SIMPLE_SYNCFIELDPROXY_OPERATION(*)
    DEFINE_SIMPLE_SYNCFIELDPROXY_OPERATION(/ )
    DEFINE_SIMPLE_SYNCFIELDPROXY_OPERATION(% )

    DEFINE_SIMPLE_SYNCFIELDPROXY_OPERATION(== )
    DEFINE_SIMPLE_SYNCFIELDPROXY_OPERATION(!= )
    DEFINE_SIMPLE_SYNCFIELDPROXY_OPERATION(> )
    DEFINE_SIMPLE_SYNCFIELDPROXY_OPERATION(< )
    DEFINE_SIMPLE_SYNCFIELDPROXY_OPERATION(>= )
    DEFINE_SIMPLE_SYNCFIELDPROXY_OPERATION(<= )

    template <typename TObj>
    struct SerializationHelper
    {
        inline static void setFromJson(const ssvj::Val& mVal, TObj& mObj)
        {
            ssvu::tplForIdx(
                [&mVal, &mObj](auto mIdx, auto& mField)
                {
                    auto key(ssvu::toStr(mIdx));

                    if(mVal.has(key))
                    {
                        mField = mVal[key]
                                     .template as<
                                         ssvu::RemoveAll<decltype(mField)>>();
                    }

                },
                mObj.fields);
        }

        inline static auto getAllToJson(const TObj& mObj)
        {
            ssvj::Val result{ssvj::Obj{}};

            ssvu::tplForIdx(
                [&result, &mObj](auto mIdx, auto&& mField)
                {
                    auto key(ssvu::toStr(mIdx));
                    result[key] = ssvu::fwd<decltype(mField)>(mField);

                },
                mObj.fields);

            return result;
        }

        inline static auto getChangedToJson(const TObj& mObj)
        {
            ssvj::Val result{ssvj::Obj{}};

            ssvu::tplForIdx(
                [&result, &mObj](auto mIdx, auto&& mField)
                {
                    if(mObj.fieldFlags[mIdx])
                    {
                        auto key(ssvu::toStr(mIdx));
                        result[key] = ssvu::fwd<decltype(mField)>(mField);
                    }

                },
                mObj.fields);

            return result;
        }
    };

    template <typename... TArgs>
    class SyncObj : public Impl::ObjBase
    {
        template <Idx, typename>
        friend class FieldProxy;
        template <typename>
        friend struct SerializationHelper;

    public:
        using TplFields = std::tuple<TArgs...>;

    private:
        static constexpr ssvu::SizeT fieldCount{sizeof...(TArgs)};
        TplFields fields;
        std::bitset<fieldCount> fieldFlags;

    public:
        template <Idx TI>
        using TypeAt = std::tuple_element_t<TI, decltype(fields)>;
        template <Idx TI>
        using ProxyAt = FieldProxy<TI, SyncObj<TArgs...>>;

    private:
        template <Idx TI>
        inline auto& getFieldAt() noexcept
        {
            return std::get<TI>(fields);
        }
        template <Idx TI>
        inline void setBitAt() noexcept
        {
            fieldFlags[TI] = true;
        }

    public:
        template <Idx TI>
        inline auto get() noexcept
        {
            return ProxyAt<TI>{*this};
        }
        inline void resetFlags() noexcept { fieldFlags.reset(); }

        inline void setFromJson(const ssvj::Val& mX)
        {
            SerializationHelper<SyncObj>::setFromJson(mX, *this);
        }
        inline auto toJsonAll()
        {
            return SerializationHelper<SyncObj>::getAllToJson(*this);
        }
        inline auto toJsonChanged()
        {
            return SerializationHelper<SyncObj>::getChangedToJson(*this);
        }
    };

    struct ManagerHelper
    {
        template <typename TManager, ssvu::SizeT>
        inline static void initManager(TManager&)
        {
        }

        template <typename TManager, ssvu::SizeT TI, typename T,
            typename... TTypes>
        inline static void initManager(TManager& mManager)
        {
            mManager.funcsCreate[TI] = &TManager::template createImpl<T>;
            mManager.funcsRemove[TI] = &TManager::template removeImpl<T>;
            mManager.funcsUpdate[TI] = &TManager::template updateImpl<T>;

            initManager<TManager, TI + 1, TTypes...>(mManager);
        }
    };

    constexpr ssvu::SizeT maxObjs{100};

    template <template <typename> class TLFManager, typename... TTypes>
    class SyncManager
    {
        friend struct ManagerHelper;

    public:
        static constexpr ssvu::SizeT typeCount{sizeof...(TTypes)};

        template <typename T>
        using LFManagerFor = TLFManager<T>;
        template <typename T>
        using HandleFor = typename LFManagerFor<T>::Handle;
        template <typename T>
        using HandleMapFor = std::map<ID, HandleFor<T>>;

    private:
        using TplLFManagers = std::tuple<LFManagerFor<TTypes>...>;
        using TplHandleMaps = std::tuple<HandleMapFor<TTypes>...>;
        using TplIDs = ssvu::TplRepeat<ID, typeCount>;

        using ObjBitset = std::bitset<maxObjs>;
        // using TplBitset = ssvu::TplRepeat<ID, typeCount>;

        using MemFnCreate = void (SyncManager<TLFManager, TTypes...>::*)(
            ID, const ssvj::Val&);
        using MemFnRemove = void (SyncManager<TLFManager, TTypes...>::*)(ID);
        using MemFnUpdate = void (SyncManager<TLFManager, TTypes...>::*)(
            ID, const ssvj::Val&);

        TplLFManagers lfManagers;
        TplHandleMaps handleMaps;
        TplIDs lastIDs;

        std::array<MemFnCreate, typeCount> funcsCreate;
        std::array<MemFnRemove, typeCount> funcsRemove;
        std::array<MemFnUpdate, typeCount> funcsUpdate;

        std::array<ObjBitset, typeCount> bitsetIDs;

        void testFn(int) {}

        template <typename T>
        inline auto& getBitsetFor() noexcept
        {
            return bitsetIDs[getTypeID<T>()];
        }
        template <typename T>
        inline const auto& getBitsetFor() const noexcept
        {
            return bitsetIDs[getTypeID<T>()];
        }
        template <typename T>
        inline bool isPresent(ID mID) const noexcept
        {
            return getBitsetFor<T>()[mID];
        }
        template <typename T>
        inline void setPresent(ID mID, bool mX) noexcept
        {
            getBitsetFor<T>()[mID] = mX;
        }

        template <typename T>
        inline void createImpl(ID mID, const ssvj::Val& mVal)
        {
            SSVU_ASSERT(!isPresent<T>(mID));
            setPresent<T>(mID, true);

            auto& handle(getHandleFor<T>(mID));
            handle = getLFManagerFor<T>().create();

            handle->setFromJson(mVal);

            getHandleMapFor<T>()[mID] = handle;
        }
        template <typename T>
        inline void removeImpl(ID mID)
        {
            SSVU_ASSERT(isPresent<T>(mID));
            setPresent<T>(mID, false);

            auto& handle(getHandleFor<T>(mID));
            getLFManagerFor<T>().remove(handle);
            handle = getNullHandleFor<T>();

            getHandleMapFor<T>().erase(mID);
        }
        template <typename T>
        inline void updateImpl(ID mID, const ssvj::Val& mVal)
        {
            SSVU_ASSERT(isPresent<T>(mID));

            auto& handle(getHandleFor<T>(mID));
            getLFManagerFor<T>().update(handle);

            handle->setFromJson(mVal);
        }

    public:
        inline SyncManager()
        {
            ManagerHelper::initManager<SyncManager<TLFManager, TTypes...>, 0,
                TTypes...>(*this);
        }

        template <typename T>
        inline static constexpr ID getTypeID() noexcept
        {
            return ssvu::TplIdxOf<T, std::tuple<TTypes...>>::value;
        }

        template <typename T>
        inline ID getFirstFreeID() noexcept
        {
            constexpr ID typeID{getTypeID<T>()};
            return std::get<typeID>(lastIDs)++;
        }

        template <typename T>
        inline auto getNullHandleFor() noexcept
        {
            return getLFManagerFor<T>().getNullHandle();
        }

        template <typename T>
        inline auto& getLFManagerFor() noexcept
        {
            return std::get<LFManagerFor<T>>(lfManagers);
        }
        template <typename T>
        inline auto& getHandleMapFor() noexcept
        {
            return std::get<HandleMapFor<T>>(handleMaps);
        }
        template <typename T>
        inline auto& getHandleFor(ID mID) noexcept
        {
            return getHandleMapFor<T>()[mID];
        }
        /// template<typename T> inline auto& getLastIDOf() noexcept
        //{
        // return std::get<ssvu::TplIdxOf<T, decltype(handles)>>(lastIDs);
        //}

        template <typename T>
        inline auto serverCreate(const ssvj::Val& mVal)
        {
            auto createID(getFirstFreeID<T>());
            (this->*funcsCreate[getTypeID<T>()])(createID, mVal);
            return getHandleMapFor<T>()[createID];
        }

        inline auto getChanged(ID mIDType, ID mID) {}

        inline void onReceivedPacketCreate(
            ID mIDType, ID mID, const ssvj::Val& mVal)
        {
            (this->*funcsCreate[mIDType])(mID, mVal);
        }

        inline void onReceivedPacketRemove(ID mIDType, ID mID)
        {
            (this->*funcsRemove[mIDType])(mID);
        }

        inline void onReceivedPacketUpdate(
            ID mIDType, ID mID, const ssvj::Val& mVal)
        {
            (this->*funcsUpdate[mIDType])(mID, mVal);
        }

        struct Diff
        {
            struct DiffTypeData
            {
                std::map<ID, ssvj::Val> toCreate, toUpdate;
                std::vector<ID> toRemove;

                inline auto toJson() const
                {
                    using namespace ssvj;

                    Val result{Obj{}};

                    result["create"] = Obj{};
                    result["remove"] = Arr{};
                    result["update"] = Obj{};

                    auto& jCreate(result["create"]);
                    auto& jRemove(result["remove"]);
                    auto& jUpdate(result["update"]);

                    for(const auto& x : toCreate)
                        jCreate[ssvu::toStr(x.first)] = x.second;
                    for(const auto& x : toRemove) jRemove.emplace(x);
                    for(const auto& x : toUpdate)
                        jUpdate[ssvu::toStr(x.first)] = x.second;

                    return result;
                }
            };

            ssvu::TplRepeat<DiffTypeData, typeCount> diffTypeDatas;

            inline auto toJson()
            {
                using namespace ssvj;

                Val result{Arr{}};
                ssvu::tplFor(
                    [this, &result](const auto& mI)
                    {
                        result.emplace(mI.toJson());
                    },
                    diffTypeDatas);
                return result;
            }
        };

        inline auto getDiffWith(const SyncManager& mX)
        {
            Diff result;

            ssvu::tplForIdx(
                [this, &result, &mX](auto mIType, auto& mI, auto& mDTD) mutable
                {
                    const auto& myBitset(bitsetIDs[mIType]);
                    const auto& otherBitset(mX.bitsetIDs[mIType]);

                    auto otherBitsetToCreate((~otherBitset) & myBitset);
                    auto otherBitsetToRemove((~myBitset) & otherBitset);
                    auto otherBitsetToUpdate(myBitset & otherBitset);

                    for(auto i(0u); i < maxObjs; ++i)
                    {
                        if(otherBitsetToCreate[i])
                            mDTD.toCreate[i] = mI[i]->toJsonAll();
                        if(otherBitsetToRemove[i])
                            mDTD.toRemove.emplace_back(i);
                        if(otherBitsetToUpdate[i])
                            mDTD.toUpdate[i] = mI[i]->toJsonChanged();
                    }
                },
                handleMaps, result.diffTypeDatas);

            return result;
        }

        inline void applyDiff(const Diff& mX)
        {
            ssvu::tplForIdx(
                [this, &mX](auto mIType, auto& mI, auto& mDTD) mutable
                {
                    for(auto i(0u); i < maxObjs; ++i)
                    {
                        for(const auto& p : mDTD.toCreate)
                            this->onReceivedPacketCreate(
                                mIType, p.first, p.second);
                        for(auto i : mDTD.toRemove)
                            this->onReceivedPacketRemove(mIType, i);
                        for(const auto& p : mDTD.toUpdate)
                            this->onReceivedPacketUpdate(
                                mIType, p.first, p.second);
                    }
                },
                handleMaps, mX.diffTypeDatas);
        }
    };
}

struct TestPlayer : syn::SyncObj<float, // X
                        float,          // Y
                        int,            // Health
                        std::string     // Name
                        >
{
public:
    ProxyAt<0> x;
    ProxyAt<1> y;
    ProxyAt<2> health;
    ProxyAt<3> name;

    inline TestPlayer()
        : x{get<0>()}, y{get<1>()}, health{get<2>()}, name{get<3>()}
    {
    }
};

struct TestEnemy : syn::SyncObj<float, // X
                       float,          // Y
                       int             // Health
                       >
{
public:
    ProxyAt<0> x;
    ProxyAt<1> y;
    ProxyAt<2> health;

    inline TestEnemy() : x{get<0>()}, y{get<1>()}, health{get<2>()} {}
};

template <typename T>
struct LifetimeManager;

template <>
struct LifetimeManager<TestPlayer>
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

    inline void update(Handle mHandle) {}
};

template <>
struct LifetimeManager<TestEnemy>
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

    inline void update(Handle mHandle) {}
};


int main()
{
    syn::SyncManager<LifetimeManager, TestPlayer, TestEnemy> server;
    syn::SyncManager<LifetimeManager, TestPlayer, TestEnemy> client;

    ssvj::Val temp{ssvj::Obj{}};
    temp["0"] = 10.f;
    temp["1"] = 25.f;
    temp["2"] = 100;
    temp["3"] = "banana";

    auto h1(server.serverCreate<TestPlayer>(temp));

    ssvu::lo() << server.getDiffWith(client).toJson() << std::endl;
    // ssvu::lo() << client.getDiffWith(server) << std::endl;

    // client.serverCreate<TestPlayer>();
    // ssvu::lo() << server.getDiffWith(client).toJson() << std::endl;

    // h1->x = 100;
    // ssvu::lo() << server.getDiffWith(client).toJson() << std::endl;

    client.applyDiff(server.getDiffWith(client));
    ssvu::lo() << server.getDiffWith(client).toJson() << std::endl;

    auto h1c(client.getHandleFor<TestPlayer>(0));

    ssvu::lo() << h1c->toJsonAll() << std::endl;

    // ssvu::lo() << sizeof h1->x << std::endl;

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
