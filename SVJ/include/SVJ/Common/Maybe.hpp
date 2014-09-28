// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SVJ_COMMON_MAYBE
#define SVJ_COMMON_MAYBE

namespace ssvu
{
	namespace Json
	{
		namespace Internal
		{
			template<typename T> class Maybe
			{
				private:
					AlignedStorageBasic<T> storage;

				public:
					#if !(SSVU_IMPL_ASSERT_DISABLED)
						bool alive;
					#endif

					inline Maybe() noexcept = default;

					inline void init(const T& mV) noexcept(isNothrowConstructible<T>())
					{
						SSVU_ASSERT(alive == false);

						new (&storage) T(mV);

						#if !(SSVU_IMPL_ASSERT_DISABLED)
							alive = true;
						#endif
					}
					inline void init(T&& mV) noexcept(isNothrowMoveConstructible<T>())
					{
						SSVU_ASSERT(alive == false);

						new (&storage) T(std::move(mV));

						#if !(SSVU_IMPL_ASSERT_DISABLED)
							alive = true;
						#endif
					}
					inline void init(const Maybe& mM) noexcept(noexcept(init(mM.get())))		{ init(mM.get()); }
					inline void init(Maybe&& mM) noexcept(noexcept(init(std::move(mM.get()))))	{ init(std::move(mM.get())); }

					inline void deinit() noexcept(isNothrowDestructible<T>())
					{
						SSVU_ASSERT(alive == true);

						get().~T();

						#if !(SSVU_IMPL_ASSERT_DISABLED)
							alive = false;
						#endif

						SSVU_ASSERT(alive == false);
					}

					inline T& get() noexcept
					{
						SSVU_ASSERT(alive == true);
						return reinterpret_cast<T&>(storage);
					}
					inline const T& get() const noexcept
					{
						SSVU_ASSERT(alive == true);
						return reinterpret_cast<const T&>(storage);
					}

					/*
					inline T getMove() noexcept
					{
						SSVU_ASSERT(alive == true);
						return std::move(reinterpret_cast<T&>(storage));
					}
					*/
			};
		}
	}
}

#endif
