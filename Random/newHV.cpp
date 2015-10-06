#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/Range/Range.hpp"
#include "SSVUtils/GrowableArray/GrowableArray.hpp"
//#include "SSVUtils/HandleVector/Internal/Common.hpp"
#include "SSVUtils/UnionVariant/Internal/CTUtils.hpp"
#include "SSVUtils/Internal/SharedFuncs.hpp"

namespace ssvu
{
    namespace Internal
    {
        /// @brief Data structure keeping track of the validity of an item.
        struct HVMark
        {
            HIdx statIdx;
            HCtr ctr;

            inline HVMark(HIdx mStatIdx) noexcept : statIdx{mStatIdx} {}
        };

        /// @brief Data structure keeping track of the status of an item.
        struct HVStat
        {
            HIdx markIdx;
            bool alive{false};

            inline HVStat(HIdx mMarkIdx) noexcept : markIdx{mMarkIdx} {}
        };

        template <typename T>
        class HVHandleBase
        {
        protected:
            /// @brief Internal pointer to the HandleVector.
            T* hVec;

            /// @brief Index of the mark to check.
            HIdx markIdx;

            /// @brief Counter of the handle. Will be compared to the mark's
            /// counter.
            HCtr ctr;

        public:
            inline HVHandleBase(T& mHVec, HIdx mMarkIdx, HCtr mCtr) noexcept
                : hVec{&mHVec},
                  markIdx{mMarkIdx},
                  ctr{mCtr}
            {
            }

            /// @brief Returns whether the handle is valid or not.
            /// @details The handle is considered valid only when it points to
            /// the item it originally pointed to.
            /// If the item is set to dead but the HandleVector was not yet
            /// refreshed, the handle is considered valid.
            bool isAlive() const noexcept;

            /// @brief Sets the pointed item's status as dead.
            void setDead() noexcept;
        };
    }

    /// @brief Handle class that points to HandleVector elements.
    template <typename T>
    class HVHandleSingle : public Internal::HVHandleBase<HVSingle<T>>
    {
        template <typename, typename>
        friend class ssvu::Internal::HVImpl;

    private:
        inline HVHandleSingle(
            HVSingle<T>& mHVec, HIdx mMarkIdx, HCtr mCtr) noexcept
            : Internal::HVHandleBase<HVSingle<T>>{mHVec, mMarkIdx, mCtr}
        {
        }

        /// @brief Internal implementation method that returns a reference or a
        /// const reference to the item.
        template <typename TR>
        inline TR getImpl() noexcept
        {
            SSVU_ASSERT(this->isAlive());
            return this->hVec->getItemFromMark(this->markIdx);
        }

    public:
        /// @brief Returns a reference to the data. Assumes the handle is valid.
        inline T& get() noexcept { return getImpl<T&>(); }

        /// @brief Returns a const reference to the data. Assumes the handle is
        /// valid.
        inline const T& get() const noexcept { return getImpl<const T&>(); }

        // Pointer-like interface
        inline T& operator*() noexcept { return get(); }
        inline const T& operator*() const noexcept { return get(); }
        inline T* operator->() noexcept { return &(get()); }
        inline const T* operator->() const noexcept { return &(get()); }
    };

    /// @brief Handle class that points to HandleVector elements.
    template <typename... TTs>
    class HVHandleMulti : public Internal::HVHandleBase<HVMulti<TTs...>>
    {
        template <typename, typename>
        friend class ssvu::Internal::HVImpl;

    private:
        inline HVHandleMulti(
            HVMulti<TTs...>& mHVec, HIdx mMarkIdx, HCtr mCtr) noexcept
            : Internal::HVHandleBase<HVMulti<TTs...>>{mHVec, mMarkIdx, mCtr}
        {
        }

        /// @brief Internal implementation method that returns a reference or a
        /// const reference to the item tuple.
        template <typename T, typename TR>
        inline TR getImpl() noexcept
        {
            SSVU_ASSERT(this->isAlive());
            return this->hVec->template getItemFromMark<T>(this->markIdx);
        }

    public:
        inline auto getRefTpl() noexcept
        {
            return std::tuple<TTs&...>{getImpl<TTs, TTs&>()...};
        }
        inline auto getRefTpl() const noexcept
        {
            return std::tuple<const TTs&...>{getImpl<TTs, const TTs&>()...};
        }

