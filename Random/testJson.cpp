#include <SSVUtils/Core/Core.hpp>
#include "../SVJ/include/SVJ/SVJ.hpp"

int main()
{
	using namespace svj;

	Value v{"hello"};
	Value v2 = v;
	v = v2;
	v2 = v;
	v = std::move(v2);

	auto document(svj::getFromString(R"(

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
		}
	}

	)"));

	auto output = svj::getWriteToString(document);

	ssvu::lo() << output << std::endl;
/*
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
	return 0;
}