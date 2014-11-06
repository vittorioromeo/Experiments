#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/Range/Range.hpp"
#include "SSVUtils/GrowableArray/GrowableArray.hpp"
//#include "SSVUtils/HandleVector/Internal/Common.hpp"
#include "SSVUtils/UnionVariant/Internal/CTUtils.hpp"
#include "SSVUtils/Internal/SharedFuncs.hpp"

namespace ssvu
{
	using HIdx = SizeT;
	using HCtr = int;

	template<typename, typename> class HVImpl;
	template<typename> class HV1;
	template<typename...> class HV2;

	namespace Internal
	{
		template<typename... TArgs> inline auto makeInitializerList(TArgs&&... mArgs) noexcept
		{
			auto il{fwd<TArgs>(mArgs)...};
			return il;
		}

		template<typename T, typename TF, SizeT... TIs>
		inline void tplForImpl(T&& mT, const TF& mF, IdxSeq<TIs...>) 
		{
			makeInitializerList((mF(std::get<TIs>(mT)), 0)...);
		}

		template<typename... TTs, typename TF>
		inline void tplFor(std::tuple<TTs...>& mT, const TF& mF)
		{
			tplForImpl(mT, mF, IdxSeqFor<TTs...>{});
		}
	}

	namespace Internal
	{
		struct HVMark
		{
			HIdx statIdx;
			HCtr ctr;

			inline HVMark(HIdx mStatIdx) noexcept : statIdx{mStatIdx} { }
		};

		struct HVStat
		{
			HIdx markIdx;
			bool alive{false};

			inline HVStat(HIdx mMarkIdx) noexcept : markIdx{mMarkIdx} { }
		};
	}

	template<typename T> class HandleBase
	{
		protected:
			/// @brief Internal pointer to the HandleVector.
			T* hVec;

			/// @brief Index of the mark to check.
			HIdx markIdx;

			/// @brief Counter of the handle. Will be compared to the mark's counter.
			HCtr ctr;

		public:
			inline HandleBase(T& mHVec, HIdx mMarkIdx, HCtr mCtr) noexcept : hVec{&mHVec}, markIdx{mMarkIdx}, ctr{mCtr} { }

			/// @brief Returns whether the handle is valid or not.
			/// @details The handle is considered valid only when it points to the atom it originally pointed to.
			bool isAlive() const noexcept;

			/// @brief Sets the pointed stat's status as dead.
			void destroy() noexcept;
	};

	/// @brief Handle class that points to HandleVector elements.
	template<typename T> class HVHandle1 : public HandleBase<HV1<T>>
	{
		template<typename, typename> friend class ssvu::HVImpl;

		private:
			inline HVHandle1(HV1<T>& mHVec, HIdx mMarkIdx, HCtr mCtr) noexcept : HandleBase<HV1<T>>{mHVec, mMarkIdx, mCtr} { }

			/// @brief Internal implementation method that returns a reference or a const reference to the atom.
			template<typename TR> inline TR getImpl() noexcept
			{
				SSVU_ASSERT(this->isAlive());
				return this->hVec->getItemFromMark(this->markIdx);
			}

		public:
			/// @brief Returns a reference to the data. Assumes the handle is valid.
			inline T& get() noexcept { return getImpl<T&>(); }

			/// @brief Returns a const reference to the data. Assumes the handle is valid.
			inline const T& get() const noexcept { return getImpl<const T&>(); }

			// Pointer-like interface
			inline T& operator*() noexcept				{ return get(); }
			inline const T& operator*() const noexcept	{ return get(); }
			inline T* operator->() noexcept				{ return &(get()); }
			inline const T* operator->() const noexcept	{ return &(get()); }
	};

	/// @brief Handle class that points to HandleVector elements.
	template<typename... TTs> class HVHandle2 : public HandleBase<HV2<TTs...>>
	{
		template<typename, typename> friend class ssvu::HVImpl;

		private:
			inline HVHandle2(HV2<TTs...>& mHVec, HIdx mMarkIdx, HCtr mCtr) noexcept : HandleBase<HV2<TTs...>>{mHVec, mMarkIdx, mCtr} { }

			/// @brief Internal implementation method that returns a reference or a const reference to the atom.
			template<typename T, typename TR> inline TR getImpl() noexcept
			{
				SSVU_ASSERT(this->isAlive());
				return this->hVec->template getItemFromMark<T>(this->markIdx);
			}

		public:
			inline auto getRefTpl() noexcept { return std::tuple<TTs&...>{getImpl<TTs, TTs&>()...}; }
			inline auto getRefTpl() const noexcept { return std::tuple<const TTs&...>{getImpl<TTs, const TTs&>()...}; }

			/// @brief Returns a reference to the data. Assumes the handle is valid.
			template<typename T> inline T& get() noexcept { return getImpl<T, T&>(); }

			/// @brief Returns a const reference to the data. Assumes the handle is valid.
			template<typename T> inline const T& get() const noexcept { return getImpl<T, const T&>(); }

			// Pointer-like interface
			inline auto operator*() noexcept				{ return getRefTpl(); }
			inline const auto operator*() const noexcept	{ return getRefTpl(); }
	};

