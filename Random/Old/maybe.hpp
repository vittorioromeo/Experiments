#define SSVU_DEBUG 1

#include <SSVUtils/Core/Core.hpp>

namespace ssvu
{
	template<typename T> class Maybe
	{
		private:
			AlignedStorageFor<T> storage;

			#if defined(SSVU_DEBUG)
				bool clean{true};

				inline void setClean(bool mX) noexcept { clean = mX; }
				inline void assertClean() noexcept { SSVU_ASSERT(clean); }
				inline void assertNotClean() noexcept { SSVU_ASSERT(!clean); }
			#else
				inline void setClean(bool) noexcept { }
				inline void assertClean() noexcept { }
				inline void assertNotClean() noexcept { }
			#endif

		public:
			template<typename... TArgs> inline void init(TArgs&&... mArgs)
				noexcept(noexcept(T(SSVU_FWD(mArgs))))
			{
				assertClean();
				new (&storage) T(SSVU_FWD(mArgs));
				setClean(false);
			}

			inline void deinit() noexcept(isNothrowDtor<T>())
			{
				assertNotClean();
				castStorage<T>(storage).~T();
				setClean(true);
			}

			inline auto& get() noexcept { return castStorage<T>(storage); }
			inline const auto& get() const noexcept { return castStorage<T>(storage); }
			
			inline ~Maybe() noexcept { assertClean(); }
	};
}

int main()
{
	using namespace ssvu;

	Maybe<int> mbInt; 
	Maybe<std::vector<int>> mbVecInt;

	

	return 0;
}