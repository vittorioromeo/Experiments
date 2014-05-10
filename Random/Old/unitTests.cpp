#include <iostream>
#include <set>
#include <utility>
#include <cstddef>
#include <map>
#include <string>
#include <SSVUtils/SSVUtils.hpp>

#define NSUT_IMPL_GET_NAME_TYPE(mName) 		SSVPP_CAT(NSTestUnique_, mName, __LINE__)
#define NSUT_IMPL_GET_NAME_RUNNER(mName) 	SSVPP_CAT(NSUT_IMPL_GET_NAME_TYPE(mName), runner)
#define NSUT_IMPL_GET_NAME_INSTANCE(mName) 	SSVPP_CAT(NSUT_IMPL_GET_NAME_TYPE(mName), instance)
#define NSUT_IMPL_GET_KEY(mName) 			SSVPP_TOSTR(NSUT_IMPL_GET_NAME_TYPE(mName))

#define NSUT_IMPL_GENERATE_STRUCT(mName) \
	struct NSUT_IMPL_GET_NAME_TYPE(mName) : public ::NSUnitTest::TestBase \
		{ \
			inline NSUT_IMPL_GET_NAME_TYPE(mName) () : ::NSUnitTest::TestBase{SSVPP_TOSTR(mName), SSVPP_TOSTR(__LINE__), SSVPP_TOSTR(__FILE__)} { } \
			virtual void run() const override; \
		} NSUT_IMPL_GET_NAME_INSTANCE(mName); 
	
#define NSUT_IMPL_GENERATE_RUNNER(mName) \
	static ::NSUnitTest::Runner NSUT_IMPL_GET_NAME_RUNNER(mName) {[] \
	{ \
		if(::NSUnitTest::wasTestExecuted(NSUT_IMPL_GET_KEY(mName))) return; \
		::NSUnitTest::setTestExecuted(NSUT_IMPL_GET_KEY(mName)); \
		ssvu::getEmplaceUptr<NSUT_IMPL_GET_NAME_TYPE(mName)>(::NSUnitTest::getTestStorage(), NSUT_IMPL_GET_NAME_INSTANCE(mName)); \
	}}; 

#ifndef NSUT_TEST_DISABLE
	#define NSUT_TEST(mName) \
		NSUT_IMPL_GENERATE_STRUCT(mName) \
		NSUT_IMPL_GENERATE_RUNNER(mName) \
		inline void NSUT_IMPL_GET_NAME_TYPE(mName)::run() const 

	#define NSUT_EXPECT(mExpr) \
		while(true) \
		{ \
			if(!(mExpr)) throw ::NSUnitTest::Fail{this, SSVPP_TOSTR(mExpr)}; \
			break; \
		}

	#define NSUT_RUN_ALL_TESTS() ::NSUnitTest::runAllTests()
#else
	#define NSUT_TEST(mName) inline void SSVPP_CAT(NSUT_IMPL_GET_NAME_TYPE(mName), unused) ()
	#define NSUT_EXPECT(...)
	#define NSUT_RUN_ALL_TESTS() while(false){ } 
#endif

namespace NSUnitTest
{
	#ifndef NSUT_TEST_DISABLE
		struct Runner
		{
			template<typename T> inline Runner(const T& mAction) { mAction(); }		
		};

		class TestBase
		{
			private:
				const std::string name, line, file;

			public:
			    inline TestBase(std::string mName, std::string mLine, std::string mFile) : name{std::move(mName)}, line{std::move(mLine)}, file{std::move(mFile)} { }
			    inline virtual ~TestBase() { }
			    inline virtual void run() const { }

			    inline const std::string& getName() const 	{ return name; }
			    inline const std::string& getLine() const 	{ return line; }
			    inline const std::string& getFile() const 	{ return file; }
		};

		struct Fail	
		{ 
			const TestBase* test;
			const std::string expr;
			inline Fail(const TestBase* mTest, std::string mExpr) : test{mTest}, expr{std::move(mExpr)} { }
		};

		using TestStorage = std::vector<ssvu::Uptr<TestBase>>;
		using TestExecMap = std::map<std::string, bool>;
		
		inline TestStorage& getTestStorage() noexcept 	{ static TestStorage result; return result; }
		inline TestExecMap& getTestExecMap() noexcept	{ static TestExecMap result; return result; }
		
		inline bool wasTestExecuted(const std::string& mKey) noexcept	{ return getTestExecMap()[mKey]; }
		inline void setTestExecuted(const std::string& mKey) noexcept	{ getTestExecMap()[mKey] = true; }

		inline void runAllTests()
		{
			static bool	done{false};
			if(done) return;
			
			done = true;

			bool failure{false};

			for(const auto& t : ::NSUnitTest::getTestStorage()) 
			{ 
				try 
				{
					t->run(); 
				}
				catch(::NSUnitTest::Fail& mFail)
				{
					failure = true;

					auto& test(*mFail.test);

					ssvu::lo("Test") << "Test failure\n\n" 
						<< "Test:\t<"	<< test.getName() << ">\n"
						<< "Line:\t<" 	<< test.getLine() << ">\n"
						<< "File:\t<" 	<< test.getFile() << ">\n\n"
						<< "Expression:\n" << mFail.expr << "\n"
						<< "_________________________________________\n\n";
				}
			}

			if(!failure) ssvu::lo("Test") << "All tests passed!\n";

			ssvu::lo().flush();
		}
	#endif
}

NSUT_TEST(Banana) 
{
	NSUT_EXPECT(10 == 10);
	NSUT_EXPECT(15 == 15);
	NSUT_EXPECT(10 == 15);
}

int main() 
{ 
	NSUT_RUN_ALL_TESTS();
	return 0; 
}