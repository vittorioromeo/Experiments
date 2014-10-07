#include <SSVUtils/Core/Core.hpp>

class Dictionary;

class Expander
{
	private:
		const Dictionary& dict;
		const std::string& src;
		std::string& bufferResult;
		std::string& bufferKey;
		const std::size_t idxBegin, idxEnd;
		std::size_t idx;
		bool addSeparator;

		inline auto getC() const noexcept 					{ return src[idx]; }
		inline auto getC(std::size_t mIdx) const noexcept 	{ return src[mIdx]; }

	public:
		inline Expander(const Dictionary& mDict, const std::string& mSrc, std::string& mBufferResult, std::string& mBufferKey, std::size_t mIdxBegin, std::size_t mIdxEnd, bool mAddSeparator) 
			: dict{mDict}, src{mSrc}, bufferResult{mBufferResult}, bufferKey{mBufferKey}, idxBegin{mIdxBegin}, idxEnd{mIdxEnd}, idx{idxBegin}, addSeparator{mAddSeparator}
		{ 
			
		}

		void expand();
};

class Dictionary
{
	friend class Expander;

	private:
		std::map<std::string, std::string> replacements; 
		std::map<std::string, std::vector<Dictionary>> sectionDictionaries;

		template<typename TKey> class Proxy
		{
			private:
				TKey key;
				Dictionary& dict;

			public:
				inline Proxy(TKey mKey, Dictionary& mDict) : key{mKey}, dict{mDict} { }

				template<typename T> inline auto& operator=(T&& mReplacement) { dict.replacements[ssvu::fwd<TKey>(key)] = ssvu::fwd<T>(mReplacement); return *this; }

				inline auto& operator+=(const Dictionary& mDict)	{ dict.sectionDictionaries[ssvu::fwd<TKey>(key)].emplace_back(mDict); return *this; }
				inline auto& operator+=(Dictionary&& mDict)			{ dict.sectionDictionaries[ssvu::fwd<TKey>(key)].emplace_back(std::move(mDict)); return *this; }
		};

		// Init single replacement 
		inline void initImpl(const std::string& mKey,	const std::string& mReplacement)	{ replacements[mKey] 			= mReplacement; }
		inline void initImpl(std::string&& mKey,		const std::string& mReplacement)	{ replacements[std::move(mKey)]	= mReplacement; }
		inline void initImpl(const std::string& mKey,	std::string&& mReplacement)			{ replacements[mKey]			= std::move(mReplacement); }
		inline void initImpl(std::string&& mKey,		std::string&& mReplacement)			{ replacements[std::move(mKey)]	= std::move(mReplacement); }

		// Init section replacement
		inline void initImpl(const std::string& mKey,	const std::vector<Dictionary>& mDicts)	{ sectionDictionaries[mKey]				= mDicts; }
		inline void initImpl(std::string&& mKey,		const std::vector<Dictionary>& mDicts)	{ sectionDictionaries[std::move(mKey)]	= mDicts; }
		inline void initImpl(const std::string& mKey,	std::vector<Dictionary>&& mDicts)		{ sectionDictionaries[mKey]				= std::move(mDicts); }
		inline void initImpl(std::string&& mKey,		std::vector<Dictionary>&& mDicts)		{ sectionDictionaries[std::move(mKey)]	= std::move(mDicts); }

		// Copy/move init
		inline void initImpl(const Dictionary& mDict) 
		{ 
			replacements = mDict.replacements;
			sectionDictionaries = mDict.sectionDictionaries;
		}
		inline void initImpl(Dictionary&& mDict) noexcept 
		{ 
			replacements = std::move(mDict.replacements); 
			sectionDictionaries = std::move(mDict.sectionDictionaries);
		}

		inline void init() noexcept { }
		template<typename T1, typename... TArgs> inline void init(T1&& mA1, TArgs&&... mArgs)
		{
			initImpl(ssvu::fwd<T1>(mA1)); init(ssvu::fwd<TArgs>(mArgs)...);
		}
		template<typename T1, typename T2, typename... TArgs> inline void init(T1&& mA1, T2&& mA2, TArgs&&... mArgs)
		{
			initImpl(ssvu::fwd<T1>(mA1), ssvu::fwd<T2>(mA2)); init(ssvu::fwd<TArgs>(mArgs)...);
		}

