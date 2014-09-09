#include <future>
#include <SSVUtils/Core/Core.hpp>

int main() 
{
	auto t1(std::async(std::launch::async, []
	{
		ssvu::lo() << "starting t1" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(2));
		ssvu::lo() << "finished t1" << std::endl;
	}));

	auto t2(std::async(std::launch::async, []
	{
		ssvu::lo() << "starting t2" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(2));
		ssvu::lo() << "finished t2" << std::endl;
	}));

	//t1.get();
	//t2.get();

	return 0;
}