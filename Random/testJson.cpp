#include <SSVUtils/Core/Core.hpp>

namespace sjt
{
	class Value;

	template<typename T, typename TDeleter = std::default_delete<T>> using UPtr = ssvu::UPtr<T, TDeleter>;
	using Idx = std::size_t;
	using ObjMap = std::unordered_map<std::string, UPtr<Value>>;
	using Array = std::vector<UPtr<Value>>;

	struct Null { };



	template<typename T> struct NumberHelper;

	class Number
	{
		template<typename T> friend struct NumberHelper;

		public:
			enum class Type{IntS, IntU, Real};
			
			using IntS = int;
			using IntU = unsigned int;
			using Real = double;

		private:
			Type type;

			union Holder
			{
				IntS hIntS;
				IntU hIntU;
				Real hReal;
			} holder;

			inline void setIntS(IntU mValue) noexcept { type = Type::IntS; holder.hIntS = mValue; }
			inline void setIntU(IntS mValue) noexcept { type = Type::IntU; holder.hIntU = mValue; }
			inline void setReal(Real mValue) noexcept { type = Type::Real; holder.hReal = mValue; }

			inline IntS getIntS() const noexcept
			{
				switch(type)
				{
					case Type::IntS: return holder.hIntS;
					case Type::IntU: return static_cast<IntS>(holder.hIntU);
					case Type::Real: return static_cast<IntS>(holder.hReal);
				}
			}

			inline IntU getIntU() const noexcept
			{
				switch(type)
				{
					case Type::IntS: return static_cast<IntU>(holder.hIntS);
					case Type::IntU: return holder.hIntU;
					case Type::Real: return static_cast<IntU>(holder.hReal);
				}
			}

			inline Real getReal() const noexcept
			{
				switch(type)
				{
					case Type::IntS: return static_cast<Real>(holder.hIntS);
					case Type::IntU: return static_cast<Real>(holder.hIntU);
					case Type::Real: return holder.hReal;
				}
			}

		public:
			inline Number(IntS mValue) noexcept { setIntS(mValue); }
			inline Number(IntU mValue) noexcept { setIntU(mValue); }
			inline Number(Real mValue) noexcept { setReal(mValue); }

			template<typename T> void set(T mValue) noexcept { return NumberHelper<T>::set(*this, mValue); }
			template<typename T> auto get() const noexcept { return NumberHelper<T>::get(*this); }			
	};

	template<> struct NumberHelper<char>
	{
		inline static void set(Number& mN, char mV) noexcept { return mN.setIntS(mV); }
		inline static auto get(const Number& mN) noexcept { return mN.getIntS(); }
	};

	template<> struct NumberHelper<unsigned char>
	{
		inline static void set(Number& mN, unsigned char mV) noexcept { return mN.setIntU(mV); }
		inline static auto get(const Number& mN) noexcept { return mN.getIntU(); }
	};

	template<> struct NumberHelper<int>
	{
		inline static void set(Number& mN, int mV) noexcept { return mN.setIntS(mV); }
		inline static auto get(const Number& mN) noexcept { return mN.getIntS(); }
	};

	template<> struct NumberHelper<unsigned int>
	{
		inline static void set(Number& mN, unsigned int mV) noexcept { return mN.setIntU(mV); }
		inline static auto get(const Number& mN) noexcept { return mN.getIntU(); }
	};

	template<> struct NumberHelper<float>
	{
		inline static void set(Number& mN, float mV) noexcept { return mN.setReal(mV); }
		inline static auto get(const Number& mN) noexcept { return mN.getReal(); }
	};

	template<> struct NumberHelper<double>
	{
		inline static void set(Number& mN, double mV) noexcept { return mN.setReal(mV); }
		inline static auto get(const Number& mN) noexcept { return mN.getReal(); }
	};




	template<typename T> struct ValueHelper { };

	class Value
	{
		public:
			enum class Type{Object, Array, Number, String, Bool, Null};

