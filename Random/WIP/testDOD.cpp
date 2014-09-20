#include <SSVUtils/Core/Core.hpp>
#include <SSVUtils/Benchmark/Benchmark.hpp>

struct Type00 { };
struct Type01 { };
struct Type02 { };
struct Type03 { };
struct Type04 { };
struct Type05 { };
struct Type06 { };
struct Type07 { };
struct Type08 { };
struct Type09 { };
struct Type10 { };
struct Type11 { };
struct Type12 { };
struct Type13 { };
struct Type14 { };
struct Type15 { };
struct Type16 { };
struct Type17 { };
struct Type18 { };
struct Type19 { };
struct Type20 { };
struct Type21 { };
struct Type22 { };
struct Type23 { };
struct Type24 { };
struct Type25 { };
struct Type26 { };
struct Type27 { };
struct Type28 { };
struct Type29 { };
struct Type30 { };

inline auto getLastIdx() noexcept
{
	static auto lastIdx(0u);
	return lastIdx++;
}

template<typename T> inline const auto& getTypeId() noexcept
{	
	static auto idx(getLastIdx()); 
	return idx;
}

template<typename T> struct TypeId
{
	static std::size_t id;
};

template<typename T> std::size_t TypeId<T>::id{getLastIdx()};


struct TypeIdManager
{
    static std::size_t lastId;
    template<typename T> inline static std::size_t get() noexcept
    {
        static std::size_t id{lastId++};
        return id;
    }
};

std::size_t TypeIdManager::lastId{0u};

volatile std::size_t f;



int main()
{
	int kk = 1000000;

	for(int ll = 0; ll < 3; ++ll)
	{

	{
		SSVU_BENCHMARK_LOG_SCOPE_EXIT("templates");

		for(int k = 0; k < kk; ++k)
		{
			f += getTypeId<Type00>();
			f += getTypeId<Type01>();
			f += getTypeId<Type02>();
			f += getTypeId<Type03>();
			f += getTypeId<Type04>();
			f += getTypeId<Type05>();
			f += getTypeId<Type06>();
			f += getTypeId<Type07>();
			f += getTypeId<Type08>();
			f += getTypeId<Type09>();
			f += getTypeId<Type10>();
			f += getTypeId<Type11>();
			f += getTypeId<Type12>();
			f += getTypeId<Type13>();
			f += getTypeId<Type14>();
			f += getTypeId<Type15>();
			f += getTypeId<Type16>();
			f += getTypeId<Type17>();
			f += getTypeId<Type18>();
			f += getTypeId<Type19>();
			f += getTypeId<Type20>();
			f += getTypeId<Type21>();
			f += getTypeId<Type22>();
			f += getTypeId<Type23>();
			f += getTypeId<Type24>();
			f += getTypeId<Type25>();
			f += getTypeId<Type26>();
			f += getTypeId<Type27>();
			f += getTypeId<Type28>();
			f += getTypeId<Type29>();
			f += getTypeId<Type30>();
		}
	} 

	{
		SSVU_BENCHMARK_LOG_SCOPE_EXIT("typeid");

		for(int k = 0; k < kk; ++k)
		{
			f += typeid(Type00).hash_code();
			f += typeid(Type01).hash_code();
			f += typeid(Type02).hash_code();
			f += typeid(Type03).hash_code();
			f += typeid(Type04).hash_code();
			f += typeid(Type05).hash_code();
			f += typeid(Type06).hash_code();
			f += typeid(Type07).hash_code();
			f += typeid(Type08).hash_code();
			f += typeid(Type09).hash_code();
			f += typeid(Type10).hash_code();
			f += typeid(Type11).hash_code();
			f += typeid(Type12).hash_code();
			f += typeid(Type13).hash_code();
			f += typeid(Type14).hash_code();
			f += typeid(Type15).hash_code();
			f += typeid(Type16).hash_code();
			f += typeid(Type17).hash_code();
			f += typeid(Type18).hash_code();
			f += typeid(Type19).hash_code();
			f += typeid(Type20).hash_code();
			f += typeid(Type21).hash_code();
			f += typeid(Type22).hash_code();
			f += typeid(Type23).hash_code();
			f += typeid(Type24).hash_code();
			f += typeid(Type25).hash_code();
			f += typeid(Type26).hash_code();
			f += typeid(Type27).hash_code();
			f += typeid(Type28).hash_code();
			f += typeid(Type29).hash_code();
			f += typeid(Type30).hash_code();
		}
	} 

	{
		SSVU_BENCHMARK_LOG_SCOPE_EXIT("templates2");

		for(int k = 0; k < kk; ++k)
		{
			f += TypeId<Type00>::id;
			f += TypeId<Type01>::id;
			f += TypeId<Type02>::id;
			f += TypeId<Type03>::id;
			f += TypeId<Type04>::id;
			f += TypeId<Type05>::id;
			f += TypeId<Type06>::id;
			f += TypeId<Type07>::id;
			f += TypeId<Type08>::id;
			f += TypeId<Type09>::id;
			f += TypeId<Type10>::id;
			f += TypeId<Type11>::id;
			f += TypeId<Type12>::id;
			f += TypeId<Type13>::id;
			f += TypeId<Type14>::id;
			f += TypeId<Type15>::id;
			f += TypeId<Type16>::id;
			f += TypeId<Type17>::id;
			f += TypeId<Type18>::id;
			f += TypeId<Type19>::id;
			f += TypeId<Type20>::id;
			f += TypeId<Type21>::id;
			f += TypeId<Type22>::id;
			f += TypeId<Type23>::id;
			f += TypeId<Type24>::id;
			f += TypeId<Type25>::id;
			f += TypeId<Type26>::id;
			f += TypeId<Type27>::id;
			f += TypeId<Type28>::id;
			f += TypeId<Type29>::id;
			f += TypeId<Type30>::id;
		}
	} 


	{
		SSVU_BENCHMARK_LOG_SCOPE_EXIT("templates3");

		for(int k = 0; k < kk; ++k)
		{
			f += TypeIdManager::get<Type00>();
			f += TypeIdManager::get<Type01>();
			f += TypeIdManager::get<Type02>();
			f += TypeIdManager::get<Type03>();
			f += TypeIdManager::get<Type04>();
			f += TypeIdManager::get<Type05>();
			f += TypeIdManager::get<Type06>();
			f += TypeIdManager::get<Type07>();
			f += TypeIdManager::get<Type08>();
			f += TypeIdManager::get<Type09>();
			f += TypeIdManager::get<Type10>();
			f += TypeIdManager::get<Type11>();
			f += TypeIdManager::get<Type12>();
			f += TypeIdManager::get<Type13>();
			f += TypeIdManager::get<Type14>();
			f += TypeIdManager::get<Type15>();
			f += TypeIdManager::get<Type16>();
			f += TypeIdManager::get<Type17>();
			f += TypeIdManager::get<Type18>();
			f += TypeIdManager::get<Type19>();
			f += TypeIdManager::get<Type20>();
			f += TypeIdManager::get<Type21>();
			f += TypeIdManager::get<Type22>();
			f += TypeIdManager::get<Type23>();
			f += TypeIdManager::get<Type24>();
			f += TypeIdManager::get<Type25>();
			f += TypeIdManager::get<Type26>();
			f += TypeIdManager::get<Type27>();
			f += TypeIdManager::get<Type28>();
			f += TypeIdManager::get<Type29>();
			f += TypeIdManager::get<Type30>();
		}
	} 
}
	return 0;
}