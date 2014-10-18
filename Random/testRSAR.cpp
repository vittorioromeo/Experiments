#include <SSVUtils/Core/Core.hpp>

namespace ssvu
{
	template<typename T> struct ResizableArray
	{
		using TStorage = AlignedStorageBasic<T>;
		TStorage* data{nullptr};

		inline ~ResizableArray() { delete data; }

		inline void resize(SizeT mSizeOld, SizeT mSizeNew)
		{
			auto newData(new TStorage[mSizeNew]);
			for(auto i(0u); i < mSizeOld; ++i) newData[i] = std::move(data[i]);

			delete data;
			data = newData;
		}

		inline auto& operator[](SizeT mIdx) noexcept { return reinterpret_cast<T&>(data[mIdx]); }
		inline const auto& operator[](SizeT mIdx) const noexcept { return reinterpret_cast<const T&>(data[mIdx]); }
	};
}

int main() 
{
	using namespace ssvu;

	ResizableArray<int> a;
	a.resize(0, 10);
	a[5] = 100;
	lo() << a[5] << "\n";
	a.resize(10, 25);	
	lo() << a[5] << "\n";

	return 0;
}