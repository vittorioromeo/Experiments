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

					template<typename TV, SVJ_ENABLE_IF_IS_NOT(TV, Maybe)> inline void init(TV&& mV) noexcept(noexcept(T(fwd<TV>(mV))))
					{
						SSVU_ASSERT(alive == false);

						new (&storage) T(fwd<TV>(mV));

						#if !(SSVU_IMPL_ASSERT_DISABLED)
							alive = true;
						#endif
					}
					template<typename TV, SVJ_ENABLE_IF_IS(TV, Maybe)> inline void init(TV&& mM) noexcept(noexcept(init(fwd<TV>(mM).get())))
					{
						init(fwd<TV>(mM).get());
					}

					inline void deinit() noexcept(isNothrowDestructible<T>())
					{
						SSVU_ASSERT(alive == true);

						get().~T();

						#if !(SSVU_IMPL_ASSERT_DISABLED)
							alive = false;
						#endif

						SSVU_ASSERT(alive == false);
					}

					inline auto& get() & noexcept				{ SSVU_ASSERT(alive == true); return reinterpret_cast<T&>(storage); }
					inline const auto& get() const& noexcept	{ SSVU_ASSERT(alive == true); return reinterpret_cast<const T&>(storage); }
					inline auto get() && noexcept				{ SSVU_ASSERT(alive == true); return std::move(reinterpret_cast<T&>(storage)); }
			};
		}
	}
}

#endif