		protected:
			Type type;
			inline Value(Type mType) noexcept : type{mType} { }

		public:
			inline ~Value() { }

			template<typename T> inline decltype(auto) get() const { return ValueHelper<T>::get(*this); }
			template<typename T> inline decltype(auto) get(const std::string& mKey) const { return ValueHelper<T>::get((*this)[mKey]); }

			const Value& operator[](const std::string& mKey) const; 
	};

	struct ValueObject : public Value
	{
		ObjMap value;
		inline ValueObject(ObjMap mValue) : Value{Value::Type::Object}, value{std::move(mValue)} { }
	};

	struct ValueArray : public Value
	{
		Array value;
		inline ValueArray(Array mValue) : Value{Value::Type::Array}, value{std::move(mValue)} { }
	};

	struct ValueNumber : public Value
	{
		Number value;
		inline ValueNumber(const Number& mValue) : Value{Value::Type::Number}, value{mValue} { }
	};

	struct ValueString : public Value
	{
		std::string value;
		inline ValueString(const std::string& mValue) : Value{Value::Type::String}, value{mValue} { }
	};

	struct ValueBool : public Value
	{
		bool value;
		inline ValueBool(const bool& mValue) : Value{Value::Type::Bool}, value{mValue} { }
	};

	struct ValueNull : public Value
	{
		inline ValueNull() : Value{Value::Type::Null} { }
	};

	template<> struct ValueHelper<int>
	{
		inline static auto get(const Value& mValue) { return reinterpret_cast<const ValueNumber&>(mValue).value.get<int>(); }
	};

	template<> struct ValueHelper<float>
	{
		inline static auto get(const Value& mValue) { return reinterpret_cast<const ValueNumber&>(mValue).value.get<float>(); }
	};

	template<> struct ValueHelper<double>
	{
		inline static auto get(const Value& mValue) { return reinterpret_cast<const ValueNumber&>(mValue).value.get<double>(); }
	};

	template<> struct ValueHelper<bool>
	{
		inline static const auto& get(const Value& mValue) { return reinterpret_cast<const ValueBool&>(mValue).value; }
	};

	template<> struct ValueHelper<std::string>
	{
		inline static const auto& get(const Value& mValue) { return reinterpret_cast<const ValueString&>(mValue).value; }
	};

	template<> struct ValueHelper<Array>
	{
		inline static const auto& get(const Value& mValue) { return reinterpret_cast<const ValueArray&>(mValue).value; }
	};

	template<> struct ValueHelper<ObjMap>
	{
		inline static const auto& get(const Value& mValue) { return reinterpret_cast<const ValueObject&>(mValue).value; }
	};

	inline const Value& Value::operator[](const std::string& mKey) const
	{ 
		return *get<ObjMap>().at(mKey);
	}


	inline bool isWhitespace(char mC) noexcept
	{
		return mC == ' ' || mC == '\t' || mC == '\r' || mC == '\n';
	}

	inline bool isNumberStart(char mC) noexcept
	{
		return mC == '-' || ssvu::isDigit(mC);
	}

	inline void validateKeyword(const std::string& mSrc, Idx& mIdx, const std::string& mKeyword)
	{
		for(auto kc : mKeyword)
		{
			if(mSrc[mIdx] != kc) throw std::runtime_error{"Invalid keyword " + mKeyword};
			++mIdx;
		}
	}

	inline auto readStr(const std::string& mSrc, Idx& mIdx)
	{
		std::string str;
		++mIdx;

		for(; true; ++mIdx)
		{
			auto sc(mSrc[mIdx]);
			if(sc == '\\')
			{
				++mIdx;
				sc = mSrc[mIdx];

				if(sc == '"') str += '\"';
				else if(sc == '\\') str += '\\';
				else if(sc == '/') str += '/';
				else if(sc == 'b') str += '\b';
				else if(sc == 'f') str += '\f';
				else if(sc == 'n') str += '\n';
				else if(sc == 'r') str += '\r';
				else if(sc == 't') str += '\t';
			}
			else if(sc == '"') break;
			
			str += sc;
		}

		++mIdx;

		return str;
	}

