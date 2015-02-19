// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_NEWHV_HVMULTI
#define SSVU_NEWHV_HVMULTI

namespace ssvu
{
	template<typename... TTs> class HVMulti : public Internal::HVImpl<HVMulti<TTs...>, HVHandleMulti<TTs...>>
	{
		template<typename, typename> friend class ssvu::Internal::HVImpl;
		template<typename...> friend class HVHandleMulti;

		public:
			using Handle = HVHandleMulti<TTs...>;
			template<typename T> using ItrSinglePtr = Internal::HVItrSinglePtr<T>;
			template<typename T> using ItrSingleCPtr = Internal::HVItrConstSinglePtr<T>;
			template<typename T> using ItrSingleIdx = Internal::HVMultiItrSingleIdx<HVMulti<TTs...>, T>;
			template<typename T> using ItrSingleCIdx = Internal::HVMultiItrConstSingleIdx<HVMulti<TTs...>, T>;

		private:
			std::tuple<GrowableArray<TTs>...> tplArrays;

			// CTRP implementations
			inline void deinitImpl(SizeT mI) // noexcept(...)
			{
				tsFor([this, mI](auto& mA){ mA.deinitAt(mI); });
			}
			inline void growImpl(SizeT mCapacityOld, SizeT mCapacityNew)
			{
				tsFor([this, mCapacityOld, mCapacityNew](auto& mA){ mA.grow(mCapacityOld, mCapacityNew); });
			}
			inline void refreshSwapImpl(SizeT mI0, SizeT mI1) noexcept
			{
				tsFor([this, mI0, mI1](auto& mA){ std::swap(mA[mI0], mA[mI1]); });
			}
			template<typename... TArgs> inline void createImpl(TArgs&&... mArgs) // noexcept(...)
			{
				// TODO: tuple_emplace with N parameter packs for N types 
				// TODO: fix initialization                                  v ????
				tsFor([this, &mArgs...](auto& mA){ mA.initAt(this->sizeNext, fwd<TArgs>(mArgs)...); });

				// TODO: instead of passing margs, pass a single tuple argument that contains N tuples containing the 
				// parameters used to initialize every single type
				// basically, a tuple of tuples 
			}
			template<typename T> inline auto& getItemFromMark(HIdx mMarkIdx) noexcept
			{
				return getArrayOf<T>()[this->marks[mMarkIdx].statIdx];
			}

			// Utilities
			template<typename TF> inline void tsFor(const TF& mF) { tplFor(mF, tplArrays); }

		public:
			inline HVMulti() { this->growCapacityBy(25); }
			inline ~HVMulti() { this->clear(); }

			inline HVMulti(const HVMulti&) = delete;
			inline HVMulti(HVMulti&&) = delete;

			inline auto& operator=(const HVMulti&) = delete;
			inline auto& operator=(HVMulti&&) = delete;

			template<typename T> inline auto& getArrayOf() noexcept
			{
				SSVU_ASSERT_STATIC_NM(Internal::CTHas<T, TTs...>());
				return std::get<GrowableArray<T>>(tplArrays);
			}

			template<typename T> inline auto beginSingle() noexcept 			{ return ItrSinglePtr<T>(&getArrayOf<T>()[0]); }
			template<typename T> inline auto endSingle() noexcept 				{ return ItrSinglePtr<T>(&getArrayOf<T>()[this->size]); }
			template<typename T> inline auto endNextSingle() noexcept			{ return ItrSinglePtr<T>(&getArrayOf<T>()[this->sizeNext]); }

			template<typename T> inline auto beginSingle() const noexcept 		{ return ItrSingleCPtr<T>(&getArrayOf<T>()[0]); }
			template<typename T> inline auto endSingle() const noexcept 		{ return ItrSingleCPtr<T>(&getArrayOf<T>()[this->size]); }
			template<typename T> inline auto endNextSingle() const noexcept		{ return ItrSingleCPtr<T>(&getArrayOf<T>()[this->sizeNext]); }

			template<typename T> inline auto beginSingleIdx() noexcept 			{ return ItrSingleIdx<T>(0, *this); }
			template<typename T> inline auto endSingleIdx() noexcept 			{ return ItrSingleIdx<T>(this->size, *this); }
			template<typename T> inline auto endNextSingleIdx() noexcept 		{ return ItrSingleIdx<T>(this->sizeNext, *this); }

			template<typename T> inline auto beginSingleIdx() const noexcept 	{ return ItrSingleCIdx<T>(0, *this); }
			template<typename T> inline auto endSingleIdx() const noexcept 		{ return ItrSingleCIdx<T>(this->size, *this); }
			template<typename T> inline auto endNextSingleIdx() const noexcept 	{ return ItrSingleCIdx<T>(this->sizeNext, *this); }

			template<typename T> inline auto forSinglePtr() noexcept			{ return makeRange(beginSingle(), endSingle()); }
			template<typename T> inline auto forNextSinglePtr() noexcept		{ return makeRange(beginSingle(), endNextSingle()); }

			template<typename T> inline auto forSinglePtr() const noexcept		{ return makeRange(beginSingle(), endSingle()); }
			template<typename T> inline auto forNextSinglePtr() const noexcept	{ return makeRange(beginSingle(), endNextSingle()); }

			template<typename T> inline auto forSingleIdx() noexcept			{ return makeRange(beginSingleIdx(), endSingleIdx()); }
			template<typename T> inline auto forNextSingleIdx() noexcept		{ return makeRange(beginSingleIdx(), endNextSingleIdx()); }

			template<typename T> inline auto forSingleIdx() const noexcept		{ return makeRange(beginSingleIdx(), endSingleIdx()); }
			template<typename T> inline auto forNextSingleIdx() const noexcept	{ return makeRange(beginSingleIdx(), endNextSingleIdx()); }
	};
}

#endif