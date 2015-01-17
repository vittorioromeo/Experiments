// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_NEWHV_HANDLEBASE
#define SSVU_NEWHV_HANDLEBASE

namespace ssvu
{
	namespace Internal
	{
		template<typename T> class HVHandleBase
		{
			protected:
				/// @brief Internal pointer to the HandleVector.
				T* hVec;

				/// @brief Index of the mark to check.
				HIdx markIdx;

				/// @brief Counter of the handle. Will be compared to the mark's counter.
				HCtr ctr;

			public:
				inline HVHandleBase(T& mHVec, HIdx mMarkIdx, HCtr mCtr) noexcept : hVec{&mHVec}, markIdx{mMarkIdx}, ctr{mCtr} { }

				/// @brief Returns whether the handle is valid or not.
				/// @details The handle is considered valid only when it points to the item it originally pointed to.
				/// If the item is set to dead but the HandleVector was not yet refreshed, the handle is considered valid.
				bool isAlive() const noexcept;

				/// @brief Sets the pointed item's status as dead.
				void setDead() noexcept;
		};
	}	
}

#endif