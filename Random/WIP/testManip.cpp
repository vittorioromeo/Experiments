#include <SSVUtils/Core/Core.hpp>
#include <SSVUtils/Benchmark/Benchmark.hpp>

int main()
{
	std::cout << std::left << std::setw(25) << "something"  << "too" << std::endl; 
    ssvu::lo() << std::left << std::setw(25) << "something" << "too" << std::endl;   

    std::cout << std::left << std::setw(25) << "something"  << "too" << std::endl; 
    std::cout << std::left << std::setw(25) << ssvu::Console::setColorFG(ssvu::Console::Color::Red) << "something" << "too" << std::endl;

	return 0;
}