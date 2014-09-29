#include <SSVUtils/Core/Core.hpp>
#include <SSVUtils/Benchmark/Benchmark.hpp>
#include <SSVUtilsJson/SSVUtilsJson.hpp>
#include "../SVJ/include/SVJ/SVJ.hpp"

int main()
{
	using namespace ssvu;
	using namespace ssvu::Json;


	SSVUT_RUN();
	

	auto src(R"(

	{
		"n1": 10,
		"n2": 15.5,
		"n3": -35.5e+12,
		
		"s1": "bananas",	// Test comment
		"s2": "",
		"s3": "1test2",

		"a1": [1, 2, 3, "sup", { "lol":10 }], // Comment 2

		"o1": 
		{
			"b": true
		},

		"o2": 
		{
			"b": false
		},

		"o3":
		{
			"nv": null
		}, "bro": { 
			"n1": 10,
		"n2": 15.5,
		"n3": -35.5e+12,
		
		"s1": "bananas",	// Test comment
		"s2": "",
		"s3": "1test2",

		"a1": [1, 2, 3, "sup", { "lol":10 }], // Comment 2

		"o1": 
		{
			"b": true
		},

		"o2": 
		{
			"b": false
		},

		"o3":
		{
			"nv": null
		} }
	}

	)");


	while(false)
	{
		{
			SSVU_BENCHMARK_LOG_SCOPE_EXIT("svj");
			for(int i = 0; i < 10000; ++i)
			{
				auto document(Value::fromString(src));
			}
		}
	

	
		//if(false)
		{
			SSVU_BENCHMARK_LOG_SCOPE_EXIT("ssvuj");
			for(int i = 0; i < 10000; ++i)
			{
				auto document(ssvuj::getFromString(src));
			}
		}
	}

	ssvu::lo("Value") << sizeof(Value) << std::endl;
	ssvu::lo("Value::Holder") << sizeof(Value::Holder) << std::endl;
	ssvu::lo("hObject") << sizeof(decltype(Value::Holder::hObject)) << std::endl;	
	ssvu::lo("hArray") << sizeof(decltype(Value::Holder::hArray)) << std::endl;
	ssvu::lo("hString") << sizeof(decltype(Value::Holder::hString)) << std::endl;
	ssvu::lo("hNumber") << sizeof(decltype(Value::Holder::hNumber)) << std::endl;

}