#include "shared.hpp"

struct Type00;
struct Type01;
struct Type02;
struct Type03;

struct Type04 { };
struct Type05 { };
struct Type06 { };
struct Type07 { };

void print2()
{
	check(0, TypeId<Type00>::id);
	check(1, TypeId<Type01>::id);
	check(2, TypeId<Type02>::id);
	check(3, TypeId<Type03>::id);
	check(4, TypeId<Type04>::id);
	check(5, TypeId<Type05>::id);
	check(6, TypeId<Type06>::id);
	check(7, TypeId<Type07>::id);
}