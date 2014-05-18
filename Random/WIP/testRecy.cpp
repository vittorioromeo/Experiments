#include <SSVUtils/SSVUtils.hpp>

/*
namespace ssvu
{
	namespace Internal
	{
		template<typename T, typename TBase> class PolyPtrRecycler
		{
			private:
				std::vector<TBase*> ptrs;
				std::allocator<T> alloc;

			public:
				inline PolyPtrRecycler() = default;
				inline ~PolyPtrRecycler() noexcept { for(auto p : ptrs) alloc.deallocate(reinterpret_cast<T*>(p), 1); }

				inline T* allocate() noexcept(noexcept(alloc.allocate(1))) { return alloc.allocate(1); }

				template<typename... TArgs> inline void construct(T* mPtr, TArgs&&... mArgs) 
					noexcept(noexcept(alloc.construct(mPtr, std::forward<TArgs>(mArgs)...)))
				{ 
					alloc.construct(mPtr, std::forward<TArgs>(mArgs)...); 
				}

				inline void destroy(TBase* mPtr) noexcept(noexcept(alloc.destroy(mPtr))) 
				{ 
					alloc.destroy(mPtr); 
				}

				inline void push(TBase* mPtr) noexcept 
				{
					SSVU_ASSERT(mPtr != nullptr);

					ptrs.emplace_back(mPtr);
				}
				inline T* pop() noexcept
				{
					SSVU_ASSERT(ptrs.size() > 0);
					
					auto result(ptrs.back());
					ptrs.pop_back();
					return reinterpret_cast<T*>(result);
				}
				inline bool isEmpty() const noexcept { return ptrs.size() == 0; }
		};

		template<typename T, typename TBase> inline PolyPtrRecycler<T, TBase>& getPolyPtrRecycler() noexcept
		{
			static PolyPtrRecycler<T, TBase> result;
			return result;
		}

		template<typename T, typename TBase, typename... TArgs> inline T* makePolyPtr(TArgs&&... mArgs) 
		{
			auto& pr(getPolyPtrRecycler<T, TBase>());
			T* result{pr.isEmpty() ? pr.allocate() : pr.pop()};
			pr.construct(result, std::forward<TArgs>(mArgs)...);
			return result;
		}
	}
.
	template<typename T, typename TBase> using RecPolyUptr = Uptr<T, void(*)(TBase*)>;
	template<typename T, typename TBase, typename... TArgs> inline RecPolyUptr<T, TBase> makeRecPolyUptr(TArgs&&... mArgs) 
	{
		return RecPolyUptr<T, TBase>(Internal::makePolyPtr<T, TBase>(std::forward<TArgs>(mArgs)...), [](TBase* mPtr) 
		{
			auto& pr(Internal::getPolyPtrRecycler<T, TBase>());
			pr.destroy(mPtr); pr.push(mPtr);			
		});
	}

	template<typename T, typename TBase, typename... TArgs, typename TC> inline T& getEmplaceRecPolyUptr(TC& mContainer, TArgs&&... mArgs)
	{
		auto uptr(makeRecPolyUptr<T, TBase>(std::forward<TArgs>(mArgs)...));
		auto result(uptr.get());
		mContainer.emplace_back(std::move(uptr));
		return *result;
	}
}*/


namespace ssvu
{
	namespace Internal
	{
		template<typename TDerived, typename TBase> class RecMemoryManagerBase : protected RecPolyUptrVector<TBase>
		{
			template<typename T, typename P> friend void ssvu::eraseRemoveIf(T&, const P&);

			protected:
				using TUptr = RecUptr<TBase>;
				using Container = RecPolyUptrVector<TBase>;
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
				eraseRemoveIf(*this, this->template isDead<RecUptr<TBase>>);
			}
			template<typename TType = TBase, typename... TArgs> inline TType& createTImpl(TArgs&&... mArgs)
			{
				return ssvu::getEmplaceRecPolyUptr<TType, TBase>(this->toAdd, std::forward<TArgs>(mArgs)...);
			}
	};
}


