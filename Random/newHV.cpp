#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/Range/Range.hpp"
#include "SSVUtils/GrowableArray/GrowableArray.hpp"
#include "SSVUtils/HandleVector/Internal/Uncertain.hpp"
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
	    inline void tplForImpl(T&& mT, TF mF, std::index_sequence<TIs...>)
	    {
	        auto l{(mF(std::get<TIs>(mT)), 0)...};
	    }

	    template<typename... TTs, typename TF>
		inline void tplFor(std::tuple<TTs...>& mT, TF mF)
		{
		    tplForImpl(mT, mF, std::index_sequence_for<TTs...>{});
		}
	}

	/// @brief Handle class that points to HandleVector elements.
	template<typename... TTs> class HVHandle
	{
		template<typename...> friend class ssvu::HV2;
	
		private:
			/// @brief Internal pointer to the HandleVector.
			HV2<TTs...>* hVec;

			/// @brief Index of the mark to check.
			HIdx markIdx;

			/// @brief Counter of the handle. Will be compared to the mark's counter.
			HCtr ctr;

			inline HVHandle(HV2<TTs...>& mHVec, HIdx mMarkIdx, HCtr mCtr) noexcept 
				: hVec(&mHVec), markIdx{mMarkIdx}, ctr{mCtr} { }

			/// @brief Internal implementation method that returns a reference or a const reference to the atom.
			template<typename T, typename TR> inline TR getImpl() noexcept
			{
				SSVU_ASSERT(isAlive());				
				return hVec->template getFromMark<T>(hVec->marks[markIdx]);
			}

		public:
			/// @brief Returns a reference to the data. Assumes the handle is valid.
			template<typename T> inline T& get() noexcept { return getImpl<T, T&>(); }

			/// @brief Returns a const reference to the data. Assumes the handle is valid.
			template<typename T> inline const T& get() const noexcept { return getImpl<T, const T&>(); }

			/// @brief Returns whether the handle is valid or not.
			/// @details The handle is considered valid only when it points to the atom it originally pointed to.
			bool isAlive() const noexcept;

			/// @brief Sets the pointed atom's status as dead.
			void destroy() noexcept;

			// Pointer-like interface
			/*inline T& operator*() noexcept			{ return get(); }
			inline const T& operator*() const noexcept	{ return get(); }
			inline T* operator->() noexcept				{ return &(get()); }
			inline const T* operator->() const noexcept	{ return &(get()); }*/
	};


	template<typename... TTs> class HV2
	{
		public:
			using Stat = Internal::HVStat;
			using Mark = Internal::HVMark;	
			using Handle = HVHandle<TTs...>;		

		public:
			SizeT capacity{0u};
			SizeT size{0u};
			SizeT sizeNext{0u};
			
			// These move togheter
			GrowableArray<Stat> statuses;
			std::tuple<GrowableArray<TTs>...> tplArrays;
			
			// This is separated
			GrowableArray<Mark> marks;

			template<typename T> inline auto& getArrayOf() noexcept
			{
				SSVU_ASSERT_STATIC_NM(Internal::CTHas<T, TTs...>());
				return std::get<GrowableArray<T>>(tplArrays); 
			}

			template<typename T> inline auto& getFromMark(const Mark& mMark) noexcept 
			{ 
				return getArrayOf<T>()[mMark.statIdx]; 
			}

			inline auto& getMark(const Stat& mStat)	noexcept 
			{
				return marks[mStat.markIdx]; 
			}

			inline void destroy(HIdx mMarkIdx) noexcept
			{
				statuses[marks[mMarkIdx].statIdx].alive = false;
			}

			/// @brief Increases internal storage capacity by mAmount.
			inline void growCapacityBy(SizeT mAmount)
			{
				auto capacityNew(capacity + mAmount);
				SSVU_ASSERT(capacityNew >= 0 && capacityNew >= capacity);

				Internal::tplFor(tplArrays, [this, capacityNew](auto& mA)
				{ 
					mA.grow(capacity, capacityNew); 
				});				

				statuses.grow(capacity, capacityNew);					
				marks.grow(capacity, capacityNew);

				// Initialize resized storage
				for(; capacity < capacityNew; ++capacity)
				{
					statuses.initAt(capacity, capacity);					
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

				if(capacity <= sizeNext) growCapacityTo((capacity + growAmount) * growMultiplier);
			}
		
			inline bool isAliveAt(SizeT mI) const noexcept	{ return statuses[mI].alive; }
			inline bool isDeadAt(SizeT mI) const noexcept	{ return !statuses[mI].alive; }

		public:
			inline HV2() { growCapacityBy(10); }
			inline ~HV2() { clear(); }

			inline HV2(const HV2&) = delete;
			inline HV2(HV2&&) = delete;

			inline auto& operator=(const HV2&) = delete;
			inline auto& operator=(HV2&&) = delete;

			/// @brief Clears the HandleVector, destroying all elements.
			/// @details Does not alter the capacity.
			inline void clear()
			{
				refresh();

				for(auto i(0u); i < size; ++i)
				{
					SSVU_ASSERT(statuses[i].alive);
					statuses[i].alive = false;

					Internal::tplFor(tplArrays, [this, i](auto& mA)
					{ 
						mA.deinitAt(i); 
					});		
					
					++(marks[i].ctr);
				}

				size = sizeNext = 0u;
			}

			/// @brief Reserves storage, increasing the capacity.
			inline void reserve(SizeT mCapacityNew) { if(capacity < mCapacityNew) growCapacityTo(mCapacityNew); }

			/// @brief Creates an atom, returning an handle pointing to it.
			/// @details The created atom will not be used until the HandleVector is refreshed.
			template<typename... TArgs> inline Handle create()
			{
				// `sizeNext` may be greater than the sizes of the vectors - resize vectors if needed
				growIfNeeded();

				// `sizeNext` now is the first empty valid index - we create our atom there
				Internal::tplFor(tplArrays, [this](auto& mA)
				{ 
					//mA.initAt(sizeNext, fwd<TArgs>(mArgs)...);
					mA.initAt(sizeNext);
				});		
				statuses[sizeNext].alive = true;

				// Update the mark				
				getMark(statuses[sizeNext]).statIdx = sizeNext;

				Handle result{*this, statuses[sizeNext].markIdx, getMark(statuses[sizeNext]).ctr};
				
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
					
					Internal::tplFor(tplArrays, [this, iD, iA](auto& mA)
					{ 
						std::swap(mA[iD], mA[iA]);						
					});				
					std::swap(statuses[iD], statuses[iA]);			
					getMark(statuses[iD]).statIdx = iD;

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
					
					Internal::tplFor(tplArrays, [this, iD](auto& mA)
					{ 
						mA.deinitAt(iD); 
					});		

					++(getMark(statuses[iD]).ctr);
				}
			}
	};

	template<typename... TTs> inline bool HVHandle<TTs...>::isAlive() const noexcept
	{
		return hVec->marks[markIdx].ctr == ctr;
	}

	template<typename... TTs> inline void HVHandle<TTs...>::destroy() noexcept
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

	h1.destroy();
	test.refresh();

	ssvu::lo("h0 str") << h0.get<std::string>() << "\n"; 
	ssvu::lo("h1 str") << h1.get<std::string>() << "\n";
	ssvu::lo("h2 str") << h2.get<std::string>() << "\n";

	return 0;
}