        /// @brief Returns a reference to the data. Assumes the handle is valid.
        template <typename T>
        inline T& get() noexcept
        {
            return getImpl<T, T&>();
        }

        /// @brief Returns a const reference to the data. Assumes the handle is
        /// valid.
        template <typename T>
        inline const T& get() const noexcept
        {
            return getImpl<T, const T&>();
        }

        // Pointer-like interface
        inline auto operator*() noexcept { return getRefTpl(); }
        inline auto operator*() const noexcept { return getRefTpl(); }
    };

    namespace Internal
    {
        /// @brief Base CTRP HandleVector class.
        template <typename TDerived, typename THandle>
        class HVImpl
        {
            template <typename>
            friend class ssvu::Internal::HVHandleBase;

        public:
            using Stat = Internal::HVStat;
            using Mark = Internal::HVMark;
            using Handle = THandle;

        private:
            inline auto& getTD() noexcept { return castUp<TDerived>(*this); }

        protected:
            SizeT capacity{0u};
            SizeT size{0u};
            SizeT sizeNext{0u};

            GrowableArray<Stat> stats;
            GrowableArray<Mark> marks;

            /// @brief Increases internal storage capacity by mAmount.
            inline void growCapacityBy(SizeT mAmount)
            {
                auto capacityNew(capacity + mAmount);
                SSVU_ASSERT(capacityNew >= 0 && capacityNew >= capacity);

                getTD().growImpl(capacity, capacityNew);
                stats.grow(capacity, capacityNew);
                marks.grow(capacity, capacityNew);

                // Initialize resized storage and set `capacity` to
                // `capacityNew`.
                for(; capacity < capacityNew; ++capacity)
                {
                    stats.initAt(capacity, capacity);
                    marks.initAt(capacity, capacity);
                }
            }

            /// @brief Sets internal storage capacity to mCapacity.
            inline void growCapacityTo(SizeT mCapacityNew)
            {
                SSVU_ASSERT(capacity < mCapacityNew);
                growCapacityBy(mCapacityNew - capacity);
            }

            /// @brief Checks if the current capacity is enough - if it isn't,
            /// increases it.
            inline void growIfNeeded()
            {
                constexpr float growMultiplier{2.f};
                constexpr SizeT growAmount{5};

                if(SSVU_LIKELY(capacity > sizeNext)) return;
                growCapacityTo((capacity + growAmount) * growMultiplier);
            }

            inline auto& getMarkFromStat(HIdx mStatIdx) noexcept
            {
                return marks[stats[mStatIdx].markIdx];
            }
            inline auto& getStatFromMark(HIdx mMarkIdx) noexcept
            {
                return stats[marks[mMarkIdx].statIdx];
            }

            inline void setDeadFromMark(HIdx mMarkIdx) noexcept
            {
                getStatFromMark(mMarkIdx).alive = false;
            }

        public:
            /// @brief Clears the HandleVector, destroying all elements.
            /// @details Does not alter the capacity.
            inline void clear()
            {
                getTD().refresh();

                for(auto i(0u); i < size; ++i)
                {
                    SSVU_ASSERT(stats[i].alive);
                    stats[i].alive = false;

                    getTD().deinitImpl(i);
                    ++(marks[i].ctr);
                }

                size = sizeNext = 0u;
            }

            /// @brief Reserves storage, increasing the capacity.
            inline void reserve(SizeT mCapacityNew)
            {
                if(capacity < mCapacityNew) growCapacityTo(mCapacityNew);
            }

            /// @brief Creates an atom, returning an handle pointing to it.
            /// @details The created atom will not be used until the
            /// HandleVector is refreshed.
            template <typename... TArgs>
            inline auto create(TArgs&&... mArgs)
            {
                // `sizeNext` may be greater than the sizes of the vectors -
                // resize vectors if needed
                growIfNeeded();

                // `sizeNext` now is the first empty valid index - we create our
                // atom there
                getTD().createImpl(fwd<TArgs>(mArgs)...);
                stats[sizeNext].alive = true;

                // Update the mark
                getMarkFromStat(sizeNext).statIdx = sizeNext;

                // Create the handle
                Handle result{getTD(), stats[sizeNext].markIdx,
                    getMarkFromStat(sizeNext).ctr};

                // Update next size
                ++sizeNext;

                return result;
            }

