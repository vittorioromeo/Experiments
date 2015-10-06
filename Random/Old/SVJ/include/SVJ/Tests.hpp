// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SVJ_TESTS
#define SVJ_TESTS

#include <SSVUtils/Test/Test.hpp>

SSVUT_TEST(SVJNumberTests)
{
    using namespace ssvu;
    using namespace ssvu::Json;
    using namespace ssvu::Json::Internal;

#define EXEC_NUMBER_TESTS()                                      \
    SSVUT_EXPECT(n.get<char>() == char{10});                     \
    SSVUT_EXPECT(n.get<int>() == 10);                            \
    SSVUT_EXPECT(n.get<long int>() == 10l);                      \
    SSVUT_EXPECT(n.get<unsigned char>() == (unsigned char){10}); \
    SSVUT_EXPECT(n.get<unsigned int>() == 10u);                  \
    SSVUT_EXPECT(n.get<unsigned long int>() == 10ul);            \
    SSVUT_EXPECT(n.get<float>() == 10.f);                        \
    SSVUT_EXPECT(n.get<double>() == 10.);

    {
        Number n;

        n.set(10);
        SSVUT_EXPECT(n.getType() == Number::Type::IntS);
        EXEC_NUMBER_TESTS()

        n.set(10u);
        SSVUT_EXPECT(n.getType() == Number::Type::IntU);
        EXEC_NUMBER_TESTS()

        n.set(10.f);
        SSVUT_EXPECT(n.getType() == Number::Type::Real);
        EXEC_NUMBER_TESTS()
    }

#undef EXEC_NUMBER_TESTS

    Number ns{15}, nsu{15u}, nsul{15ul}, nf{15.f}, nd{15.0};
    SSVUT_EXPECT(ns == nsu);
    SSVUT_EXPECT(nsu == nsul);
    SSVUT_EXPECT(nsul == nf);
    SSVUT_EXPECT(nf == nd);
}

SSVUT_TEST(SVJValueTests)
{
    using namespace ssvu;
    using namespace ssvu::Json;
    using namespace ssvu::Json::Internal;

#define EXEC_TEST_BASIC(mType, mValue)           \
    {                                            \
        Value v0, v1, v2;                        \
        v0 = mValue;                             \
        v1 = v0;                                 \
        SSVUT_EXPECT(v0.get<mType>() == mValue); \
        SSVUT_EXPECT(v1.get<mType>() == mValue); \
        SSVUT_EXPECT(v0 == v1);                  \
        v2 = std::move(v1);                      \
        SSVUT_EXPECT(v2.get<mType>() == mValue); \
        v0 = Object{};                           \
        v0["inner"] = v2;                        \
        SSVUT_EXPECT(v0["inner"] == v2);         \
        auto sv0(v0.getWriteToString());         \
        auto sv2(v2.getWriteToString());         \
        auto osv0(Value::fromString(sv0));       \
        auto osv2(Value::fromString(sv2));       \
        SSVUT_EXPECT(v0 == osv0);                \
        SSVUT_EXPECT(v2 == osv2);                \
        SSVUT_EXPECT(v0["inner"] == osv2);       \
        SSVUT_EXPECT(osv0["inner"] == v2);       \
        SSVUT_EXPECT(osv0["inner"] == osv2);     \
    }

    EXEC_TEST_BASIC(bool, true)
    EXEC_TEST_BASIC(bool, false)
    EXEC_TEST_BASIC(char, 'a')
    EXEC_TEST_BASIC(int, 10)
    EXEC_TEST_BASIC(long int, 10l)
    EXEC_TEST_BASIC(unsigned char, 'a')
    EXEC_TEST_BASIC(unsigned int, 10u)
    EXEC_TEST_BASIC(unsigned long int, 10ul)
    EXEC_TEST_BASIC(float, 10.f)
    EXEC_TEST_BASIC(double, 10.)

    EXEC_TEST_BASIC(String, "hello")
    EXEC_TEST_BASIC(String, "hello"s)

    {
        Array a{"hello", "bye"s, 10, 15.f, 'u'};
        EXEC_TEST_BASIC(Array, a)
    }

    {
        Object o{{"hello", "bye"}, {"welcome"s, "goodbye"s}, {"banana", 15},
            {"best letter", 'v'}};
        EXEC_TEST_BASIC(Object, o)
    }
}

SSVUT_TEST(SVJReadTests)
{
    using namespace ssvu;
    using namespace ssvu::Json;
    using namespace ssvu::Json::Internal;

    auto testSrc(R"(
	{
		// Hello
		"a":1,//Comment // iko
		"b":null,//
		"c":"//",//
		"d":true,//////
		"e":"//\"//",//k
		"f"//a
		://a
		{//a
		 //a

				"oo"//a
		 ://g
		 [//a
		 1,//b // h
		 2,//h // ds
		 3//l
		 ]//g
		 ,//p
		 //p //pp p p
		 "2"://k
		 2
////////
		 ///s
   //s/s/

		}//a
	}
	)");

    auto v(Value::fromString(testSrc));

    SSVUT_EXPECT(v["a"] == 1);
    SSVUT_EXPECT(v["b"] == Null{});
    SSVUT_EXPECT(v["c"] == "//");
    SSVUT_EXPECT(v["d"] == true);
    SSVUT_EXPECT(v["e"] == "//\"//");
}

#endif
