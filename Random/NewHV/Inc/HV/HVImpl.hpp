// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_NEWHV_HVIMPL
#define SSVU_NEWHV_HVIMPL

namespace ssvu
{
	namespace Internal
	{
		/// @brief Base CTRP HandleVector class.
		template<typename TDerived, typename THandle> class HVImpl
		{
			template<typename> friend class ssvu::Internal::HVHandleBase;

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

					// Initialize resized storage and set `capacity` to `capacityNew`.
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

					if(SSVU_LIKELY(capacity > sizeNext)) return;
					growCapacityTo((capacity + growAmount) * growMultiplier);
				}

				inline auto& getMarkFromStat(HIdx mStatIdx) noexcept { return marks[stats[mStatIdx].markIdx]; }
				inline auto& getStatFromMark(HIdx mMarkIdx) noexcept { return stats[marks[mMarkIdx].statIdx]; }

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
				/// @details The created atom will not be used until the HandleVector is refreshed.
				template<typename... TArgs> inline auto create(TArgs&&... mArgs)
				{
					// `sizeNext` may be greater than the sizes of the vectors - resize vectors if needed
					growIfNeeded();

					// `sizeNext` now is the first empty valid index - we create our atom there
					getTD().createImpl(fwd<TArgs>(mArgs)...);
					stats[sizeNext].alive = true;

					// Update the mark
					getMarkFromStat(sizeNext).statIdx = sizeNext;

					// Create the handle
					Handle result{getTD(), stats[sizeNext].markIdx, getMarkFromStat(sizeNext).ctr};

					// Update next size
					++sizeNext;

					return result;
				}

				inline void refresh() // noexcept(noexcept(getTD().refreshDeinitImpl(mD)))
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
							getTD().deinitImpl(mD);
							++(getMarkFromStat(mD).ctr);
						});
				}

				inline auto getCapacity() const noexcept	{ return capacity; }
				inline auto getSize() const noexcept		{ return size; }
				inline auto getSizeNext() const noexcept	{ return sizeNext; }

				inline auto& getStats() noexcept				{ return stats; }
				inline const auto& getStats() const noexcept	{ return stats; }

				inline auto& getMarks() noexcept				{ return marks; }
				inline const auto& getMarks() const noexcept	{ return marks; }

				inline bool isAliveAt(SizeT mI) const noexcept	{ return stats[mI].alive; }
		};
	}
}

#endif