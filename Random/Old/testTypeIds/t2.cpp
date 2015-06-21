#include "shared.hpp"

struct Type04 { };
struct Type05 { };
struct Type06 { };
struct Type07 { };

void print2()
{
	std::cout << "04 -> " << TypeId<Type04>::id << std::endl;
	std::cout << "05 -> " << TypeId<Type05>::id << std::endl;
	std::cout << "06 -> " << TypeId<Type06>::id << std::endl;
	std::cout << "07 -> " << TypeId<Type07>::id << std::endl;

	std::cout << "07 -> " << TypeId<Type07>::id << std::endl;
	std::cout << "06 -> " << TypeId<Type06>::id << std::endl;
	std::cout << "05 -> " << TypeId<Type05>::id << std::endl;
	std::cout << "04 -> " << TypeId<Type04>::id << std::endl;
}