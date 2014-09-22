#include <SSVUtils/Core/Core.hpp>
#include <SSVUtils/Benchmark/Benchmark.hpp>

int main()
{
	ssvu::lo() << std::left << std::setw(25) << "something"  << "too" << std::endl;
    ssvu::lo() << std::left << std::setw(25) << "something" << "too" << std::endl; 

    ssvu::lo() << std::left << std::setw(25) << std::string("something")  << "too" << std::endl; 
    ssvu::lo() << std::left << std::setw(20) << std::string("something")  << std::string("too") << std::endl; 
    ssvu::lo() << std::left << std::setw(15) << 10ul  << std::setw(15)<< std::string("too") << std::setw(15)<< 5u<< std::setw(15) << 15.f<< std::endl; 
    //std::cout << std::left << std::setw(25) << ssvu::Console::setColorFG(ssvu::Console::Color::Red) << "something" << "too" << std::endl;

	return 0;
}