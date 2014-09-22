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

/*

stringifier<long>(number, postfix, cache)
{
	cache.append<Fmt>(blue);
	cache.append<Str>(toStr(number));
	cache.append<FmtOnlyStr>(postfix);
	cache.append<Commit>();
}

left << setw << 10ul << 5.f << setw << 15ul

cache:
	M   left
	M   setw
	F   blue
	S   number
	FOS postfix
	C

	F   red
	S   number
	FOS postfix
	C

	M   setw
	F   blue
	S   number
	FOS postfix
	C

...then...

cache:
	M   left		|1
	M   setw		|2
	F   blue		|0
	S   number		|3
	FOS postfix		|4
	C 				|5

	F   red 		|0     
	S   number		|1
	FOS postfix		|2
	C 				|3

	M   setw		|1
	F   blue		|0
	S   number		|2
	FOS postfix		|3
	C 				|4
*/