// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_NEWHV_ITERATOR
#define SSVU_NEWHV_ITERATOR

#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/Range/Range.hpp"
#include "../../NewHV/Inc/Common.hpp"

namespace ssvu
{
	namespace Internal
	{
		template<typename T, typename TItrValue, typename TImpl> class HVItrSingleBase : public BaseAdaptorItrRnd<TItrValue, TImpl>
		{
			protected:
				TImpl impl;

			public:
				template<typename... TArgs> inline HVItrSingleBase(TItrValue mValue, TArgs&&... mArgs) noexcept 
					: BaseAdaptorItrRnd<TItrValue, TImpl>{mValue}, impl{fwd<TArgs>(mArgs)...} { }

				inline decltype(auto) operator*() noexcept			{ return impl.template get<T&>(this->itr); }
				inline decltype(auto) operator*() const noexcept	{ return impl.template get<const T&>(this->itr); }
				inline decltype(auto) operator->() noexcept			{ return &impl.template get<T&>(this->itr); }
				inline decltype(auto) operator->() const noexcept	{ return &impl.template get<const T&>(this->itr); }
		};

		template<typename T> struct HVItrSingleImplPtr
		{
			template<typename TR> inline TR get(T* mPtr) const noexcept { return *mPtr; }
		};

		template<typename THV, typename T> struct HVItrSingleImplIdx
		{
			THV& hVec;
			inline HVItrSingleImplIdx(THV& mHVec) noexcept : hVec{mHVec} { }
			template<typename TR> inline TR get(SizeT mI) const noexcept { return hVec.getItems()[mI]; }
		};

		template<typename THV, typename T> struct HVMultiItrSingleImplIdx
		{
			THV& hVec;
			inline HVMultiItrSingleImplIdx(THV& mHVec) noexcept : hVec{mHVec} { }
			template<typename TR> inline TR get(SizeT mI) const noexcept { return hVec.template getArrayOf<T>()[mI]; }
		};

		template<typename T> using HVItrSinglePtr =							HVItrSingleBase<T,			T*,			HVItrSingleImplPtr<T>>;
		template<typename T> using HVItrConstSinglePtr =					HVItrSingleBase<const T,	const T*,	HVItrSingleImplPtr<const T>>;

		template<typename T> using HVItrSingleIdx =							HVItrSingleBase<T,			HIdx,		HVItrSingleImplIdx<HVSingle<T>,			T>>;
		template<typename T> using HVItrConstSingleIdx =					HVItrSingleBase<const T,	HIdx,		HVItrSingleImplIdx<const HVSingle<T>,	const T>>;
	
		template<typename THV, typename T> using HVMultiItrSingleIdx =		HVItrSingleBase<T,			HIdx,		HVMultiItrSingleImplIdx<THV,		T>>;
		template<typename THV, typename T> using HVMultiItrConstSingleIdx =	HVItrSingleBase<const T,	HIdx,		HVMultiItrSingleImplIdx<const THV,	const T>>;
	}
}

#endif