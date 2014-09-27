// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SVJ_COMMON
#define SVJ_COMMON

#ifndef NDL
	#define DL(...) do { ssvu::lo("DEBUG") << __VA_ARGS__ << std::endl; } while(0)
#else
	#define DL(...) do { } while(0)
#endif


namespace svj
{
	using Idx = std::size_t;
	using Key = std::string;
	using String = std::string;
	template<typename T> using ObjectImpl = std::map<Key, T>;
	template<typename T> using ArrayImpl = std::vector<T>;

	struct Null { };
}

#endif