struct Base : ssvu::MemoryManageable
{
	volatile int k;
	virtual void call() { } 
	virtual ~Base() { }
};

struct Der1 : public Base { char s[10]; void call() override { k += 1; } };
struct Der2 : public Base { char b[100]; void call() override { k += 2; } };

void doBench()
{
	using namespace ssvu;
	constexpr std::size_t s(10000000);
	constexpr int jj{10};

	Benchmark::start("Vector<Uptr>");
	{
		Benchmark::start("Create");
				
			std::vector<Uptr<Base>> v;
			v.reserve(s);
		
		Benchmark::endLo();

		Benchmark::start("Loop");
		for(int j{0}; j < jj; ++j)
		{
			Benchmark::start("Fill");
			{
				for(int i{0}; i < s; ++i)
				{
					if(i % 2 == 0) v.emplace_back(makeUptr<Der1>());
					else v.emplace_back(makeUptr<Der2>());
				}
			}
			Benchmark::endLo();
		
			Benchmark::start("Clear");
			{
				v.clear();
			}
			Benchmark::endLo();
		}
		Benchmark::endLo();
	}
	Benchmark::endLo();

	ssvu::lo() << "" << std::endl;

	Benchmark::start("Vector<RecPolyUptr>");
	{
		Benchmark::start("Create");
				
			std::vector<RecPolyUptr<Base, Base>> v;
			v.reserve(s);
		
		Benchmark::endLo();

		Benchmark::start("Loop");
		for(int j{0}; j < jj; ++j)
		{
			Benchmark::start("Fill");
			{
				for(int i{0}; i < s; ++i)
				{
					if(i % 2 == 0) v.emplace_back(makeRecPolyUptr<Der1, Base>());
					else v.emplace_back(makeRecPolyUptr<Der2, Base>());
				}
			}
			Benchmark::endLo();

			Benchmark::start("Clear");
			{
				v.clear();
			}
			Benchmark::endLo();
		}
		Benchmark::endLo();
	}
	Benchmark::endLo();
}

void doBench2()
{
	using namespace ssvu;
	constexpr std::size_t s(10000000);
	constexpr int jj{20};

for(int n{0}; n < 2; ++n){

	Benchmark::start("MemoryManager");
	{
		//Benchmark::start("Create");
				
			MemoryManager<Base> v;
		
		//Benchmark::endLo();

		//Benchmark::start("Loop");
		for(int j{0}; j < jj; ++j)
		{
		//	Benchmark::start("Fill");
			{
				for(int i{0}; i < s; ++i)
				{
					if(i % 2 == 0) v.create<Der1>();
					else v.create<Der2>();
				}
			}
		//	Benchmark::endLo();
		
		//	Benchmark::start("Refresh");
			{
				v.refresh();
			}
		//	Benchmark::endLo();

		//	Benchmark::start("SetDead");
			{
				int i{0};
				for(auto& r : v)
				{
					++i;
					if(i % 3 == 0) v.del(*r);
				}
			}
		//	Benchmark::endLo();

		//	Benchmark::start("Refresh");
			{
				v.refresh();
			}
		//	Benchmark::endLo();

		//	Benchmark::start("Clear");
			{
				v.clear();
			}
		//	Benchmark::endLo();
		}
	//	Benchmark::endLo();
	}
	Benchmark::endLo();

	ssvu::lo() << "" << std::endl;

	Benchmark::start("RecMemoryManager");
	{
		//Benchmark::start("Create");
				
			RecMemoryManager<Base> v;
		
		//Benchmark::endLo();

		//Benchmark::start("Loop");
		for(int j{0}; j < jj; ++j)
		{
			//Benchmark::start("Fill");
			{
				for(int i{0}; i < s; ++i)
				{
					if(i % 2 == 0) v.create<Der1>();
					else v.create<Der2>();
				}
			}
		//	Benchmark::endLo();
		
		//	Benchmark::start("Refresh");
			{
				v.refresh();
			}
		//	Benchmark::endLo();

		//	Benchmark::start("SetDead");
			{
				int i{0};
				for(auto& r : v)
				{
					++i;
					if(i % 3 == 0) v.del(*r);
				}
			}
		//	Benchmark::endLo();

		//	Benchmark::start("Refresh");
			{
				v.refresh();
			}
		//	Benchmark::endLo();

		//	Benchmark::start("Clear");
			{
				v.clear();
			}
		//	Benchmark::endLo();
		}
		//Benchmark::endLo();
	}
	Benchmark::endLo();
}
}

