// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SVJ_COMMON_VECMAP
#define SVJ_COMMON_VECMAP

namespace ssvu
{
	namespace Json
	{
		namespace Internal
		{
			template<typename TK, typename TV> class VecMap
			{
				public:
					using Item = std::pair<TK, TV>;

				private:
					std::vector<Item> data;

				public:
					inline VecMap() { data.reserve(10); }
					inline VecMap(const VecMap& mVM) : data{mVM.data} { }
					inline VecMap(VecMap&& mVM) : data{std::move(mVM.data)} { }
					inline VecMap(std::initializer_list<Item>&& mIL)
					{
						data.reserve(mIL.size());
						for(auto& i : mIL) data.emplace_back(std::move(i));
						sort(data, [](const auto& mA, const auto& mB){ return mA.first < mB.first; });
					}

					inline auto& operator[](const TK& mKey)
					{
						auto itr(lowerBound(data, mKey, [](const auto& mA, const auto& mB){ return mA.first < mB; }));
						if(itr != std::end(data) && itr->first == mKey) return itr->second;

						return data.emplace(itr, mKey, TV{})->second;
					}
					inline const auto& at(const TK& mKey) const
					{
						auto itr(lowerBound(data, mKey, [](const auto& mA, const auto& mB){ return mA.first < mB; }));
						if(itr != std::end(data) && itr->first == mKey) return itr->second;

						throw std::out_of_range{""};
					}

					inline auto operator==(const VecMap& mVM) const noexcept { return data == mVM.data; }
					inline auto operator!=(const VecMap& mVM) const noexcept { return !(operator==(mVM)); }

					inline auto begin()		noexcept		{ return std::begin(data); }
					inline auto end()		noexcept		{ return std::end(data); }
					inline auto begin()		const noexcept	{ return std::begin(data); }
					inline auto end()		const noexcept	{ return std::end(data); }
			};
		}
	}
}

#endif