            inline void
            refresh() // noexcept(noexcept(getTD().refreshDeinitImpl(mD)))
            {
                Internal::refreshImpl(size, sizeNext,
                    [this](SizeT mI)
                    {
                        return isAliveAt(mI);
                    },
                    [this](SizeT mD, SizeT mA)
                    {
                        getTD().refreshSwapImpl(mD, mA);
                        std::swap(stats[mD], stats[mA]);
                        getMarkFromStat(mD).statIdx = mD;
                    },
                    [this](SizeT mD)
                    {
                        getTD().deinitImpl(mD);
                        ++(getMarkFromStat(mD).ctr);
                    });
            }

            inline auto getCapacity() const noexcept { return capacity; }
            inline auto getSize() const noexcept { return size; }
            inline auto getSizeNext() const noexcept { return sizeNext; }

            inline auto& getStats() noexcept { return stats; }
            inline const auto& getStats() const noexcept { return stats; }

            inline auto& getMarks() noexcept { return marks; }
            inline const auto& getMarks() const noexcept { return marks; }

            inline bool isAliveAt(SizeT mI) const noexcept
            {
                return stats[mI].alive;
            }
        };
    }

    namespace Internal
    {
        template <typename T, typename TItrValue, typename TImpl>
        class HVItrSingleBase : public BaseAdaptorItrRnd<TItrValue, TImpl>
        {
        protected:
            TImpl impl;

        public:
            template <typename... TArgs>
            inline HVItrSingleBase(TItrValue mValue, TArgs&&... mArgs) noexcept
                : BaseAdaptorItrRnd<TItrValue, TImpl>{mValue},
                  impl{fwd<TArgs>(mArgs)...}
            {
            }

            inline decltype(auto) operator*() noexcept
            {
                return impl.template get<T&>(this->itr);
            }
            inline decltype(auto) operator*() const noexcept
            {
                return impl.template get<const T&>(this->itr);
            }
            inline decltype(auto) operator-> () noexcept
            {
                return &impl.template get<T&>(this->itr);
            }
            inline decltype(auto) operator-> () const noexcept
            {
                return &impl.template get<const T&>(this->itr);
            }
        };

        template <typename T>
        struct HVItrSingleImplPtr
        {
            template <typename TR>
            inline TR get(T* mPtr) const noexcept
            {
                return *mPtr;
            }
        };

        template <typename THV, typename T>
        struct HVItrSingleImplIdx
        {
            THV& hVec;
            inline HVItrSingleImplIdx(THV& mHVec) noexcept : hVec{mHVec} {}
            template <typename TR>
            inline TR get(SizeT mI) const noexcept
            {
                return hVec.getItems()[mI];
            }
        };

        template <typename THV, typename T>
        struct HVMultiItrSingleImplIdx
        {
            THV& hVec;
            inline HVMultiItrSingleImplIdx(THV& mHVec) noexcept : hVec{mHVec} {}
            template <typename TR>
            inline TR get(SizeT mI) const noexcept
            {
                return hVec.template getArrayOf<T>()[mI];
            }
        };

        template <typename T>
        using HVItrSinglePtr = HVItrSingleBase<T, T*, HVItrSingleImplPtr<T>>;
        template <typename T>
        using HVItrConstSinglePtr =
            HVItrSingleBase<const T, const T*, HVItrSingleImplPtr<const T>>;

        template <typename T>
        using HVItrSingleIdx =
            HVItrSingleBase<T, HIdx, HVItrSingleImplIdx<HVSingle<T>, T>>;
        template <typename T>
        using HVItrConstSingleIdx = HVItrSingleBase<const T, HIdx,
            HVItrSingleImplIdx<const HVSingle<T>, const T>>;

        template <typename THV, typename T>
        using HVMultiItrSingleIdx =
            HVItrSingleBase<T, HIdx, HVMultiItrSingleImplIdx<THV, T>>;
        template <typename THV, typename T>
        using HVMultiItrConstSingleIdx = HVItrSingleBase<const T, HIdx,
            HVMultiItrSingleImplIdx<const THV, const T>>;
    }

    template <typename T>
    class HVSingle : public Internal::HVImpl<HVSingle<T>, HVHandleSingle<T>>
    {
        template <typename, typename>
        friend class ssvu::Internal::HVImpl;
        template <typename>
        friend class HVHandleSingle;

