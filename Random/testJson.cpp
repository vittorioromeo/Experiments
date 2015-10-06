
#include <SSVUtils/Core/Core.hpp>
#include <SSVUtils/Benchmark/Benchmark.hpp>
#include <SSVUtils/Json/Json.hpp>
#include <SSVUtils/Test/Test.hpp>
// #include <SSVUtils/Tests/Tests.hpp>

template <typename T>
struct TestVec2
{
    T x, y;
};


SSVJ_CNV_NAMESPACE()
{
    template <typename T>
    SSVJ_CNV(TestVec2<T>, mV, mX)
    {
        ssvj::cnvArr(mV, mX.x, mX.y);
    }
    SSVJ_CNV_END();
}
SSVJ_CNV_NAMESPACE_END()

int main()
{
    using namespace ssvu;
    using namespace ssvu::Json;

    SSVUT_RUN();

    // auto x = TestVec2<float>{1.5f, 0.5f};
    // auto vx = ssvj::Val{x};
    // lo() << vx << "\n";

    // auto sx = vx.getWriteToStr();
    // lo() << sx << "\n";

    // auto osx = ssvj::Val::fromStr(sx);
    // lo() << osx[0] << ", " << osx[1] << "\n";

    // TestVec2<float> out;
    // extrArr(osx, out.x, out.y);
    // lo() << out.x << ", " << out.y << "\n";

    // auto osxas = osx.as<TestVec2<float>>();
    // lo() << osxas.x << ", " << osxas.y << "\n";

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
                auto document(Val::fromStr(src));
            }
        }
    }

    /*ssvu::lo("Value") << sizeof(Value) << std::endl;
    ssvu::lo("Value::Holder") << sizeof(Value::Holder) << std::endl;
    ssvu::lo("hObject") << sizeof(decltype(Value::Holder::hObject)) <<
    std::endl;
    ssvu::lo("hArray") << sizeof(decltype(Value::Holder::hArray)) << std::endl;
    ssvu::lo("hString") << sizeof(decltype(Value::Holder::hString)) <<
    std::endl;
    ssvu::lo("hNumber") << sizeof(decltype(Value::Holder::hNumber)) <<
    std::endl;*/
}