	inline std::string getPurgedFromComments(const std::string& mSrc)
	{
		std::string result, buffer;

		auto commit([&result, &buffer]
		{
			result += buffer + "\n";
			buffer.clear();
		});

		for(auto i(0u); i < mSrc.size(); ++i)
		{
			if(mSrc[i] == '\n')
			{
				commit();
			}
			else if(mSrc[i] == '/' && mSrc[i + 1] == '/')
			{
				commit();

				while(mSrc[i] != '\n') ++i;
				++i;
			}
			else
			{
				buffer += mSrc[i];
			}
		}

		return result;
	}

	UPtr<Value> parseValue(const std::string& mSrc, Idx& mIdx);

	inline UPtr<Value> parseNull(const std::string& mSrc, Idx& mIdx)
	{
		validateKeyword(mSrc, mIdx, "null");
		return ssvu::makeUPtr<ValueNull>();
	}

	inline UPtr<Value> parseBoolFalse(const std::string& mSrc, Idx& mIdx)
	{
		validateKeyword(mSrc, mIdx, "false");
		return ssvu::makeUPtr<ValueBool>(false);
	}

	inline UPtr<Value> parseBoolTrue(const std::string& mSrc, Idx& mIdx)
	{
		validateKeyword(mSrc, mIdx, "true");
		return ssvu::makeUPtr<ValueBool>(true);
	}	

	inline UPtr<Value> parseNumber(const std::string& mSrc, Idx& mIdx)
	{
		bool negative{false};
		std::string strNum;
		std::size_t cntDInt{0u}, cntDDec{0u};

		if(mSrc[mIdx] == '-') { ++mIdx; negative = true; }
		
		while(ssvu::isDigit(mSrc[mIdx])) { strNum += mSrc[mIdx]; ++mIdx; ++cntDInt; }
		
		if(mSrc[mIdx] != '.') 
		{
			//if(std::numeric_limits<int>::digits <= cntDInt)
				return ssvu::makeUPtr<ValueNumber>(Number{std::stoi(strNum)});
		}

		strNum += '.';
		
		++mIdx;
		while(ssvu::isDigit(mSrc[mIdx])) { strNum += mSrc[mIdx]; ++mIdx; ++cntDDec; }	

		if(mSrc[mIdx] == 'e' || mSrc[mIdx] == 'E') 
		{
			strNum += 'e';

			++mIdx;
			if(mSrc[mIdx] == '+') strNum += '+';
			if(mSrc[mIdx] == '-') strNum += '-';

			++mIdx;
			while(ssvu::isDigit(mSrc[mIdx])) { strNum += mSrc[mIdx]; ++mIdx; }	
		}

		
		if(std::numeric_limits<float>::digits10 <= cntDDec)
			return ssvu::makeUPtr<ValueNumber>(Number{std::stof(strNum)});
		//else if(std::numeric_limits<double>::digits10 <= cntDDec)
			return ssvu::makeUPtr<ValueNumber>(Number{std::stod(strNum)});	
	}

	inline UPtr<Value> parseString(const std::string& mSrc, Idx& mIdx)
	{
		return ssvu::makeUPtr<ValueString>(readStr(mSrc, mIdx));
	}

	inline UPtr<Value> parseArray(const std::string& mSrc, Idx& mIdx)
	{
		++mIdx;
		while(isWhitespace(mSrc[mIdx])) ++mIdx;

		Array array;

		if(mSrc[mIdx] != ']')	
		{
			while(true)
			{
				while(isWhitespace(mSrc[mIdx])) ++mIdx;

				array.emplace_back(parseValue(mSrc, mIdx));

				while(isWhitespace(mSrc[mIdx])) ++mIdx;

				if(mSrc[mIdx] == ',') { ++mIdx; continue; }
				if(mSrc[mIdx] == ']') break;

				throw std::runtime_error{"Invalid array"};
			}
		}

		++mIdx;

		return ssvu::makeUPtr<ValueArray>(std::move(array));
	}