    public:
        using Handle = HVHandleSingle<T>;
        using ItrSinglePtr = Internal::HVItrSinglePtr<T>;
        using ItrSingleCPtr = Internal::HVItrConstSinglePtr<T>;
        using ItrSingleIdx = Internal::HVItrSingleIdx<T>;
        using ItrSingleCIdx = Internal::HVItrConstSingleIdx<T>;

    private:
        GrowableArray<T> items;

        // CTRP implementations
        inline void deinitImpl(SizeT mI) noexcept(isNothrowDtor<T>())
        {
            items.deinitAt(mI);
        }
        inline void growImpl(SizeT mCapacityOld, SizeT mCapacityNew)
        {
            items.grow(mCapacityOld, mCapacityNew);
        }
        inline void refreshSwapImpl(SizeT mI0, SizeT mI1) noexcept
        {
            std::swap(items[mI0], items[mI1]);
        }
        template <typename... TArgs>
        inline void createImpl(TArgs&&... mArgs) noexcept(isNothrowCtor<T>())
        {
            items.initAt(this->sizeNext, fwd<TArgs>(mArgs)...);
        }
        inline auto& getItemFromMark(HIdx mMarkIdx) noexcept
        {
            return items[this->marks[mMarkIdx].statIdx];
        }

    public:
        inline HVSingle() { this->growCapacityBy(25); }
        inline ~HVSingle() { this->clear(); }

        inline HVSingle(const HVSingle&) = delete;
        inline HVSingle(HVSingle&&) = delete;

        inline auto& operator=(const HVSingle&) = delete;
        inline auto& operator=(HVSingle&&) = delete;

        inline auto& getItems() noexcept { return items; }
        inline const auto& getItems() const noexcept { return items; }

        inline auto begin() noexcept { return ItrSinglePtr{&items[0]}; }
        inline auto end() noexcept { return ItrSinglePtr{&items[this->size]}; }
        inline auto endNext() noexcept
        {
            return ItrSinglePtr{&items[this->sizeNext]};
        }

        inline auto begin() const noexcept { return ItrSingleCPtr{&items[0]}; }
        inline auto end() const noexcept
        {
            return ItrSingleCPtr{&items[this->size]};
        }
        inline auto endNext() const noexcept
        {
            return ItrSingleCPtr{&items[this->sizeNext]};
        }

        inline auto beginIdx() noexcept { return ItrSingleIdx{0, *this}; }
        inline auto endIdx() noexcept
        {
            return ItrSingleIdx{this->size, *this};
        }
        inline auto endIdxNext() noexcept
        {
            return ItrSingleIdx{this->sizeNext, *this};
        }

        inline auto beginIdx() const noexcept
        {
            return ItrSingleCIdx{0, *this};
        }
        inline auto endIdx() const noexcept
        {
            return ItrSingleCIdx{this->size, *this};
        }
        inline auto endIdxNext() const noexcept
        {
            return ItrSingleCIdx{this->sizeNext, *this};
        }

        inline auto forPtr() noexcept { return makeRange(begin(), end()); }
        inline auto forNextPtr() noexcept
        {
            return makeRange(begin(), endNext());
        }

        inline auto forPtr() const noexcept
        {
            return makeRange(begin(), end());
        }
        inline auto forNextPtr() const noexcept
        {
            return makeRange(begin(), endNext());
        }

        inline auto forIdx() noexcept
        {
            return makeRange(beginIdx(), endIdx());
        }
        inline auto forNextIdx() noexcept
        {
            return makeRange(beginIdx(), endIdxNext());
        }

        inline auto forIdx() const noexcept
        {
            return makeRange(beginIdx(), endIdx());
        }
        inline auto forNextIdx() const noexcept
        {
            return makeRange(beginIdx(), endIdxNext());
        }
    };


