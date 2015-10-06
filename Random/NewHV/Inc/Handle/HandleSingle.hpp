// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_NEWHV_HANDLESINGLE
#define SSVU_NEWHV_HANDLESINGLE

namespace ssvu
{
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
}

#endif