int main()
{

	ssvu::lo("N") << sizeof(ssvu::Uptr<Base>) << std::endl;
	ssvu::lo("R") << sizeof(ssvu::RecUptr<Base>) << std::endl;
	SSVU_ASSERT(false);
	//doBench();
	doBench2();
}








/*

#include <SSVUtils/SSVUtils.hpp>

namespace ssvu
{
	namespace Internal
	{
		template<std::size_t TS, typename T, typename TAlloc> class PtrBuffer 
		{
			public:
				using PtrType = T*;
			
			private:
				PtrType items[TS];
				std::size_t size{0u};

			public:
				TAlloc alloc;

				inline bool full() const noexcept 	{ return size == TS; }
				inline bool empty() const noexcept 	{ return size == 0; }
				inline void push(T* mPtr) noexcept 	{ items[size++] = mPtr; }
				inline T* pop() noexcept 			{ return items[--size]; }		    
		};

		template<typename T, typename TAlloc, typename TCont = PtrBuffer<128, T, TAlloc>> inline TCont* getMemWaste() 
		{
			thread_local std::unique_ptr<TCont, void(*)(TCont*)> tPtrs(new TCont{}, [](TCont* mC) 
			{
				while (!mC->empty()) mC->allocator.deallocate(mC->pop(), 1);				
				delete mC;
			});

			return tPtrs.get();
		}

		template<typename T, typename TAlloc, typename... TArgs> inline T* makePtr(TArgs&&... mArgs) 
		{
			auto q(getMemWaste<T, TAlloc>());
			T* result{q->empty() ? q->allocator.allocate(1) : q->pop()};
			q->allocator.construct(result, std::forward<TArgs>(mArgs)...);
			return result;
		}

		template<typename T, typename TSmart, typename TAlloc, typename... TArgs> inline TSmart makeSmart(TArgs&&... mArgs) 
		{
			return TSmart(Internal::makePtr<T, TAlloc>(std::forward<TArgs>(mArgs)...), [](T* mPtr) 
			{
				auto q(Internal::getMemWaste<T, TAlloc>());
				q->allocator.destroy(mPtr);
				
				if(q->full()) q->allocator.deallocate(mPtr, 1);
				else q->push(mPtr);			
			});
		}
	}


	template<typename T> using RecUptr = Uptr<T, void(*)(T*)>;
	template<typename T> using RecSptr = std::shared_ptr<T>;

	template<typename T, typename TBase> using RecPolyUptr = Uptr<T, void(*)(TBase*)>;

	template<typename T, typename... TArgs> inline RecUptr<T> makeRecUptr(TArgs&&... mArgs) 
	{
		return Internal::makeSmart<T, RecUptr<T>, std::allocator<T>>(std::forward<TArgs>(mArgs)...);
	}

	template<typename T, typename... TArgs> inline RecSptr<T> makeRecSptr(TArgs&&... mArgs) 
	{
		return Internal::makeSmart<T, RecSptr<T>, std::allocator<T>>(std::forward<TArgs>(mArgs)...);
	}
}

struct Base
{
	virtual ~Base() { }
};

struct Der1 : Base { };
struct Der2 : Base { };

int main()
{
	std::vector<ssvu::RecUptr<Base>> v;

	v.emplace_back(ssvu::makeRecUptr<Base>());
	v.emplace_back(ssvu::makeRecUptr<Der1>());
}

*/