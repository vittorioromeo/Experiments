#include <SSVUtils/SSVUtils.hpp>

namespace ssvu
{
	namespace Internal
	{
		template<typename TDerived, typename TBase> class RecMemoryManagerBase : protected VecUptrRec<TBase>
		{
			template<typename T, typename P> friend void ssvu::eraseRemoveIf(T&, const P&);

			protected:
				using TUptr = UptrRec<TBase>;
				using Container = VecUptrRec<TBase>;
				Container toAdd;

			public:
				using Container::begin;
				using Container::end;
				using Container::size;

				inline void clear()	noexcept { Container::clear(); toAdd.clear(); }
				inline void del(TBase& mItem) const noexcept { mItem.ssvu_mmAlive = false; }

				// Statically polymorphic methods
				inline void refresh() { reinterpret_cast<TDerived*>(this)->refreshImpl(); }
				template<typename TType = TBase, typename... TArgs> inline TType& create(TArgs&&... mArgs)
				{
					return reinterpret_cast<TDerived*>(this)->template createTImpl<TType, TArgs...>(std::forward<TArgs>(mArgs)...);
				}

				template<typename TType> inline static bool isAlive(const TType& mItem) noexcept	{ return mItem->ssvu_mmAlive; }
				template<typename TType> inline static bool isDead(const TType& mItem) noexcept		{ return !isAlive(mItem); }
		 };
	}

	template<typename TBase> class RecMemoryManager : public Internal::RecMemoryManagerBase<RecMemoryManager<TBase>, TBase>
	{
		public:
			inline void refreshImpl()
			{
				for(auto& i : this->toAdd) this->emplace_back(std::move(i)); this->toAdd.clear();
				eraseRemoveIf(*this, this->template isDead<UptrRec<TBase>>);
			}
			template<typename TType = TBase, typename... TArgs> inline TType& createTImpl(TArgs&&... mArgs)
			{
				return ssvu::getEmplaceUptrRecPoly<TType, TBase>(this->toAdd, std::forward<TArgs>(mArgs)...);
			}
	};
}


struct Base : ssvu::MemoryManageable
{
	volatile int k;
	virtual volatile void call() { } 
	virtual ~Base() { }
};

struct Der1 : public Base { char s[10]; volatile void call() override { k += 1; } };
struct Der2 : public Base { char b[100]; volatile void call() override { k += 2; } };

void doBench()
{
	using namespace ssvu;
	constexpr std::size_t s(10000000);
	constexpr int jj{10};

	Benchmark::start("Vector<Uptr>");
	{
		std::vector<Uptr<Base>> v;
		v.reserve(s);
		
		for(int j{0}; j < jj; ++j)
		{
			for(int i{0}; i < s; ++i)
			{
				if(i % 2 == 0) v.emplace_back(makeUptr<Der1>());
				else v.emplace_back(makeUptr<Der2>());
			}

			v.clear();
		}
	}
	Benchmark::endLo();

	Benchmark::start("Vector<UptrRecPoly>");
	{	
		VecUptrRec<Base> v;
		v.reserve(s);
		
		for(int j{0}; j < jj; ++j)
		{
			for(int i{0}; i < s; ++i)
			{
				if(i % 2 == 0) v.emplace_back(makeUptrRecPoly<Der1, Base>());
				else v.emplace_back(makeUptrRecPoly<Der2, Base>());
			}
	
			v.clear();
		}
	}
	Benchmark::endLo();
}

void doBench2()
{
	using namespace ssvu;
	constexpr std::size_t s(3500000);
	constexpr int jj{20};

	Benchmark::start("MemoryManager");
	{				
		MemoryManager<Base> v;		
		
		for(int j{0}; j < jj; ++j)
		{			
			for(int i{0}; i < s; ++i)
			{
				if(i % 2 == 0) v.create<Der1>();
				else v.create<Der2>();
			}
		
			v.refresh();
			
			int i{0};
			for(auto& r : v) if(i++ % 3 == 0) v.del(*r);
			
			v.refresh();
			v.clear();	
		}
	}
	Benchmark::endLo();

	Benchmark::start("RecMemoryManager");
	{				
		RecMemoryManager<Base> v;		
		
		for(int j{0}; j < jj; ++j)
		{			
			for(int i{0}; i < s; ++i)
			{
				if(i % 2 == 0) v.create<Der1>();
				else v.create<Der2>();
			}
		
			v.refresh();
			
			int i{0};
			for(auto& r : v) if(i++ % 3 == 0) v.del(*r);
			
			v.refresh();
			v.clear();	
		}
	}
	Benchmark::endLo();

	Benchmark::start("PolyManager");
	{				
		PolyManager<Base> v;		
		
		for(int j{0}; j < jj; ++j)
		{			
			for(int i{0}; i < s; ++i)
			{
				if(i % 2 == 0) v.create<Der1>();
				else v.create<Der2>();
			}
		
			v.refresh();
			
			int i{0};
			for(auto& r : v) if(i++ % 3 == 0) v.del(*r);
			
			v.refresh();
			v.clear();	
		}
	}
	Benchmark::endLo();
}

int main()
{

	ssvu::lo("N") << sizeof(ssvu::Uptr<Base>) << std::endl;
	ssvu::lo("R") << sizeof(ssvu::UptrRec<Base>) << std::endl;
	SSVU_ASSERT(false);
	//for(int n{0}; n < 3; ++n) doBench();
	for(int n{0}; n < 3; ++n) doBench2();
}