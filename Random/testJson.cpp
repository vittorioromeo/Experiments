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

while(true)
{
	{
		SSVU_BENCHMARK_LOG_SCOPE_EXIT("svj");
		for(int i = 0; i < 10000; ++i)
		{
			auto document(Value::fromString(src));
		}
	}

	{
		SSVU_BENCHMARK_LOG_SCOPE_EXIT("ssvuj");
		for(int i = 0; i < 10000; ++i)
		{
			auto document(ssvuj::getFromString(src));
		}
	}

	std::this_thread::sleep_for(1s);
}
return 0;
/*
	Value k{Object{}};
	k["x"] = Array{"a"};


	Value v{Object{}};

	v["stuff"] = Array
	{
		"red", 
		"blue", 
		33, 
		125.f, 
		
		Object
		{
			{"banana", "yummy"},
			{"avocado", "yucky"},
			{"favorites", Array{"croissant", "yogurt"}},
			{"hungry", true},
			{"chocolate", Null{}}
		}
	};

	v["stuff"].get<Array>().clear();

const auto& a = v["stuff"].get<Array>();


	Value v2{v};
	

	ssvu::lo("v") << v << std::endl;	
	ssvu::lo("v") << v2 << std::endl;

	Value v3{Array{1, 2, 3, 4 ,5}};

	ssvu::lo("v") << v3 << std::endl;



	

	auto output = document.getWriteToString<WriterMode::Minified>();

	auto doc2 = Value::fromString(output);

	ssvu::lo() << output << std::endl;
	ssvu::lo() << doc2 << std::endl;

	ssvu::lo("Value") << sizeof(Value) << std::endl;
	ssvu::lo("Value::Holder") << sizeof(Value::Holder) << std::endl;
	ssvu::lo("hObject") << sizeof(decltype(Value::Holder::hObject)) << std::endl;
	ssvu::lo("hObject impl") << sizeof(Internal::ObjectImpl<Value>) << std::endl;
	ssvu::lo("hArray") << sizeof(decltype(Value::Holder::hArray)) << std::endl;
	ssvu::lo("hString") << sizeof(decltype(Value::Holder::hString)) << std::endl;
	ssvu::lo("hNumber") << sizeof(decltype(Value::Holder::hNumber)) << std::endl;
	
	//ssvu::lo("Number") << sizeof(Number) << std::endl;

	auto document(svj::parseDocument(R"(

	{
		"n1": 10,
		
		"s1": "bananas"	// Test comment
		
	}

	)"));
*/

	
/*

	using namespace ssvu;

	lo() << document.get<svj::Object>().at("n1").get<int>() << std::endl;
	lo() << document.get<svj::Object>().at("n2").get<float>() << std::endl;
	lo() << document.get<svj::Object>().at("n3").get<double>() << std::endl;

	lo() << document.get<svj::Object>().at("s1").get<std::string>() << std::endl;
	lo() << document.get<svj::Object>().at("s2").get<std::string>() << std::endl;
	lo() << document.get<svj::Object>().at("s3").get<std::string>() << std::endl;

	lo() << document.get<svj::Object>().at("a1").get<svj::Array>().at(3).get<std::string>() << std::endl;

	lo() << document.get<svj::Object>().at("o1").get<svj::Object>().at("b").get<bool>() << std::endl;	
	lo() << document.get<svj::Object>().at("o2").get<svj::Object>().at("b").get<bool>() << std::endl;	

	lo() << "\n\n\n";

	auto doc2 = document;

	lo() << doc2.get<svj::Object>().at("n1").get<int>() << std::endl;
	lo() << doc2.get<svj::Object>().at("n2").get<float>() << std::endl;
	lo() << doc2.get<svj::Object>().at("n3").get<double>() << std::endl;

	lo() << doc2.get<svj::Object>().at("s1").get<std::string>() << std::endl;
	lo() << doc2.get<svj::Object>().at("s2").get<std::string>() << std::endl;
	lo() << doc2.get<svj::Object>().at("s3").get<std::string>() << std::endl;

	lo() << doc2.get<svj::Object>().at("a1").get<svj::Array>().at(3).get<std::string>() << std::endl;

	lo() << doc2.get<svj::Object>().at("o1").get<svj::Object>().at("b").get<bool>() << std::endl;	
	lo() << doc2.get<svj::Object>().at("o2").get<svj::Object>().at("b").get<bool>() << std::endl;	

	lo() << "\n\n\n";

	auto doc3 = std::move(document);

	lo() << doc3.get<svj::Object>().at("n1").get<int>() << std::endl;
	lo() << doc3.get<svj::Object>().at("n2").get<float>() << std::endl;
	lo() << doc3.get<svj::Object>().at("n3").get<double>() << std::endl;

	lo() << doc3.get<svj::Object>().at("s1").get<std::string>() << std::endl;
	lo() << doc3.get<svj::Object>().at("s2").get<std::string>() << std::endl;
	lo() << doc3.get<svj::Object>().at("s3").get<std::string>() << std::endl;

	lo() << doc3.get<svj::Object>().at("a1").get<svj::Array>().at(3).get<std::string>() << std::endl;

	lo() << doc3.get<svj::Object>().at("o1").get<svj::Object>().at("b").get<bool>() << std::endl;	
	lo() << doc3.get<svj::Object>().at("o2").get<svj::Object>().at("b").get<bool>() << std::endl;
*/
}