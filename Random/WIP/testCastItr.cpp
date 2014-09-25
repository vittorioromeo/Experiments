#include <future>
#include <SSVUtils/Core/Core.hpp>
#include <SSVUtils/Benchmark/Benchmark.hpp>

template<typename TItr> class BaseAdaptorRandomItr
{
	protected:
		using BaseType = BaseAdaptorRandomItr;
		TItr itr;	

	public:
		inline BaseAdaptorRandomItr(TItr mItr) noexcept : itr{mItr} { }

		inline auto& operator++() noexcept				{ ++itr;			return *this; }
		inline auto& operator++(int) noexcept			{ ++itr;			return *this; }
		inline auto& operator--() noexcept				{ --itr;			return *this; }
		inline auto& operator--(int) noexcept			{ --itr;			return *this; }
		inline auto& operator+=(int mOffset) noexcept	{ itr += mOffset;	return *this; }
		inline auto& operator-=(int mOffset) noexcept	{ itr -= mOffset;	return *this; }

		inline bool operator==(const BaseType& mRhs) const noexcept	{ return itr == mRhs.itr; }
		inline bool operator!=(const BaseType& mRhs) const noexcept	{ return itr != mRhs.itr; }
		inline bool operator<(const BaseType& mRhs) const noexcept	{ return itr < mRhs.itr; }
		inline bool operator>(const BaseType& mRhs) const noexcept	{ return itr > mRhs.itr; }
		inline bool operator<=(const BaseType& mRhs) const noexcept	{ return itr <= mRhs.itr; }
		inline bool operator>=(const BaseType& mRhs) const noexcept	{ return itr >= mRhs.itr; }
};

template<typename TBase, typename T, typename TItr, typename TPolicy> 
struct BaseAdaptorRandomItrCast : public BaseAdaptorRandomItr<TItr>
{
	inline BaseAdaptorRandomItrCast(TItr mItr) noexcept : BaseAdaptorRandomItr<TItr>{mItr} { }

	inline T& operator*() noexcept				{ return TPolicy::template getCasted<T&>(this->itr); }
	inline const T& operator*() const noexcept	{ return TPolicy::template getCasted<const T&>(this->itr); }
	inline T* operator->() noexcept				{ return &TPolicy::template getCasted<T&>(this->itr); }
	inline const T* operator->() const noexcept	{ return &TPolicy::template getCasted<const T&>(this->itr); }
};

struct CIA_PolicyUPtr
{
	template<typename TT, typename TItr> inline static TT getCasted(TItr mItr) noexcept 
	{ 
		return reinterpret_cast<TT>(**mItr); 
	}	
};

struct CIA_PolicyPtr
{
	template<typename TT, typename TItr> inline static TT getCasted(TItr mItr) noexcept 
	{ 
		return reinterpret_cast<TT>(*mItr); 
	}	
};

template<typename TBase, typename T, typename TItr> 
using CIA_UPtr = BaseAdaptorRandomItrCast<TBase, T, TItr, CIA_PolicyUPtr>;

template<typename TBase, typename T, typename TItr> 
using CIA_Ptr = BaseAdaptorRandomItrCast<TBase, T, TItr, CIA_PolicyUPtr>;

template<typename TBase, typename T, typename TItr> 
inline auto makeCIA_UPtrItr(TItr mItr) noexcept
{
	return CIA_UPtr<TBase, T, TItr>{mItr};
}

template<typename TBase, typename T, typename TItr> 
inline auto makeCIA_PtrItr(TItr mItr) noexcept
{
	return CIA_Ptr<TBase, T, TItr>{mItr};
}

template<typename TBase, typename T> 
struct MakerCIA_UPtrItr
{
	template<typename TItr> inline static auto make(TItr mItr) noexcept
	{
		return makeCIA_UPtrItr<TBase, T>(mItr);
	}
};

template<typename TBase, typename T> 
struct MakerCIA_PtrItr
{
	template<typename TItr> inline static auto make(TItr mItr) noexcept
	{
		return makeCIA_PtrItr<TBase, T>(mItr);
	}
};

template<typename TItr> class Range
{
	private:
		TItr itrBegin, itrEnd;

	public:
		inline constexpr Range(const TItr& mItrBegin, const TItr& mItrEnd) noexcept 
			: itrBegin{mItrBegin}, itrEnd{mItrEnd} { }

		inline constexpr TItr begin() const noexcept 	{ return itrBegin; }
		inline constexpr TItr end() const noexcept 		{ return itrEnd; }
};


template<typename TBase, typename T, typename TContainer, typename TMaker> 
inline auto makeCIARangeImpl(TContainer& mContainer) noexcept
{
	auto itrBegin(TMaker::make(std::begin(mContainer)));
	auto itrEnd(TMaker::make(std::end(mContainer)));

	return Range<decltype(itrBegin)>{itrBegin, itrEnd};
}

template<typename TBase, typename T, typename TContainer> 
inline auto makeCIA_UPtrRange(TContainer& mContainer) noexcept
{
	return makeCIARangeImpl<TBase, T, TContainer, MakerCIA_UPtrItr<TBase, T>>(mContainer);
}

template<typename TBase, typename T, typename TContainer> 
inline auto makeCIA_PtrRange(TContainer& mContainer) noexcept
{
	return makeCIARangeImpl<TBase, T, TContainer, MakerCIA_PtrItr<TBase, T>>(mContainer);
}

struct Base
{
	virtual ~Base() { }
	virtual void exec() { }
};

struct Der1 : public Base
{
	std::string a{"hi"};
	void exec() override { ssvu::lo() << a << "\n"; }
	void execNotOverr() { a = "bye"; }
};

int main() 
{
	{
		std::vector<ssvu::UPtr<Base>> v;
		v.emplace_back(ssvu::makeUPtr<Der1>());
		v.emplace_back(ssvu::makeUPtr<Der1>());
		v.emplace_back(ssvu::makeUPtr<Der1>());
		v.emplace_back(ssvu::makeUPtr<Der1>());	

		for(auto& i : makeCIA_UPtrRange<Base, Der1>(v)) i.execNotOverr();

		for(auto& i : v) i->exec();
	}

	{
		std::vector<Base*> v;
		v.emplace_back(new Der1);
		v.emplace_back(new Der1);
		v.emplace_back(new Der1);
		v.emplace_back(new Der1);

		for(auto& i : makeCIA_PtrRange<Base, Der1>(v)) i.execNotOverr();

		for(auto& i : v) i->exec();
	}

	return 0;
}
