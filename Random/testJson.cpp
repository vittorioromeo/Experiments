#include <SSVUtils/Core/Core.hpp>

namespace sjt
{
	enum class Tkn : std::size_t
	{
		BracketCurlyLeft,
		BracketCurlyRight,
		BracketSquareLeft,
		BracketSquareRight,
		BoolTrue,
		BoolFalse,
		Colon,
		Comma,
		Null,
		Whitespace
	};

	class Value;

	template<typename T, typename TDeleter = std::default_delete<T>> using UPtr = ssvu::UPtr<T, TDeleter>;
	using Idx = std::size_t;
	using ObjMap = std::unordered_map<std::string, UPtr<Value>>;
	using Array = std::vector<UPtr<Value>>;

	struct Null { };

	using Number = double;

	/*class Number
	{
		public:
			enum class Type{Int, Float, Double};

		protected:			
			Type type;
			inline Number(Type mType) noexcept : type{mType} { }

		public:
			inline virtual ~Number() { }
	};

	struct NumberInt : public Number
	{
		int value;
		inline NumberInt(int mValue) noexcept : Number{Number::Type::Int}, value{mValue} { }
	};	

	struct NumberFloat : public Number
	{
		float value;
		inline NumberFloat(float mValue) noexcept : Number{Number::Type::Float}, value{mValue} { }
	};	

	struct NumberDouble : public Number
	{
		double value;
		inline NumberDouble(double mValue) noexcept : Number{Number::Type::Double}, value{mValue} { }
	};*/	

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

			template<typename T> inline auto& get() { return ValueHelper<T>::get(*this); }
			template<typename T> inline const auto& get() const { return ValueHelper<T>::get(*this); }

			Value& operator[](const std::string& mKey); 
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



	template<> struct ValueHelper<double>
	{
		inline static auto& get(Value& mValue) { return reinterpret_cast<ValueNumber&>(mValue).value; }
	};

	template<> struct ValueHelper<bool>
	{
		inline static auto& get(Value& mValue) { return reinterpret_cast<ValueBool&>(mValue).value; }
	};

	template<> struct ValueHelper<std::string>
	{
		inline static auto& get(Value& mValue) { return reinterpret_cast<ValueString&>(mValue).value; }
	};

	template<> struct ValueHelper<Array>
	{
		inline static auto& get(Value& mValue) { return reinterpret_cast<ValueArray&>(mValue).value; }
	};

	template<> struct ValueHelper<ObjMap>
	{
		inline static auto& get(Value& mValue) { return reinterpret_cast<ValueObject&>(mValue).value; }
	};

	inline Value& Value::operator[](const std::string& mKey)
	{ 
		return *get<ObjMap>()[mKey];
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

	inline auto readNumber(const std::string& mSrc, Idx& mIdx)
	{
		bool negative{false};
		std::string strNum;

		if(mSrc[mIdx] == '-') { ++mIdx; negative = true; }
		
		
		while(ssvu::isDigit(mSrc[mIdx])) { strNum += mSrc[mIdx]; ++mIdx; }
		
		if(mSrc[mIdx] != '.') return std::stod(strNum);

		strNum += '.';
		
		++mIdx;
		while(ssvu::isDigit(mSrc[mIdx])) { strNum += mSrc[mIdx]; ++mIdx; }	

		if(mSrc[mIdx] != 'e' && mSrc[mIdx] != 'E') return std::stod(strNum);
		strNum += 'e';

		++mIdx;
		if(mSrc[mIdx] == '+') strNum += '+';
		if(mSrc[mIdx] == '-') strNum += '-';

		++mIdx;
		while(ssvu::isDigit(mSrc[mIdx])) { strNum += mSrc[mIdx]; ++mIdx; }	

		return std::stod(strNum);
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



	UPtr<Value> parseNull(const std::string& mSrc, Idx& mIdx);
	UPtr<Value> parseBoolFalse(const std::string& mSrc, Idx& mIdx);
	UPtr<Value> parseBoolTrue(const std::string& mSrc, Idx& mIdx);
	UPtr<Value> parseNumber(const std::string& mSrc, Idx& mIdx);
	UPtr<Value> parseString(const std::string& mSrc, Idx& mIdx);
	UPtr<Value> parseArray(const std::string& mSrc, Idx& mIdx);
	UPtr<Value> parseObject(const std::string& mSrc, Idx& mIdx);
	UPtr<Value> parseValue(const std::string& mSrc, Idx& mIdx);
	UPtr<Value> parseDocument(const std::string& mSrc);

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
		return ssvu::makeUPtr<ValueNumber>(readNumber(mSrc, mIdx));
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
		Idx idx{0u};
		while(isWhitespace(mSrc[idx])) ++idx;

		if(mSrc[idx] == '{') return parseObject(mSrc, idx);
		if(mSrc[idx] == '[') return parseArray(mSrc, idx);

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
		
		"s1": "bananas",
		"s2": "",
		"s3": "1test2",

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

	sjt::ObjMap& om = document->get<sjt::ObjMap>();
	for(const auto& p : om) ssvu::lo() << p.first << " -> " << "?" << "\n";

	auto& om_01 = om["o1"]->get<sjt::ObjMap>();
	for(const auto& p : om_01) ssvu::lo() << p.first << " -> " << "?" << "\n";

	lo() << (*document)["n1"].get<double>() << "\n";
	lo() << om["n1"]->get<double>() << "\n";

	return 0;
}