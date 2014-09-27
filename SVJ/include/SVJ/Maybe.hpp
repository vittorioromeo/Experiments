// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SVJ_MAYBE
#define SVJ_MAYBE

namespace svj
{
	namespace Internal
	{
		template<typename T> class Maybe
		{
			private:
				ssvu::AlignedStorageBasic<T> storage;

			public:
				inline Maybe() noexcept = default;

				inline void init(const T& mV) noexcept(ssvu::isNothrowConstructible<T>())	{ new (&storage) T(mV); }
				inline void init(T&& mV) noexcept(ssvu::isNothrowMoveConstructible<T>())	{ new (&storage) T(std::move(mV)); }
				inline void init(const Maybe& mM) noexcept(noexcept(init(mM.get())))		{ init(mM.get()); }
				inline void init(Maybe&& mM) noexcept(noexcept(init(std::move(mM.get()))))	{ init(std::move(mM.get())); }

				inline void deinit() noexcept(ssvu::isNothrowDestructible<T>())
				{
					DL("deinit call dtor");
					get().~T();
				}

				inline T& get() noexcept { return reinterpret_cast<T&>(storage); }
				inline const T& get() const noexcept { return reinterpret_cast<const T&>(storage); }
		};
	}
}

#endif
