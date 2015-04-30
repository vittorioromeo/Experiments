#include <SSVUtils/Core/Core.hpp>
#include <SSVUtils/Benchmark/Benchmark.hpp>
#include <SSVUtils/MemoryManager/MemoryManager.hpp>
#include <SSVUtils/Tests/Tests.hpp>

volatile int state{0};
template<typename T> struct OV : public T { bool alive{true}; };

void doBench()
{
	using namespace ssvu;

	struct OBase { virtual ~OBase() { } };
	struct OSmall : public OBase
	{
		char k[16];
		int myState;
		void a() { ++state; ++myState; }
		OSmall() { ++state; }
		~OSmall() override { ++state; }
	};
	struct OBig : public OBase
	{
		char k[128];
		int myState;
		void a() { ++state; ++myState; }
		OBig() { ++state; }
		~OBig() override { ++state; }
	};
	struct OHuge : public OBase
	{
		char k[256];
		int myState;
		void a() { ++state; ++myState; }
		OHuge() { ++state; }
		~OHuge() override { ++state; }
	};

	constexpr std::size_t s(200000);
	constexpr std::size_t nn(4);


	Benchmark::start("VecUPtr");
	if(false){
		VecUPtr<OBase> v;

		for(int n = 0; n < nn; ++n)
		{
			int m = s / 3;
			for(int i = 0; i < m * 1; ++i)
			{
				v.emplace_back(mkUPtr<OSmall>());
 			 	v.emplace_back(mkUPtr<OBig>());
 			 	v.emplace_back(mkUPtr<OHuge>());
			}
			v.clear();
			for(int i = 0; i < m * 2; ++i)
			{
				v.emplace_back(mkUPtr<OSmall>());
 			 	v.emplace_back(mkUPtr<OBig>());
 			 	v.emplace_back(mkUPtr<OHuge>());
			}
			v.clear();
			for(int i = 0; i < m * 3; ++i)
			{				
 			 	v.emplace_back(mkUPtr<OSmall>());
 			 	v.emplace_back(mkUPtr<OBig>());
 			 	v.emplace_back(mkUPtr<OHuge>());
			}
			v.clear();
		}
	}
	Benchmark::endLo();


	Benchmark::start("PolyRec");
	{
		PolyRecycler<OBase> r;
		std::vector<decltype(r)::PtrType> v;

		for(int n = 0; n < nn; ++n)
		{
			int m = s / 3;
			for(int i = 0; i < m * 1; ++i)
			{
				v.emplace_back(r.create<OSmall>());
 				v.emplace_back(r.create<OBig>());
 				v.emplace_back(r.create<OHuge>());
			}
			v.clear();
			for(int i = 0; i < m * 2; ++i)
			{
				v.emplace_back(r.create<OSmall>());
 				v.emplace_back(r.create<OBig>());
 				v.emplace_back(r.create<OHuge>());
			}
			v.clear();
			for(int i = 0; i < m * 3; ++i)
			{
				v.emplace_back(r.create<OSmall>());
 				v.emplace_back(r.create<OBig>());
 				v.emplace_back(r.create<OHuge>());
			}
			v.clear();
		}
	}
	Benchmark::endLo();


	Benchmark::start("PolyFixedRec");
	{
		PolyFixedRecycler<OBase, OSmall, OBig, OHuge> r;
		std::vector<decltype(r)::PtrType> v;

		for(int n = 0; n < nn; ++n)
		{
			int m = s / 3;
			for(int i = 0; i < m * 1; ++i)
			{
				v.emplace_back(r.create<OSmall>());
 				v.emplace_back(r.create<OBig>());
 				v.emplace_back(r.create<OHuge>());
			}
			v.clear();
			for(int i = 0; i < m * 2; ++i)
			{
				v.emplace_back(r.create<OSmall>());
 				v.emplace_back(r.create<OBig>());
 				v.emplace_back(r.create<OHuge>());
			}
			v.clear();
			for(int i = 0; i < m * 3; ++i)
			{
				v.emplace_back(r.create<OSmall>());
 				v.emplace_back(r.create<OBig>());
 				v.emplace_back(r.create<OHuge>());
			}
			v.clear();
		}
	}
	Benchmark::endLo();

	ssvu::lo() << "" << std::endl;
}

int main()
{
	SSVUT_RUN();

	doBench();
	return 0;
}