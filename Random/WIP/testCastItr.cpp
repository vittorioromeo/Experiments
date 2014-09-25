#include <future>
#include <SSVUtils/Core/Core.hpp>
#include <SSVUtils/Benchmark/Benchmark.hpp>

template<typename TBase, typename T> class CastItr
{
	private:
		TBase* ptr;
		template<typename TT> inline auto getCasted() { return reinterpret_cast<TT>(ptr); }

	public:
		inline CastItr(TBase* mPtr) noexcept : ptr{mPtr} { }

		inline auto& operator++() noexcept				{ ++ptr;			return *this; }
		inline auto& operator++(int) noexcept			{ ++ptr;			return *this; }
		inline auto& operator--() noexcept				{ --ptr;			return *this; }
		inline auto& operator--(int) noexcept			{ --ptr;			return *this; }
		inline auto& operator+=(int mOffset) noexcept	{ ptr += mOffset;	return *this; }
		inline auto& operator-=(int mOffset) noexcept	{ ptr -= mOffset;	return *this; }

		inline T& operator*() noexcept				{ return *getCasted<T*>(); }
		inline const T& operator*() const noexcept	{ return *getCasted<const T*>(); }
		inline T* operator->() noexcept				{ return getCasted<T*>(); }
		inline const T* operator->() const noexcept	{ return getCasted<const T*>(); }

		inline bool operator==(const CastItr& mRhs) const noexcept	{ return ptr == mRhs.ptr; }
		inline bool operator!=(const CastItr& mRhs) const noexcept	{ return ptr != mRhs.ptr; }
		inline bool operator<(const CastItr& mRhs) const noexcept	{ return ptr < mRhs.ptr; }
		inline bool operator>(const CastItr& mRhs) const noexcept	{ return ptr > mRhs.ptr; }
		inline bool operator<=(const CastItr& mRhs) const noexcept	{ return ptr <= mRhs.ptr; }
		inline bool operator>=(const CastItr& mRhs) const noexcept	{ return ptr >= mRhs.ptr; }
};

template<typename TBase, typename T, typename TItr> class CastItrAdaptor
{
	// ...
};

struct Base
{
	virtual ~Base() { }
	virtual void exec() { }
};

struct Der1 : public Base
{
	std::string a{"hi"};
	void exec() override { ssvu::lo() << a << "\n"; }
	void execNotOverr() { ssvu::lo() << "not overr " << a << "\n"; }
};

int main() 
{
	std::vector<ssvu::UPtr<Base>> v;
	v.emplace_back(ssvu::makeUPtr<Der1>());
	v.emplace_back(ssvu::makeUPtr<Der1>());
	v.emplace_back(ssvu::makeUPtr<Der1>());
	v.emplace_back(ssvu::makeUPtr<Der1>());

	CastItr<Base, Der1> itr{v[0].get()};
	CastItr<Base, Der1> itrEnd{v[v.size() - 1].get()};
	for(; itr != itrEnd; ++itr)
	{
		itr->execNotOverr();
	}

	return 0;
}