	class HVBase
	{
		public:
			using Stat = Internal::HVStat;
			using Mark = Internal::HVMark;

		protected:
			SizeT capacity{0u};
			SizeT size{0u};
			SizeT sizeNext{0u};

			GrowableArray<Stat> stats;
			GrowableArray<Mark> marks;

		public:
			inline auto getCapacity() const noexcept	{ return capacity; }
			inline auto getSize() const noexcept		{ return size; }
			inline auto getSizeNext() const noexcept	{ return sizeNext; }
	};

	template<typename TDerived, typename THandle> class HVImpl : public HVBase
	{
		template<typename> friend class HandleBase;

		private:
			inline auto& getTD() noexcept { return castUp<TDerived>(*this); }

		public:
			using Handle = THandle;		

		protected:
			/// @brief Increases internal storage capacity by mAmount.
			inline void growCapacityBy(SizeT mAmount)
			{
				auto capacityNew(capacity + mAmount);
				SSVU_ASSERT(capacityNew >= 0 && capacityNew >= capacity);

				getTD().growImpl(capacity, capacityNew);
				stats.grow(capacity, capacityNew);
				marks.grow(capacity, capacityNew);

				// Initialize resized storage
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

			/// @brief Checks if the current capacity is enough - if it isn't, increases it.
			inline void growIfNeeded()
			{
				constexpr float growMultiplier{2.f};
				constexpr SizeT growAmount{5};

				if(capacity <= sizeNext)
					growCapacityTo((capacity + growAmount) * growMultiplier);
			}

			inline bool isAliveAt(SizeT mI) const noexcept	{ return stats[mI].alive; }
			inline bool isDeadAt(SizeT mI) const noexcept	{ return !stats[mI].alive; }

			inline auto& getMarkFromStat(HIdx mStatIdx) noexcept { return marks[stats[mStatIdx].markIdx]; }
			inline auto& getStatFromMark(HIdx mMarkIdx) noexcept { return stats[marks[mMarkIdx].statIdx]; }

			inline void destroyFromMark(HIdx mMarkIdx) noexcept
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

					getTD().clearImpl(i);
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
			/// @details The created atom will not be used until the HandleVector is refreshed.
			template<typename... TArgs> inline THandle create(TArgs&&... mArgs)
			{
				// `sizeNext` may be greater than the sizes of the vectors - resize vectors if needed
				growIfNeeded();

				// `sizeNext` now is the first empty valid index - we create our atom there
				getTD().createImpl(fwd<TArgs>(mArgs)...);
				stats[sizeNext].alive = true;

				// Update the mark
				getMarkFromStat(sizeNext).statIdx = sizeNext;

				Handle result{getTD(), stats[sizeNext].markIdx, getMarkFromStat(sizeNext).ctr};

				// Update next size
				++sizeNext;

				return result;
			}

			inline void refresh()
			{
				Internal::refreshImpl(size, sizeNext,
					[this](SizeT mI){ return isAliveAt(mI); },
					[this](SizeT mD, SizeT mA)
					{
						getTD().refreshSwapImpl(mD, mA);
						std::swap(stats[mD], stats[mA]);
						getMarkFromStat(mD).statIdx = mD;
					},
					[this](SizeT mD)
					{
						getTD().refreshDeinitImpl(mD);
						++(getMarkFromStat(mD).ctr);
					});
			}
	};

	template<typename T> class HV1 : public HVImpl<HV1<T>, HVHandle1<T>>
	{
		template<typename, typename> friend class HVImpl;
		template<typename> friend class HVHandle1;

		public:
			using Handle = HVHandle1<T>;

		private:
			GrowableArray<T> items;

			inline void clearImpl(SizeT mI) { items.deinitAt(mI); }
			inline void growImpl(SizeT mCapacityOld, SizeT mCapacityNew) { items.grow(mCapacityOld, mCapacityNew);  }
			inline void refreshSwapImpl(SizeT mI0, SizeT mI1) { std::swap(items[mI0], items[mI1]); }
			inline void refreshDeinitImpl(SizeT mI) { items.deinitAt(mI); }

			template<typename... TArgs> inline void createImpl(TArgs&&... mArgs) 
			{ 
				items.initAt(this->sizeNext, fwd<TArgs>(mArgs)...); 
			}

			inline auto& getItemFromMark(HIdx mMarkIdx) noexcept
			{
				return items[this->marks[mMarkIdx].statIdx];
			}
		
		public:
			inline HV1() { this->growCapacityBy(10); }
			inline ~HV1() { this->clear(); }

			inline HV1(const HV1&) = delete;
			inline HV1(HV1&&) = delete;

			inline auto& operator=(const HV1&) = delete;
			inline auto& operator=(HV1&&) = delete;		
	};


	template<typename... TTs> class HV2 : public HVImpl<HV2<TTs...>, HVHandle2<TTs...>>
	{
		template<typename, typename> friend class HVImpl;
		template<typename...> friend class HVHandle2;

		public:
			using Handle = HVHandle2<TTs...>;

		private:
			std::tuple<GrowableArray<TTs>...> tplArrays;

			inline void clearImpl(SizeT mI)
			{
				tplFor([this, mI](auto& mA){ mA.deinitAt(mI); });
			}

			inline void growImpl(SizeT mCapacityOld, SizeT mCapacityNew)
			{
				tplFor([this, mCapacityOld, mCapacityNew](auto& mA){ mA.grow(mCapacityOld, mCapacityNew); });
			}

			template<typename... TArgs> inline void createImpl(TArgs&&... mArgs)
			{
				//mA.initAt(sizeNext, fwd<TArgs>(mArgs)...);
				tplFor([this](auto& mA){ mA.initAt(this->sizeNext); });
			}

			inline void refreshSwapImpl(SizeT mI0, SizeT mI1)
			{
				tplFor([this, mI0, mI1](auto& mA){ std::swap(mA[mI0], mA[mI1]); });
			}

			inline void refreshDeinitImpl(SizeT mI)
			{
				tplFor([this, mI](auto& mA){ mA.deinitAt(mI); });
			}

			template<typename TF> inline void tplFor(const TF& mF) { Internal::tplFor(tplArrays, mF); }

			template<typename T> inline auto& getArrayOf() noexcept
			{
				SSVU_ASSERT_STATIC_NM(Internal::CTHas<T, TTs...>());
				return std::get<GrowableArray<T>>(tplArrays);
			}

			template<typename T> inline auto& getItemFromMark(HIdx mMarkIdx) noexcept
			{
				return getArrayOf<T>()[this->marks[mMarkIdx].statIdx];
			}
		
		public:
			inline HV2() { this->growCapacityBy(10); }
			inline ~HV2() { this->clear(); }

			inline HV2(const HV2&) = delete;
			inline HV2(HV2&&) = delete;

			inline auto& operator=(const HV2&) = delete;
			inline auto& operator=(HV2&&) = delete;		
	};

	template<typename T> inline bool HandleBase<T>::isAlive() const noexcept
	{
		return hVec->marks[markIdx].ctr == ctr;
	}

	template<typename T> inline void HandleBase<T>::destroy() noexcept
	{
		hVec->destroy(markIdx);
	}
}

struct ST0
{
	char data[10];
	ST0() 	{ ssvu::lo("ST0 CTOR") << " \n"; }
	~ST0() 	{ ssvu::lo("ST0 DTOR") << " \n"; }
};

struct ST1
{
	char data[20];
	ST1() 	{ ssvu::lo("ST1 CTOR") << " \n"; }
	~ST1() 	{ ssvu::lo("ST1 DTOR") << " \n"; }
};

int main()
{
	using namespace ssvu;

	{
		HV1<std::string> test;
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
	}


	{
		HV2<ST0, ST1, std::string> test;
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
	}


	//h1.destroy();
	//test.refresh();

	//ssvu::lo("h0 str") << h0.get<std::string>() << "\n";
	//ssvu::lo("h1 str") << h1.get<std::string>() << "\n";
	//ssvu::lo("h2 str") << h2.get<std::string>() << "\n";

	return 0;
}