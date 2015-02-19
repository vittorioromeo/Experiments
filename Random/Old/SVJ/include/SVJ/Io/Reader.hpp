// Copyright (c) 2013-2015 Vittorio Romeo
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
			inline constexpr char getEscapeSequence(char mC) noexcept
			{
				switch(mC)
				{
					case '"':	return '"';
					case '\\':	return '\\';
					case '/':	return '/';
					case 'b':	return '\b';
					case 'f':	return '\f';
					case 'n':	return '\n';
					case 'r':	return '\r';
					case 't':	return '\t';
				}

				SSVU_ASSERT_CONSTEXPR(false);
				std::terminate();
			}

			class Reader
			{
				private:
					std::string src;
					Idx idx{0u};

					inline auto getErrorSrc()
					{
						auto iStart(std::max(Idx(0), idx - 20));
						auto iEnd(std::min(src.size() - 1, idx + 20));

						auto iDStart(std::max(Idx(0), idx - 4));
						auto iDEnd(std::min(src.size() - 1, idx + 4));

						auto strMarked
						(
							std::string{std::begin(src) + iStart, std::begin(src) + iDStart}
							+ " >>>>>> "
							+ std::string{std::begin(src) + iDStart, std::begin(src) + iDEnd}
							+ " <<<<<< "
							+ std::string{std::begin(src) + iDEnd, std::begin(src) + iEnd}
						);

						auto strUnmarked(std::string{std::begin(src) + iStart, std::begin(src) + iEnd});

						replaceAll(strMarked, "\n", "");
						replaceAll(strUnmarked, "\n", "");

						return strUnmarked + "\n" + strMarked;
					}

					inline static constexpr auto isWhitespace(char mC) noexcept		{ return mC == ' ' || mC == '\t' || mC == '\r' || mC == '\n'; }
					inline static constexpr auto isNumberStart(char mC) noexcept	{ return mC == '-' || isDigit(mC); }

					inline char getC() const noexcept		{ return src[idx]; }
					inline auto isC(char mC) const noexcept	{ return getC() == mC; }
					inline auto isCDigit() const noexcept	{ return isDigit(getC()); }

					inline void skipWhitespaceAndComments() noexcept
					{
						while(isWhitespace(getC())) ++idx;

						if(src[idx] != '/' || src[idx + 1] != '/') return;

						while(getC() != '\n') ++idx;
						skipWhitespaceAndComments();
					}

					template<std::size_t TS> inline void match(const char(&mKeyword)[TS])
					{
						for(auto i(0u); i < TS - 1; ++i)
						{
							if(getC() != mKeyword[i]) throw ReadException("Invalid keyword", std::string{"Couldn't match keyword `"} + std::string{mKeyword} + "'", getErrorSrc());
							++idx;
						}
					}

					inline auto readString()
					{
						// Skip opening '"'
						++idx;

						// Find end index of the string
						auto end(idx);
						for(; true; ++end)
						{
							// End of the string
							if(src[end] == '"') break;

							// TODO: assert escape sequence validity
							if(src[end] == '\\') { ++end; continue; }
						}

						// Reserve memory for the string
						String result;
						result.reserve(end - idx);

						for(; idx < end; ++idx)
						{
							// Not an escape sequence
							if(!isC('\\')) { result += getC(); continue; }

							// Escape sequence: skip '\'
							++idx;

							// Convert escape sequence
							result += getEscapeSequence(getC());
						}

						// Skip closing '"'
						++idx;

						return result;
					}

					inline auto parseNull()			{ match("null"); return Value{Null{}}; }
					inline auto parseBoolFalse()	{ match("false"); return Value{false}; }
					inline auto parseBoolTrue()		{ match("true"); return Value{true}; }

					inline auto parseNumber()
					{
						char* endChar;

						auto realN(static_cast<Number::Real>(std::strtod(src.data() + idx, &endChar)));
						auto intSN(static_cast<Number::IntS>(realN));

						idx = endChar - src.data();

						auto isDecimal(intSN != realN);
						return Value{Number{isDecimal ? realN : intSN}};
					}

					inline auto parseString() { return Value{readString()}; }

					inline auto parseArray()
					{
						Array array;

						// Skip '['
						++idx;

						skipWhitespaceAndComments();

						// Empty array
						if(isC(']')) goto end;

						while(true)
						{
							// Get value
							skipWhitespaceAndComments();
							array.emplace_back(parseValue());
							skipWhitespaceAndComments();

							// Check for another value
							if(isC(',')) { ++idx; continue; }

							// Check for end of the array
							if(isC(']')) break;

							throw ReadException{"Invalid array", "Expected either `,` or `]`, got `"s + getC() + "`", getErrorSrc()};
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

						skipWhitespaceAndComments();

						// Empty object
						if(isC('}')) goto end;

						while(true)
						{
							// Read string key
							skipWhitespaceAndComments();
							if(!isC('"')) throw ReadException{"Invalid object", "Expected `\"` , got `"s + getC() + "`", getErrorSrc()};
							auto key(readString());

							// Read ':'
							skipWhitespaceAndComments();
							if(!isC(':')) throw ReadException{"Invalid object", "Expected `:` , got `"s + getC() + "`", getErrorSrc()};

							// Skip ':'
							++idx;

							// Read value
							skipWhitespaceAndComments();
							object[key] = parseValue();
							skipWhitespaceAndComments();

							// Check for another key-value pair
							if(isC(',')) { ++idx; continue; }

							// Check for end of the object
							if(isC('}')) break;

							throw ReadException{"Invalid object", "Expected either `,` or `}`, got `"s + getC() + "`", getErrorSrc()};
						}

						end:

						// Skip '}'
						++idx;

						return Value{object};
					}

				public:
					inline Reader(std::string mSrc) : src{std::move(mSrc)} { }

					inline Value parseValue()
					{
						skipWhitespaceAndComments();

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

						throw ReadException{"Invalid value", "No match for values beginning with `"s + getC() + "`", getErrorSrc()};
					}
			};
		}
	}
}

#endif
