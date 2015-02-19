// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_NEWHV_HANDLEMULTI
#define SSVU_NEWHV_HANDLEMULTI

namespace ssvu
{
	/// @brief Handle class that points to HandleVector elements.
	template<typename... TTs> class HVHandleMulti : public Internal::HVHandleBase<HVMulti<TTs...>>
	{
		template<typename, typename> friend class ssvu::Internal::HVImpl;

		private:
			inline HVHandleMulti(HVMulti<TTs...>& mHVec, HIdx mMarkIdx, HCtr mCtr) noexcept
				: Internal::HVHandleBase<HVMulti<TTs...>>{mHVec, mMarkIdx, mCtr} { }

			/// @brief Internal implementation method that returns a reference or a const reference to the item tuple.
			template<typename T, typename TR> inline TR getImpl() noexcept
			{
				SSVU_ASSERT(this->isAlive());
				return this->hVec->template getItemFromMark<T>(this->markIdx);
			}

		public:
			inline auto getRefTpl() noexcept		{ return std::tuple<TTs&...>{getImpl<TTs, TTs&>()...}; }
			inline auto getRefTpl() const noexcept	{ return std::tuple<const TTs&...>{getImpl<TTs, const TTs&>()...}; }

			/// @brief Returns a reference to the data. Assumes the handle is valid.
			template<typename T> inline T& get() noexcept { return getImpl<T, T&>(); }

			/// @brief Returns a const reference to the data. Assumes the handle is valid.
			template<typename T> inline const T& get() const noexcept { return getImpl<T, const T&>(); }

			// Pointer-like interface
			inline auto operator*() noexcept		{ return getRefTpl(); }
			inline auto operator*() const noexcept	{ return getRefTpl(); }
	};		
}

#endif