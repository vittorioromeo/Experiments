#include <SSVUtils/Core/Core.hpp>

class Dictionary
{
	private:
		std::map<std::string, std::string> replacements; 
		std::map<std::string, std::vector<Dictionary>> sectionDictionaries;

		template<typename TKey> class Proxy
		{
			private:
				TKey key;
				Dictionary& dict;

			public:
				inline Proxy(TKey mKey, Dictionary& mDict) noexcept : key{mKey}, dict{mDict} { }

				template<typename T> inline auto& operator=(T&& mReplacement) { dict.replacements[ssvu::fwd<TKey>(key)] = ssvu::fwd<T>(mReplacement); return *this; }

				inline auto& operator+=(const Dictionary& mDict)	{ dict.sectionDictionaries[ssvu::fwd<TKey>(key)].emplace_back(mDict); return *this; }
				inline auto& operator+=(Dictionary&& mDict)			{ dict.sectionDictionaries[ssvu::fwd<TKey>(key)].emplace_back(std::move(mDict)); return *this; }
		};

		inline void initImpl(const std::string& mKey,	const std::string& mReplacement)	{ replacements[mKey] 			= mReplacement; }
		inline void initImpl(std::string&& mKey,		const std::string& mReplacement)	{ replacements[std::move(mKey)]	= mReplacement; }
		inline void initImpl(const std::string& mKey,	std::string&& mReplacement)			{ replacements[mKey]			= std::move(mReplacement); }
		inline void initImpl(std::string&& mKey,		std::string&& mReplacement)			{ replacements[std::move(mKey)]	= std::move(mReplacement); }

		// WTF
		//inline void initImpl(const std::string& mKey,	const std::vector<Dictionary>& mDicts)	{ sectionDictionaries[mKey]				= mDicts; }
		//inline void initImpl(std::string&& mKey,		const std::vector<Dictionary>& mDicts)	{ sectionDictionaries[std::move(mKey)]	= mDicts; }
		inline void initImpl(const std::string& mKey,	std::vector<Dictionary>&& mDicts)		{ sectionDictionaries[mKey]				= std::move(mDicts); }
		inline void initImpl(std::string&& mKey,		std::vector<Dictionary>&& mDicts)		{ sectionDictionaries[std::move(mKey)]	= std::move(mDicts); }

		inline void init() { }
		template<typename T1, typename T2, typename... TArgs> inline void init(T1&& mA1, T2&& mA2, TArgs&&... mArgs)
		{
			initImpl(ssvu::fwd<T1>(mA1), ssvu::fwd<T2>(mA2)); init(ssvu::fwd<TArgs>(mArgs)...);
		}

		inline std::string getExpandedImpl(const std::string& mSrc, std::size_t mIBegin, std::size_t mIEnd) const
		{
			using namespace ssvu;
			// TODO: buffer in class
			std::string result;
			//result.reserve(mSrc.size() * 2);

			auto idx(mIBegin);
			auto getC([&mSrc, &idx]{ return mSrc[idx]; });
			auto getCX([&mSrc](auto mIdx){ return mSrc[mIdx]; });

			for(; idx < mIEnd; ++idx)
			{
				//lo("R") << result << "\n";

				// Skip non-special characters or escaped special characters
				if(getC() != '{' || (idx > 0 && getCX(idx - 1) == '\\')) { result += getC(); continue; }

				// "{{" combination
				if(getCX(idx + 1) == '{')
				{
					// Skip "{{"
					idx += 2;

					bool section{false};

					// Section start
					if(getC() == '#')
					{
						section = true;

						// Skip '#'
						++idx;
					}

					// TODO: buffer in class
					std::string key;

					for(; getC() != '}'; ++idx) key += getC();

					SSVU_ASSERT(getC() == '}' && getCX(idx + 1) == '}');

					// Not a section
					if(!section)
					{
						// Skip first '}', second one will be skipped by the for's `idx` increment
						++idx;

						result += replacements.at(key);
						continue;
					}

					// Section

					// Skip "}}"
					idx += 2;

					auto startIdx(idx), endIdx(idx);

					//lo() << "found section\n";
					//lo() << "key: " << key << "\n";

					// Find section end
					for(; true; ++idx)
					{
						// Skip non-special characters or escaped special characters
						if(getC() != '{') continue;
						if(getCX(idx - 1) == '\\') continue;

						endIdx = idx;

						if(getCX(idx + 1) == '{' && getCX(idx + 2) == '/')
						{
							idx += 3;

							//lo() << "checking match\n";

							// Check if section key matches
							for(auto kc : key)
							{
								if(getC() != kc) goto next;
								++idx;
							}

							//lo() << "matches\n";

							SSVU_ASSERT(getC() == '}' && getCX(idx + 1) == '}');

							// Skip first '}', second one will be skipped by the for's `idx` increment
							++idx;
							break;
						}

						next:
						continue;
					}

					// BUG TODO : uses dic's sectionDictionaries - correct? re-think design;
					for(const auto& dic : sectionDictionaries.at(key))
					{
						//lo() << mSrc.substr(startIdx, endIdx - startIdx) << "\n";
						result += dic.getExpandedImpl(mSrc, startIdx, endIdx);
					}
				}
			}

			return result;
		}

	public:
		inline Dictionary() = default;
		inline Dictionary(const Dictionary&) = default;
		inline Dictionary(Dictionary&&) = default;

		inline Dictionary& operator=(const Dictionary&) = default;
		inline Dictionary& operator=(Dictionary&&) = default;

		template<typename... TArgs> inline Dictionary(TArgs&&... mArgs) { init(ssvu::fwd<TArgs>(mArgs)...); }

		inline std::string getExpanded(const std::string& mSrc) const { return getExpandedImpl(mSrc, 0, mSrc.size()); }

		template<typename T> inline auto operator[](T&& mKey) noexcept { return Proxy<T>{ssvu::fwd<T>(mKey), *this}; }
};


int main()
{
	std::string src{R"(
		<h1> {{title}} </h1>
		<ul>
			{{#friends}}
				<li>{{name}}</li>
				<li>{{job}}</li>
				<li>{{status}}</li>
				{{#adjs}}
					<li>{{adj}}</li>
				{{/adjs}}
			{{/friends}}
		</ul>
	)"};	

	Dictionary base;
	base["title"] = "My friends";

	base["friends"] += 
	{
		"name", "Jim",	
		"job", "Unemployed",	
		"status", "Married",
		"adjs", std::vector<Dictionary>
				{				
					{"adj", "cool"}, 
					{"adj", "big"},
					{"adj", "strong"}
				}
	};

	base["friends"] += 
	{
		"name", "Vayne",	
		"job", "Night hunter",	
		"status", "Stealthed",
		"adjs", std::vector<Dictionary>
				{				
					{"adj", "fast"}, 
					{"adj", "op"}
				}
	};

	

	ssvu::lo() << base.getExpanded(src) << "\n";
	return 0;
}