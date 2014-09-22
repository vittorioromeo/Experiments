#include <future>
#include <SSVUtils/Core/Core.hpp>

int main() 
{
	auto t1(std::async(std::launch::async, []
	{
		ssvu::lo("t1") << "starting t1" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));
		ssvu::lo("t1") << "finished t1" << std::endl;
	}));

	auto t2(std::async(std::launch::async, []
	{
		ssvu::lo("t2") << "starting t2" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));
		ssvu::lo("t2") << "finished t2" << std::endl;
	}));

	auto t3(std::async(std::launch::async, []
	{
		ssvu::lo("t3") << "starting t3" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));
		ssvu::lo("t3") << "finished t3" << std::endl;
	}));

	//t1.get();
	//t2.get();

	return 0;
}