	public:	
		template<typename... TArgs> inline Dictionary(TArgs&&... mArgs) { init(ssvu::fwd<TArgs>(mArgs)...); }

		inline std::string getExpanded(const std::string& mSrc) const 
		{ 
			std::string result; result.reserve(mSrc.size() * 2);
			std::string bufferKey; bufferKey.reserve(10);
			
			Expander e{*this, mSrc, result, bufferKey, 0, mSrc.size(), 0}; 
			e.expand();
			
			return result;
		}

		template<typename T> inline auto operator[](T&& mKey) noexcept { return Proxy<T>{ssvu::fwd<T>(mKey), *this}; }
};

inline void Expander::expand()
{
	enum class Type{Normal, Section, Separator};
	using namespace ssvu;			

	for(; idx < idxEnd; ++idx)
	{
		// Skip non-special characters or escaped special characters
		if(getC() != '{' || (idx > 0 && getC(idx - 1) == '\\')) { bufferResult += getC(); continue; }

		// "{{" combination
		if(getC(idx + 1) == '{')
		{
			// Skip "{{"
			idx += 2;

			Type type{Type::Normal};

			// Section start
			if(getC() == '#')
			{
				type = Type::Section;

				// Skip '#'
				++idx;
			}
			else if(getC() == '*')
			{
				type = Type::Separator;

				// Skip '['
				++idx;
			}

			bufferKey.clear();

			for(; getC() != '}'; ++idx) bufferKey += getC();

			SSVU_ASSERT(getC() == '}' && getC(idx + 1) == '}');

			// Not a section
			if(type == Type::Normal)
			{
				// Skip first '}', second one will be skipped by the for's `idx` increment
				++idx;

				bufferResult += dict.replacements.at(bufferKey);
				continue;
			}
			else if(type == Type::Separator)
			{
				// Skip first '}', second one will be skipped by the for's `idx` increment
				++idx;

				if(addSeparator) bufferResult += bufferKey;
				continue;
			}
			else if(type == Type::Section)
			{
				// Skip "}}"
				idx += 2;

				auto sectIdxStart(idx), sectIdxEnd(idx);

				// Find section end
				for(; true; ++idx)
				{
					// Skip non-special characters or escaped special characters
					if(getC() != '{') continue;
					if(getC(idx - 1) == '\\') continue;

					sectIdxEnd = idx;

					if(getC(idx + 1) == '{' && getC(idx + 2) == '/')
					{
						idx += 3;

						// Check if section key matches
						for(auto kc : bufferKey)
						{
							if(getC() != kc) goto next;
							++idx;
						}

						SSVU_ASSERT(getC() == '}' && getC(idx + 1) == '}');

						// Skip first '}', second one will be skipped by the for's `idx` increment
						++idx;
						break;
					}

					next:
					continue;
				}

				// BUG TODO : uses dic's sectionDictionaries - correct? re-think design;
				//for(const auto& dic : dict.sectionDictionaries.at(bufferKey))
				auto& dictVec(dict.sectionDictionaries.at(bufferKey));
				auto sectCount(dictVec.size());
				for(auto i(0u); i < sectCount; ++i)
				{
					Expander e{dictVec[i], src, bufferResult, bufferKey, sectIdxStart, sectIdxEnd, i < sectCount - 1}; 
					e.expand();
				}
			}
		}
	}
}


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
					<li>{{adj}}</li>{{* and }}
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

	std::vector<Dictionary> adjsLucian
	{
		{"adj", "powerful"},
		{"adj", "cool"}
	};

	Dictionary lucian
	{
		"name", "Lucian",
		"job", "Being op",
		"status", "Nerfed",
		"adjs", adjsLucian
	};

	base["friends"] += lucian;
	

	ssvu::lo() << base.getExpanded(src) << "\n";
	return 0;
}