// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_NEWHV_HVSINGLE
#define SSVU_NEWHV_HVSINGLE

namespace ssvu
{
	template<typename T> class HVSingle : public Internal::HVImpl<HVSingle<T>, HVHandleSingle<T>>
	{
		template<typename, typename> friend class ssvu::Internal::HVImpl;
		template<typename> friend class HVHandleSingle;

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
			template<typename... TArgs> inline void createImpl(TArgs&&... mArgs) noexcept(isNothrowCtor<T>())
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

			inline auto& getItems() noexcept 				{ return items; }
			inline const auto& getItems() const noexcept	{ return items; }

			inline auto begin() noexcept			{ return ItrSinglePtr{&items[0]}; }
			inline auto end() noexcept				{ return ItrSinglePtr{&items[this->size]}; }
			inline auto endNext() noexcept			{ return ItrSinglePtr{&items[this->sizeNext]}; }

			inline auto begin() const noexcept		{ return ItrSingleCPtr{&items[0]}; }
			inline auto end() const noexcept		{ return ItrSingleCPtr{&items[this->size]}; }
			inline auto endNext() const noexcept	{ return ItrSingleCPtr{&items[this->sizeNext]}; }

			inline auto beginIdx() noexcept			{ return ItrSingleIdx{0, *this}; }
			inline auto endIdx() noexcept			{ return ItrSingleIdx{this->size, *this}; }
			inline auto endIdxNext() noexcept		{ return ItrSingleIdx{this->sizeNext, *this}; }

			inline auto beginIdx() const noexcept	{ return ItrSingleCIdx{0, *this}; }
			inline auto endIdx() const noexcept		{ return ItrSingleCIdx{this->size, *this}; }
			inline auto endIdxNext() const noexcept	{ return ItrSingleCIdx{this->sizeNext, *this}; }

			inline auto forPtr() noexcept			{ return makeRange(begin(), end()); }
			inline auto forNextPtr() noexcept		{ return makeRange(begin(), endNext()); }

			inline auto forPtr() const noexcept		{ return makeRange(begin(), end()); }
			inline auto forNextPtr() const noexcept	{ return makeRange(begin(), endNext()); }

			inline auto forIdx() noexcept			{ return makeRange(beginIdx(), endIdx()); }
			inline auto forNextIdx() noexcept		{ return makeRange(beginIdx(), endIdxNext()); }
			
			inline auto forIdx() const noexcept		{ return makeRange(beginIdx(), endIdx()); }
			inline auto forNextIdx() const noexcept	{ return makeRange(beginIdx(), endIdxNext()); }
	};
}

#endif