#include <SSVUtils/Core/Core.hpp>
#include <SSVUtils/Benchmark/Benchmark.hpp>

using ComponentBitType = std::size_t;

template<ComponentBitType TB> struct ComponentBit : std::integral_constant<ComponentBitType, TB> { };

struct Type00 : ComponentBit<0> { };
struct Type01 : ComponentBit<1> { };
struct Type02 : ComponentBit<2> { };
struct Type03 : ComponentBit<3> { };
struct Type04 : ComponentBit<4> { };
struct Type05 : ComponentBit<5> { };
struct Type06 : ComponentBit<6> { };
struct Type07 : ComponentBit<7> { };
struct Type08 : ComponentBit<8> { };
struct Type09 : ComponentBit<9> { };
struct Type10 : ComponentBit<10> { };

inline auto getLast() noexcept { static ComponentBitType result{0}; return result++; }
template<typename T> inline auto getSId() noexcept { static auto result(getLast()); return result; }

template<typename T> struct TypeId { static ComponentBitType id; };
template<typename T> ComponentBitType TypeId<T>::id{getLast()};

template<typename T> inline auto getSIdImp() noexcept { return TypeId<T>::id; }

volatile int f{0};

int main() 
{
	int kk = 1000000;

	for(int ll = 0; ll < 3; ++ll)
	{
		{
			SSVU_BENCHMARK_LOG_SCOPE_EXIT("getsid");

			for(int k = 0; k < kk; ++k)
			{
				f += getSId<Type00>();
				f += getSId<Type01>();
				f += getSId<Type02>();
				f += getSId<Type03>();
				f += getSId<Type04>();
				f += getSId<Type05>();
				f += getSId<Type06>();
				f += getSId<Type07>();
				f += getSId<Type08>();
				f += getSId<Type09>();
				f += getSId<Type10>();
			}
		}

		{
			SSVU_BENCHMARK_LOG_SCOPE_EXIT("inheritance");

			for(int k = 0; k < kk; ++k)
			{
				f += Type00::value;
				f += Type01::value;
				f += Type02::value;
				f += Type03::value;
				f += Type04::value;
				f += Type05::value;
				f += Type06::value;
				f += Type07::value;
				f += Type08::value;
				f += Type09::value;
				f += Type10::value;
			}
		}

		{
			SSVU_BENCHMARK_LOG_SCOPE_EXIT("getsidimp");

			for(int k = 0; k < kk; ++k)
			{
				f += getSIdImp<Type00>();
				f += getSIdImp<Type01>();
				f += getSIdImp<Type02>();
				f += getSIdImp<Type03>();
				f += getSIdImp<Type04>();
				f += getSIdImp<Type05>();
				f += getSIdImp<Type06>();
				f += getSIdImp<Type07>();
				f += getSIdImp<Type08>();
				f += getSIdImp<Type09>();
				f += getSIdImp<Type10>();
			}
		}
	} 
}

// TODO: 	* implement in SSES and CESystem
// 			* static_asserts to assure all ids are unique
//			* static_asserts to assure all ids are lower than max
// 			* bitset generation... etc... (just implement)
//			* benchmark against static id getters
//			* test virtual with 2 inheritance