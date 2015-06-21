#include "shared.hpp"

struct Type00 { };
struct Type01 { };
struct Type02 { };
struct Type03 { };

void print1()
{
	std::cout << "00 -> " << TypeId<Type00>::id << std::endl;
	std::cout << "01 -> " << TypeId<Type01>::id << std::endl;
	std::cout << "02 -> " << TypeId<Type02>::id << std::endl;
	std::cout << "03 -> " << TypeId<Type03>::id << std::endl;

	std::cout << "00 -> " << TypeId<Type00>::id << std::endl;
	std::cout << "01 -> " << TypeId<Type01>::id << std::endl;
	std::cout << "02 -> " << TypeId<Type02>::id << std::endl;
	std::cout << "03 -> " << TypeId<Type03>::id << std::endl;
}