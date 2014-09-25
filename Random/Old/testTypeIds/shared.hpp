#pragma once

#include <SSVUtils/Core/Core.hpp>
#include <SSVUtils/Benchmark/Benchmark.hpp>

inline auto getLastIdx() noexcept
{
	static auto lastIdx(0u);
	return lastIdx++;
}

template<typename T> struct TypeId { static std::size_t id; };
template<typename T> std::size_t TypeId<T>::id{getLastIdx()};

struct Type00 { };
struct Type01 { };
struct Type02 { };
struct Type03 { };

void print1();
void print2();