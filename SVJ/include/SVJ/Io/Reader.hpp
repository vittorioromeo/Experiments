// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SVJ_IO_READER
#define SVJ_IO_READER

namespace ssvu
{
	namespace Json
	{
		namespace Internal
		{
			class Reader
			{
				private:
					std::string src;
					Idx idx{0u};

					inline auto getSrcLine()
					{
						auto iStart(idx), iEnd(idx);

						while(iStart > 0 && src[iStart] != '\n') --iStart;
						while(iEnd < src.size() && src[iEnd] != '\n') ++iEnd;

						return std::string{std::begin(src) + iStart, std::begin(src) + iEnd};
					}

					inline static auto isWhitespace(char mC) noexcept	{ return mC == ' ' || mC == '\t' || mC == '\r' || mC == '\n'; }
					inline static auto isNumberStart(char mC) noexcept	{ return mC == '-' || isDigit(mC); }

					inline char getC() const noexcept		{ return src[idx]; }
					inline auto isC(char mC) const noexcept	{ return getC() == mC; }
					inline auto isCDigit() const noexcept	{ return isDigit(getC()); }

					inline void skipWhitespace() noexcept { while(isWhitespace(getC())) ++idx; }

					inline void match(const std::string& mKeyword)
					{
						for(auto kc : mKeyword)
						{
							if(getC() != kc) throw ReadException("Invalid keyword", "Couldn't match keyword `"s + mKeyword + "'", getSrcLine());
							++idx;
						}
					}

					inline auto readString()
					{
						String result;

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
									default: throw ReadException("Invalid escape sequence", "No match for escape sequence `\\"s + getC() + "`", getSrcLine());
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

					inline auto parseString() { return Value{readString()}; }

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

							throw ReadException{"Invalid array", "Expected either `,` or `]`, got `"s + getC() + "`", getSrcLine()};
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
							if(!isC('"')) throw ReadException{"Invalid object", "Expected `\"` , got `"s + getC() + "`", getSrcLine()};
							auto key(readString());

							// Read ':'
							skipWhitespace();
							if(!isC(':')) throw ReadException{"Invalid object", "Expected `:` , got `"s + getC() + "`", getSrcLine()};

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

							throw ReadException{"Invalid object", "Expected either `,` or `}`, got `"s + getC() + "`", getSrcLine()};
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

						throw ReadException{"Invalid value", "No match for values beginning with `"s + getC() + "`", getSrcLine()};
					}

				public:
					inline Reader(std::string mSrc) : src{std::move(mSrc)} { }

					inline auto parseDocument()
					{
						purgeSource();
						skipWhitespace();

						return parseValue();
					}
			};
		}
	}
}

#endif
