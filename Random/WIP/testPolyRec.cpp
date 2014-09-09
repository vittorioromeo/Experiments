#include <SSVUtils/SSVUtils.hpp>

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

	constexpr std::size_t s(10000000);
	constexpr std::size_t nn(6);

	Benchmark::start("VecUPtr");
	{
		VecUPtr<OBase> v;

		for(int n = 0; n < nn; ++n)
		{
			int m = s / 3;
			for(int i = 0; i < m * 1; ++i)
			{
				int k = i % 3;
 				if(k == 0) v.emplace_back(makeUPtr<OSmall>());
 				else if(k == 1) v.emplace_back(makeUPtr<OBig>());
 				else if(k == 2) v.emplace_back(makeUPtr<OHuge>());
			}
			v.clear();
			for(int i = 0; i < m * 2; ++i)
			{
				int k = i % 3;
 				if(k == 0) v.emplace_back(makeUPtr<OSmall>());
 				else if(k == 1) v.emplace_back(makeUPtr<OBig>());
 				else if(k == 2) v.emplace_back(makeUPtr<OHuge>());
			}
			v.clear();
			for(int i = 0; i < m * 3; ++i)
			{
				int k = i % 3;
 				if(k == 0) v.emplace_back(makeUPtr<OSmall>());
 				else if(k == 1) v.emplace_back(makeUPtr<OBig>());
 				else if(k == 2) v.emplace_back(makeUPtr<OHuge>());
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
				int k = i % 3;
 				if(k == 0) v.emplace_back(r.create<OSmall>());
 				else if(k == 1) v.emplace_back(r.create<OBig>());
 				else if(k == 2) v.emplace_back(r.create<OHuge>());
			}
			v.clear();
			for(int i = 0; i < m * 2; ++i)
			{
				int k = i % 3;
 				if(k == 0) v.emplace_back(r.create<OSmall>());
 				else if(k == 1) v.emplace_back(r.create<OBig>());
 				else if(k == 2) v.emplace_back(r.create<OHuge>());
			}
			v.clear();
			for(int i = 0; i < m * 3; ++i)
			{
				int k = i % 3;
 				if(k == 0) v.emplace_back(r.create<OSmall>());
 				else if(k == 1) v.emplace_back(r.create<OBig>());
 				else if(k == 2) v.emplace_back(r.create<OHuge>());
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