    template <typename... TTs>
    class HVMulti
        : public Internal::HVImpl<HVMulti<TTs...>, HVHandleMulti<TTs...>>
    {
        template <typename, typename>
        friend class ssvu::Internal::HVImpl;
        template <typename...>
        friend class HVHandleMulti;

    public:
        using Handle = HVHandleMulti<TTs...>;
        template <typename T>
        using ItrSinglePtr = Internal::HVItrSinglePtr<T>;
        template <typename T>
        using ItrSingleCPtr = Internal::HVItrConstSinglePtr<T>;
        template <typename T>
        using ItrSingleIdx = Internal::HVMultiItrSingleIdx<HVMulti<TTs...>, T>;
        template <typename T>
        using ItrSingleCIdx =
            Internal::HVMultiItrConstSingleIdx<HVMulti<TTs...>, T>;

    private:
        std::tuple<GrowableArray<TTs>...> tplArrays;

        // CTRP implementations
        inline void deinitImpl(SizeT mI) // noexcept(...)
        {
            tsFor([this, mI](auto& mA)
                {
                    mA.deinitAt(mI);
                });
        }
        inline void growImpl(SizeT mCapacityOld, SizeT mCapacityNew)
        {
            tsFor([this, mCapacityOld, mCapacityNew](auto& mA)
                {
                    mA.grow(mCapacityOld, mCapacityNew);
                });
        }
        inline void refreshSwapImpl(SizeT mI0, SizeT mI1) noexcept
        {
            tsFor([this, mI0, mI1](auto& mA)
                {
                    std::swap(mA[mI0], mA[mI1]);
                });
        }
        template <typename... TArgs>
        inline void createImpl(TArgs&&... mArgs) // noexcept(...)
        {
            // TODO: fix initialization                                  v ????
            tsFor([this, &mArgs...](auto& mA)
                {
                    mA.initAt(this->sizeNext, fwd<TArgs>(mArgs)...);
                });
        }
        template <typename T>
        inline auto& getItemFromMark(HIdx mMarkIdx) noexcept
        {
            return getArrayOf<T>()[this->marks[mMarkIdx].statIdx];
        }

        // Utilities
        template <typename TF>
        inline void tsFor(const TF& mF)
        {
            tplFor(tplArrays, mF);
        }

    public:
        inline HVMulti() { this->growCapacityBy(25); }
        inline ~HVMulti() { this->clear(); }

        inline HVMulti(const HVMulti&) = delete;
        inline HVMulti(HVMulti&&) = delete;

        inline auto& operator=(const HVMulti&) = delete;
        inline auto& operator=(HVMulti&&) = delete;

        template <typename T>
        inline auto& getArrayOf() noexcept
        {
            SSVU_ASSERT_STATIC_NM(Internal::CTHas<T, TTs...>());
            return std::get<GrowableArray<T>>(tplArrays);
        }

        template <typename T>
        inline auto beginSingle() noexcept
        {
            return ItrSinglePtr<T>(&getArrayOf<T>()[0]);
        }
        template <typename T>
        inline auto endSingle() noexcept
        {
            return ItrSinglePtr<T>(&getArrayOf<T>()[this->size]);
        }
        template <typename T>
        inline auto endNextSingle() noexcept
        {
            return ItrSinglePtr<T>(&getArrayOf<T>()[this->sizeNext]);
        }

        template <typename T>
        inline auto beginSingle() const noexcept
        {
            return ItrSingleCPtr<T>(&getArrayOf<T>()[0]);
        }
        template <typename T>
        inline auto endSingle() const noexcept
        {
            return ItrSingleCPtr<T>(&getArrayOf<T>()[this->size]);
        }
        template <typename T>
        inline auto endNextSingle() const noexcept
        {
            return ItrSingleCPtr<T>(&getArrayOf<T>()[this->sizeNext]);
        }

        template <typename T>
        inline auto beginSingleIdx() noexcept
        {
            return ItrSingleIdx<T>(0, *this);
        }
        template <typename T>
        inline auto endSingleIdx() noexcept
        {
            return ItrSingleIdx<T>(this->size, *this);
        }
        template <typename T>
        inline auto endNextSingleIdx() noexcept
        {
            return ItrSingleIdx<T>(this->sizeNext, *this);
        }

        template <typename T>
        inline auto beginSingleIdx() const noexcept
        {
            return ItrSingleCIdx<T>(0, *this);
        }
        template <typename T>
        inline auto endSingleIdx() const noexcept
        {
            return ItrSingleCIdx<T>(this->size, *this);
        }
        template <typename T>
        inline auto endNextSingleIdx() const noexcept
        {
            return ItrSingleCIdx<T>(this->sizeNext, *this);
        }
    };

