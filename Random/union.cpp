#include <SSVUtils/Core/Core.hpp>

namespace ssvu
{
	namespace Internal
	{
		template<SizeT...> struct CTMax;
		template<SizeT TV0> struct CTMax<TV0> : IntegralConstant<SizeT, TV0> { };
		template<SizeT TV0, SizeT TV1, SizeT... TVs> struct CTMax<TV0, TV1, TVs...> : CTMax<TV0 < TV1 ? TV1 : TV0, TVs...> { };

		template<typename... TArgs> inline constexpr auto getCTMaxSize() noexcept	{ return CTMax<sizeof(TArgs)...>::value; }
		template<typename... TArgs> inline constexpr auto getCTMaxAlign() noexcept	{ return CTMax<alignof(TArgs)...>::value; }
	}

	template<typename... TTypes> class UnionVariant
	{
		private:
			static constexpr SizeT maxSize{Internal::getCTMaxSize<TTypes...>()};
			static constexpr SizeT maxAlign{Internal::getCTMaxAlign<TTypes...>()};

			AlignedStorage<maxSize, maxAlign> data;

		public:
			template<typename T, typename... TArgs> inline void init(TArgs&&... mArgs) noexcept(isNothrowConstructible<T, TArgs...>())
			{
				new (&data) T(fwd<TArgs>(mArgs)...);
			}
			template<typename T> inline void deinit() noexcept(isNothrowDestructible<T>())
			{
				get<T>().~T();
			}

			template<typename T> inline T& get() & noexcept				{ return reinterpret_cast<T&>(data); }
			template<typename T> inline const T& get() const& noexcept	{ return reinterpret_cast<const T&>(data); }
			template<typename T> inline T get() && noexcept				{ return std::move(reinterpret_cast<T&>(data)); }
	};
}

#define EXP(...) if(!(__VA_ARGS__)) { lo() << #__VA_ARGS__ << " NOT TRUE!" << std::endl; } do { } while(false)

int main()
{
	using namespace ssvu;


	UnionVariant<int, char, long, std::string, long long, double> u;

	u.init<double>(10.5);
	EXP(u.get<double>() == 10.5);
	u.deinit<double>();

	u.init<int>(10);
	EXP(u.get<int>() == 10);
	u.deinit<int>();

	u.init<int>(10);
	EXP(u.get<int>() == 10);
	u.deinit<int>();

	u.init<int>(10);
	EXP(u.get<int>() == 10);
	u.deinit<int>();
}