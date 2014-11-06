#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/Range/Range.hpp"
#include "SSVUtils/GrowableArray/GrowableArray.hpp"
//#include "SSVUtils/HandleVector/Internal/Common.hpp"
#include "SSVUtils/UnionVariant/Internal/CTUtils.hpp"

namespace ssvu
{
	using HIdx = SizeT;	
	using HCtr = int;

	template<typename...> class HV2;

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
 		
    	template<typename T, typename TF, SizeT... TIs>
	    inline void tplForImpl(T&& mT, const TF& mF, IdxSeq<TIs...>)
	    {
	        auto l{(mF(std::get<TIs>(mT)), 0)...};
	    }

	    template<typename... TTs, typename TF>
		inline void tplFor(std::tuple<TTs...>& mT, const TF& mF)
		{
		    tplForImpl(mT, mF, IdxSeqFor<TTs...>{});
		}
	}

	template<typename T> class HandleBase
	{
		template<typename...> friend class ssvu::HV2;

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
	template<typename... TTs> class HVHandle : public HandleBase<HV2<TTs...>>
	{
		template<typename...> friend class ssvu::HV2;
	
		private:		
			inline HVHandle(HV2<TTs...>& mHVec, HIdx mMarkIdx, HCtr mCtr) noexcept : HandleBase<HV2<TTs...>>{mHVec, mMarkIdx, mCtr} { }

			/// @brief Internal implementation method that returns a reference or a const reference to the atom.
			template<typename T, typename TR> inline TR getImpl() noexcept
			{
				SSVU_ASSERT(this->isAlive());				
				return this->hVec->template getItemFromMark<T>(this->markIdx);
			}

		public:
			/// @brief Returns a reference to the data. Assumes the handle is valid.
			template<typename T> inline T& get() noexcept { return getImpl<T, T&>(); }

			/// @brief Returns a const reference to the data. Assumes the handle is valid.
			template<typename T> inline const T& get() const noexcept { return getImpl<T, const T&>(); }

			

			// Pointer-like interface
			/*inline T& operator*() noexcept			{ return get(); }
			inline const T& operator*() const noexcept	{ return get(); }
			inline T* operator->() noexcept				{ return &(get()); }
			inline const T* operator->() const noexcept	{ return &(get()); }*/
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
	};

	template<typename TDerived> class HVImpl : public TDerived
	{
		public:

		private:		
			inline auto& getTD() noexcept { return castUp<TDerived>(*this); }

			/// @brief Increases internal storage capacity by mAmount.
			inline void growCapacityBy(SizeT mAmount)
			{
				auto capacityNew(this->capacity + mAmount);
				SSVU_ASSERT(capacityNew >= 0 && capacityNew >= capacity);

				getTD().growImpl(this->capacity, capacityNew);
				this->stats.grow(this->capacity, capacityNew);					
				this->marks.grow(this->capacity, capacityNew);

				// Initialize resized storage
				for(; this->capacity < capacityNew; ++this->capacity)
				{
					this->stats.initAt(this->capacity, this->capacity);					
					this->marks.initAt(this->capacity, this->capacity);
				}
			}

			/// @brief Sets internal storage capacity to mCapacity.
			inline void growCapacityTo(SizeT mCapacityNew)
			{
				SSVU_ASSERT(this->capacity < mCapacityNew);
				growCapacityBy(mCapacityNew - this->capacity);
			}

			/// @brief Checks if the current capacity is enough - if it isn't, increases it.
			inline void growIfNeeded()
			{
				constexpr float growMultiplier{2.f};
				constexpr SizeT growAmount{5};

				if(this->capacity <= this->sizeNext) 
					growCapacityTo((this->capacity + growAmount) * growMultiplier);
			}

			inline bool isAliveAt(SizeT mI) const noexcept	{ return this->stats[mI].alive; }
			inline bool isDeadAt(SizeT mI) const noexcept	{ return !this->stats[mI].alive; }

			inline auto& getMarkFromStat(HIdx mStatIdx) noexcept { return this->marks[this->stats[mStatIdx].markIdx]; }
			inline auto& getStatFromMark(HIdx mMarkIdx) noexcept { return this->stats[this->marks[mMarkIdx].statIdx]; }

			inline void destroy(HIdx mMarkIdx) noexcept
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
					SSVU_ASSERT(this->stats[i].alive);
					this->stats[i].alive = false;

					getTD().clearImpl(i);
					++(this->marks[i].ctr);
				}

				this->size = this->sizeNext = 0u;
			}

			/// @brief Reserves storage, increasing the capacity.
			inline void reserve(SizeT mCapacityNew) 
			{ 
				if(capacity < mCapacityNew) growCapacityTo(mCapacityNew); 
			}


	};


	template<typename... TTs> class HV2 : public HVBase
	{
		public:
			using Stat = Internal::HVStat;
			using Mark = Internal::HVMark;	
			using Handle = HVHandle<TTs...>;		

		public:			
			std::tuple<GrowableArray<TTs>...> tplArrays;

			template<typename TF> inline void tplFor(const TF& mF) { Internal::tplFor(tplArrays, mF); }

			template<typename T> inline auto& getArrayOf() noexcept
			{
				SSVU_ASSERT_STATIC_NM(Internal::CTHas<T, TTs...>());
				return std::get<GrowableArray<T>>(tplArrays); 
			}

			template<typename T> inline auto& getItemFromMark(HIdx mMarkIdx) noexcept 
			{ 
				return getArrayOf<T>()[marks[mMarkIdx].statIdx]; 
			}

			/// @brief Increases internal storage capacity by mAmount.
			inline void growImpl(SizeT mCapacityOld, SizeT mCapacityNew)
			{
				tplFor([this, mCapacityOld, mCapacityNew](auto& mA){ mA.grow(mCapacityOld, mCapacityNew); });			
			}

			inline void swapItemsAndStat(HIdx mI0, HIdx mI1) noexcept
			{
				tplFor([this, mI0, mI1](auto& mA){ std::swap(mA[mI0], mA[mI1]); });				
				std::swap(stats[mI0], stats[mI1]);			
			}

		public:
			inline HV2() { growCapacityBy(10); }
			inline ~HV2() { clear(); }

			inline HV2(const HV2&) = delete;
			inline HV2(HV2&&) = delete;

			inline auto& operator=(const HV2&) = delete;
			inline auto& operator=(HV2&&) = delete;

			
			inline void clearImpl(SizeT mI)
			{
				tplFor([this, mI](auto& mA){ mA.deinitAt(mI); });					
			}

			

			/// @brief Creates an atom, returning an handle pointing to it.
			/// @details The created atom will not be used until the HandleVector is refreshed.
			template<typename... TArgs> inline Handle create()
			{
				// `sizeNext` may be greater than the sizes of the vectors - resize vectors if needed
				growIfNeeded();

				// `sizeNext` now is the first empty valid index - we create our atom there
				tplFor([this](auto& mA)
				{ 
					//mA.initAt(sizeNext, fwd<TArgs>(mArgs)...);
					mA.initAt(sizeNext);
				});		
				stats[sizeNext].alive = true;

				// Update the mark				
				getMarkFromStat(sizeNext).statIdx = sizeNext;

				Handle result{*this, stats[sizeNext].markIdx, getMarkFromStat(sizeNext).ctr};
				
				// Update next size
				++sizeNext;

				return result;
			}

			inline void refresh()
			{
				const int intSizeNext(sizeNext);
				int iD{0}, iA{intSizeNext - 1};

				do
				{
					// Find dead item from left
					for(; true; ++iD)
					{
						// No more dead items
						if(iD > iA) goto finishRefresh;
						if(isDeadAt(iD)) break;
					}

					// Find alive item from right
					for(; true; --iA)
					{
						// No more alive items
						if(iA <= iD) goto finishRefresh;
						if(isAliveAt(iA)) break;
					}

					SSVU_ASSERT(isDeadAt(iD) && isAliveAt(iA));
					
					swapItemsAndStat(iD, iA);					
					getMarkFromStat(iD).statIdx = iD;

					SSVU_ASSERT(isAliveAt(iD) && isDeadAt(iA));

					// Move both iterators
					++iD; --iA;
				}
				while(true);

				finishRefresh:

				#if SSVU_DEBUG
					for(iA = iA - 1; iA >= 0; --iA) SSVU_ASSERT(isAliveAt(iA));
				#endif

				size = sizeNext = iD;

				for(; iD < intSizeNext; ++iD)
				{
					SSVU_ASSERT(isDeadAt(iD));
					
					tplFor([this, iD](auto& mA){ mA.deinitAt(iD); });
					++getMarkFromStat(iD).ctr;
				}
			}
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

	using HVType = HVImpl<HV2<ST0, ST1, std::string>>;

	HVType test;
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

	h1.destroy();
	test.refresh();

	ssvu::lo("h0 str") << h0.get<std::string>() << "\n"; 
	ssvu::lo("h1 str") << h1.get<std::string>() << "\n";
	ssvu::lo("h2 str") << h2.get<std::string>() << "\n";

	return 0;
}