    template <typename T>
    inline bool Internal::HVHandleBase<T>::isAlive() const noexcept
    {
        return hVec->marks[markIdx].ctr == ctr;
    }

    template <typename T>
    inline void Internal::HVHandleBase<T>::setDead() noexcept
    {
        hVec->setDeadFromMark(markIdx);
    }
}

struct ST0
{
    char data[10];
    ST0() { ssvu::lo("ST0 CTOR") << " \n"; }
    ~ST0() { ssvu::lo("ST0 DTOR") << " \n"; }
};

struct ST1
{
    char data[20];
    ST1() { ssvu::lo("ST1 CTOR") << " \n"; }
    ~ST1() { ssvu::lo("ST1 DTOR") << " \n"; }
};

int main()
{
    using namespace ssvu;

    {
        HVSingle<std::string> test;
        test.refresh();

        auto h0 = test.create();
        auto h1 = test.create();
        auto h2 = test.create();
        test.refresh();

        h0.get() = "h0 str";
        h1.get() = "h1 str";
        h2.get() = "h2 str";

        ssvu::lo("h0 str") << h0.get() << "\n";
        ssvu::lo("h1 str") << h1.get() << "\n";
        ssvu::lo("h2 str") << h2.get() << "\n";

        *h0 = "h0 str new";
        ssvu::lo("h0 str") << *h0 << "\n";

        h0->clear();
        ssvu::lo("h0 str") << *h0 << "\n";

        h2.setDead();

        for(auto& s : test) ssvu::lo() << s << ", " << std::endl;
        for(auto& s : makeRange(test.beginIdx(), test.endIdx()))
            ssvu::lo() << s << ", " << std::endl;

        for(const auto& s : test) ssvu::lo() << s << ", " << std::endl;
        for(const auto& s : makeRange(test.beginIdx(), test.endIdx()))
            ssvu::lo() << s << ", " << std::endl;

        const auto& ctest(test);


        for(const auto& s : ctest) ssvu::lo() << s << ", " << std::endl;
        for(const auto& s : makeRange(ctest.beginIdx(), ctest.endIdx()))
            ssvu::lo() << s << ", " << std::endl;
    }

    {
        HVSingle<UPtr<std::string>> test;
        test.refresh();

        auto h0 = test.create();
        auto h1 = test.create();
        auto h2 = test.create();
        test.refresh();

        h0.get() = makeUPtr<std::string>("h0 test");
        h1.get() = makeUPtr<std::string>("h1 test");
        h2.get() = makeUPtr<std::string>("h2 test");

        ssvu::lo("h0 str") << *(h0.get()) << "\n";
        ssvu::lo("h1 str") << *(h1.get()) << "\n";
        ssvu::lo("h2 str") << *(h2.get()) << "\n";

        **h0 = "h0 str new";
        ssvu::lo("h0 str") << **h0 << "\n";

        (*h0)->clear();
        ssvu::lo("h0 str") << **h0 << "\n";

        h2.setDead();
    }


    {
        HVMulti<ST0, ST1, std::string> test;
        test.refresh();

        auto h0 = test.create();
        auto h1 = test.create();
        auto h2 = test.create();
        test.refresh();

        h0.get<std::string>() = "h0 str";
        h1.get<std::string>() = "h1 str";
        h2.get<std::string>() = "h2 str";

        ssvu::lo("h0 str") << h0.get<std::string>() << "\n";
        ssvu::lo("h1 str") << h1.get<std::string>() << "\n";
        ssvu::lo("h2 str") << h2.get<std::string>() << "\n";

        std::get<2>(*h0) = "h0 str new";
        ssvu::lo("h0 str") << std::get<2>(*h0) << "\n";

        for(auto x : makeRange(
                test.beginSingle<std::string>(), test.endSingle<std::string>()))
            ssvu::lo("aaa") << x << "\n";

        for(auto x : makeRange(test.beginSingleIdx<std::string>(),
                test.endSingleIdx<std::string>()))
            ssvu::lo("aaa") << x << "\n";
    }


    // h1.destroy();
    // test.refresh();

    // ssvu::lo("h0 str") << h0.get<std::string>() << "\n";
    // ssvu::lo("h1 str") << h1.get<std::string>() << "\n";
    // ssvu::lo("h2 str") << h2.get<std::string>() << "\n";

    return 0;
}