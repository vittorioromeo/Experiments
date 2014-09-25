#include "shared.hpp"

void print1()
{
	ssvu::lo("00") << TypeId<Type00>::id << std::endl;
	ssvu::lo("01") << TypeId<Type01>::id << std::endl;
	ssvu::lo("02") << TypeId<Type02>::id << std::endl;
	ssvu::lo("03") << TypeId<Type03>::id << std::endl;

	ssvu::lo("00") << TypeId<Type00>::id << std::endl;
	ssvu::lo("01") << TypeId<Type01>::id << std::endl;
	ssvu::lo("02") << TypeId<Type02>::id << std::endl;
	ssvu::lo("03") << TypeId<Type03>::id << std::endl;
}