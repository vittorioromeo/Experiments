#include <future>
#include <SSVUtils/Core/Core.hpp>
#include <SSVUtils/Benchmark/Benchmark.hpp>

template<typename TItr, typename TImpl> class BaseAdaptorForwardItr
{
	protected:
		TItr itr;	

	public:
		inline BaseAdaptorForwardItr(TItr mItr) noexcept : itr{mItr} { }

		inline decltype(auto) operator*() noexcept			{ return TImpl::get(itr); }
		inline decltype(auto) operator*() const noexcept	{ return TImpl::get(itr); }
		inline decltype(auto) operator->() noexcept			{ return &TImpl::get(itr); }
		inline decltype(auto) operator->() const noexcept	{ return &TImpl::get(itr); }

		inline auto& operator++() noexcept		{ ++this->itr; return *this; }
		inline auto& operator++(int) noexcept	{ ++this->itr; return *this; }
		
		inline bool operator==(const BaseAdaptorForwardItr& mRhs) const noexcept	{ return this->itr == mRhs.itr; }
		inline bool operator!=(const BaseAdaptorForwardItr& mRhs) const noexcept	{ return this->itr != mRhs.itr; }
};

template<typename TItr, typename TImpl> struct BaseAdaptorBidirectionalItr : public BaseAdaptorForwardItr<TItr, TImpl>
{
	inline BaseAdaptorBidirectionalItr(TItr mItr) noexcept : BaseAdaptorForwardItr<TItr, TImpl>{mItr} { }

	inline auto& operator--() noexcept		{ --this->itr; return *this; }
	inline auto& operator--(int) noexcept	{ --this->itr; return *this; }
};

template<typename TItr, typename TImpl> struct BaseAdaptorRandomItr : public BaseAdaptorBidirectionalItr<TItr, TImpl>
{
	inline BaseAdaptorRandomItr(TItr mItr) noexcept : BaseAdaptorBidirectionalItr<TItr, TImpl>{mItr} { }
	
	inline auto& operator+=(int mOffset) noexcept	{ this->itr += mOffset;	return *this; }
	inline auto& operator-=(int mOffset) noexcept	{ this->itr -= mOffset;	return *this; }

	inline bool operator<(const BaseAdaptorRandomItr& mRhs) const noexcept	{ return this->itr < mRhs.itr; }
	inline bool operator>(const BaseAdaptorRandomItr& mRhs) const noexcept	{ return this->itr > mRhs.itr; }
	inline bool operator<=(const BaseAdaptorRandomItr& mRhs) const noexcept	{ return this->itr <= mRhs.itr; }
	inline bool operator>=(const BaseAdaptorRandomItr& mRhs) const noexcept	{ return this->itr >= mRhs.itr; }
};

template<typename T> struct AdaptorCastImpl
{
	template<typename TItr> inline static T& get(TItr mItr) noexcept { return reinterpret_cast<T&>(**mItr);  }
};


template<typename TBase, typename T, typename TItr> using CIA_UPtr = BaseAdaptorRandomItr<TItr, AdaptorCastImpl<T>>;

template<typename TBase, typename T, typename TItr> inline auto makeCIA_UPtrItr(TItr mItr) noexcept
{
	return CIA_UPtr<TBase, T, TItr>{mItr};
}

template<typename TBase, typename T> struct MakerCIA_UPtrItr
{
	template<typename TItr> inline static auto make(TItr mItr) noexcept
	{
		return makeCIA_UPtrItr<TBase, T>(mItr);
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

template<typename TBase, typename T, typename TContainer> 
inline auto makeCIA_UPtrRange(TContainer& mContainer) noexcept
{
	auto itrBegin(makeCIA_UPtrItr<TBase, T>(std::begin(mContainer)));
	auto itrEnd(makeCIA_UPtrItr<TBase, T>(std::end(mContainer)));

	return Range<decltype(itrBegin)>{itrBegin, itrEnd};
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

		for(auto& i : makeCIA_UPtrRange<Base, Der1>(v)) i.execNotOverr();

		for(auto& i : v) i->exec();
	}

	return 0;
}
