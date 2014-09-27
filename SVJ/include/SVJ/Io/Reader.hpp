// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SVJ_IO_READER
#define SVJ_IO_READER

namespace svj
{
	namespace Internal
	{
		class Reader
		{
			private:
				std::string src;
				Idx idx{0u};

				inline static bool isWhitespace(char mC) noexcept	{ return mC == ' ' || mC == '\t' || mC == '\r' || mC == '\n'; }
				inline static bool isNumberStart(char mC) noexcept	{ return mC == '-' || ssvu::isDigit(mC); }

				inline char getC() const noexcept		{ return src[idx]; }
				inline bool isC(char mC) const noexcept	{ return getC() == mC; }
				inline bool isCDigit() const noexcept	{ return ssvu::isDigit(getC()); }

				inline void skipWhitespace() noexcept { while(isWhitespace(getC())) ++idx; }

				inline void match(const std::string& mKeyword)
				{
					for(auto kc : mKeyword)
					{
						if(getC() != kc) throw std::runtime_error{"Invalid keyword " + mKeyword};
						++idx;
					}
				}

				inline auto readStr()
				{
					std::string result;

					// Skip opening '"'
					++idx;

					for(; true; ++idx)
					{
						// End of the string
						if(isC('"')) break;

						// Escape sequence
						if(isC('\\'))
						{
							// Skip '\'
							++idx;

							// Check escape sequence character after '\'
							switch(getC())
							{
								case '"':	result += '\"';	break;
								case '\\':	result += '\\';	break;
								case '/':	result += '/';	break;
								case 'b':	result += '\b';	break;
								case 'f':	result += '\f'; break;
								case 'n':	result += '\n';	break;
								case 'r':	result += '\r';	break;
								case 't':	result += '\t';	break;
								default: throw std::runtime_error("Invalid escape sequence");
							}

							continue;
						}

						// Any other character
						result += getC();
					}

					// Skip closing '"'
					++idx;

					return result;
				}

				inline void purgeSource()
				{
					std::string result, buffer;

					// `commit()` appends the buffer (with a newline) to the result and clears it
					auto commit([&result, &buffer]{ result += buffer + "\n"; buffer.clear(); });

					for(auto i(0u); i < src.size(); ++i)
					{
						// Line without comments
						if(src[i] == '\n') { commit(); continue; }

						// C++-style comment
						if(src[i] == '/' && src[i + 1] == '/')
						{
							commit();

							while(src[i] != '\n') ++i;
							++i;

							continue;
						}

						// Any character
						buffer += src[i];
					}

					// Commit remaining buffer elements and update `src`
					commit(); src = result;
				}

				inline auto parseNull()			{ match("null"); return Value{Null{}}; }
				inline auto parseBoolFalse()	{ match("false"); return Value{false}; }
				inline auto parseBoolTrue()		{ match("true"); return Value{true}; }

				inline auto parseNumber()
				{
					std::string strNum;

					//bool negative{false};
					std::size_t cntDInt{0u}, cntDDec{0u};

					// Check negativity
					if(isC('-')) { ++idx; strNum += "-"; }

					// Get and count non-decimal digits
					while(isCDigit()) { strNum += getC(); ++idx; ++cntDInt; }

					// If there's no dot, return a non-decimal number
					if(!isC('.'))
					{
						//if(std::numeric_limits<int>::digits <= cntDInt)
							return Value{Number{std::stoi(strNum)}};
					}

					// Add dot
					strNum += '.'; ++idx;

					// Get and count decimal digits
					while(isCDigit()) { strNum += getC(); ++idx; ++cntDDec; }

					// Handle possible exponent
					if(isC('e') || isC('E'))
					{
						strNum += 'e';

						++idx;
						if(isC('+')) strNum += '+';
						if(isC('-')) strNum += '-';

						++idx;
						while(isCDigit()) { strNum += getC(); ++idx; }
					}

					if(std::numeric_limits<float>::digits10 <= cntDDec)		return Value{Number{std::stof(strNum)}};
				 /* if(std::numeric_limits<double>::digits10 <= cntDDec) */	return Value{Number{std::stod(strNum)}};
				}

				inline auto parseString() { return Value{readStr()}; }

				inline auto parseArray()
				{
					Array array;

					// Skip '['
					++idx;

					skipWhitespace();

					// Empty array
					if(isC(']')) goto end;

					while(true)
					{
						// Get value
						skipWhitespace();
						array.emplace_back(parseValue());
						skipWhitespace();

						// Check for another value
						if(isC(',')) { ++idx; continue; }

						// Check for end of the array
						if(isC(']')) break;

						throw std::runtime_error{"Invalid array"};
					}

					end:

					// Skip ']'
					++idx;

					return Value{array};
				}

				inline auto parseObject()
				{
					Object object;

					// Skip '{'
					++idx;

					skipWhitespace();

					// Empty object
					if(isC('}')) goto end;

					while(true)
					{
						// Read string key
						skipWhitespace();
						if(!isC('"')) throw std::runtime_error{"Invalid object"};
						auto key(readStr());

						// Read ':'
						skipWhitespace();
						if(!isC(':')) throw std::runtime_error{"Invalid object"};

						// Skip ':'
						++idx;

						// Read value
						skipWhitespace();
						object[key] = parseValue();
						skipWhitespace();

						// Check for another key-value pair
						if(isC(',')) { ++idx; continue; }

						// Check for end of the object
						if(isC('}')) break;

						throw std::runtime_error{"Invalid object"};
					}

					end:

					// Skip '}'
					++idx;

					return Value{object};
				}

				inline Value parseValue()
				{
					skipWhitespace();

					// Check value type
					switch(getC())
					{
						case '{': return parseObject();
						case '[': return parseArray();
						case '"': return parseString();
						case 't': return parseBoolTrue();
						case 'f': return parseBoolFalse();
						case 'n': return parseNull();
					}

					// Check if value is a number
					if(isNumberStart(getC())) return parseNumber();

					throw std::runtime_error{"Invalid value"};
				}

			public:
				inline Reader(std::string mSrc) : src{std::move(mSrc)} { }

				inline auto parseDocument()
				{
					purgeSource();
					skipWhitespace();

					if(isC('{')) return parseObject();
					if(isC('[')) return parseArray();

					throw std::runtime_error{"Invalid document"};
				}
		};
	}
}

#endif