	inline UPtr<Value> parseObject(const std::string& mSrc, Idx& mIdx)
	{
		++mIdx;
		while(isWhitespace(mSrc[mIdx])) ++mIdx;

		ObjMap objMap;		

		if(mSrc[mIdx] != '}')
		{
			while(true)
			{			
				while(isWhitespace(mSrc[mIdx])) ++mIdx;

				if(mSrc[mIdx] != '"') throw std::runtime_error{"Invalid object"};
				
				auto key(readStr(mSrc, mIdx));
				
				while(isWhitespace(mSrc[mIdx])) ++mIdx;

				if(mSrc[mIdx] != ':') throw std::runtime_error{"Invalid object"};

				++mIdx;

				while(isWhitespace(mSrc[mIdx])) ++mIdx;

				objMap[key] = parseValue(mSrc, mIdx);
					
				while(isWhitespace(mSrc[mIdx])) ++mIdx;

				if(mSrc[mIdx] == ',') { ++mIdx; continue; }
				if(mSrc[mIdx] == '}') break;

				throw std::runtime_error{"Invalid object"};
			}
		}

		++mIdx;

		return ssvu::makeUPtr<ValueObject>(std::move(objMap));
	}

	inline UPtr<Value> parseValue(const std::string& mSrc, Idx& mIdx)
	{
		while(isWhitespace(mSrc[mIdx])) ++mIdx;

		if(mSrc[mIdx] == '{')			return parseObject(mSrc, mIdx);
		if(mSrc[mIdx] == '[')			return parseArray(mSrc, mIdx);
		if(mSrc[mIdx] == '"')			return parseString(mSrc, mIdx);
		if(mSrc[mIdx] == 't')			return parseBoolTrue(mSrc, mIdx);
		if(mSrc[mIdx] == 'f')			return parseBoolFalse(mSrc, mIdx);
		if(mSrc[mIdx] == 'n')			return parseNull(mSrc, mIdx);
		if(isNumberStart(mSrc[mIdx]))	return parseNumber(mSrc, mIdx);

		throw std::runtime_error{"Invalid value"};
	}

	inline UPtr<Value> parseDocument(const std::string& mSrc)
	{	
		auto purgedString(getPurgedFromComments(mSrc));

		Idx idx{0u};
		while(isWhitespace(mSrc[idx])) ++idx;

		if(mSrc[idx] == '{') return parseObject(purgedString, idx);
		if(mSrc[idx] == '[') return parseArray(purgedString, idx);

		throw std::runtime_error{"Invalid document"};
	}
}

int main()
{
	auto document(sjt::parseDocument(R"(

	{
		"n1": 10,
		"n2": 15.5,
		"n3": -35.5e+12,
		
		"s1": "bananas",	// Test comment
		"s2": "",
		"s3": "1test2",

		"a1": [1, 2, 3, "sup", { "lol":10 }], // Comment 2

		"o1": 
		{
			"b": true
		},

		"o2": 
		{
			"b": false
		},

		"o3":
		{
			"nv": null
		}
	}

	)"));

	using namespace ssvu;

	const auto& om = document->get<sjt::ObjMap>();
	for(const auto& p : om) ssvu::lo() << p.first << " -> " << "?" << "\n";

	const auto& om_01 = om.at("o1")->get<sjt::ObjMap>();
	for(const auto& p : om_01) ssvu::lo() << p.first << " -> " << "?" << "\n";

	lo() << (*document)["n1"].get<int>() << "\n";
	lo() << (*document).get<int>("n1") << "\n";
	lo() << om.at("n1")->get<int>() << "\n";
	lo() << om.at("n1")->get<float>() << "\n";
	lo() << om.at("n1")->get<double>() << "\n";

	return 0;
}