// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SVJ_STRINGIFIER
#define SVJ_STRINGIFIER

namespace ssvu
{
	template<> struct Stringifier<svj::Value>
	{
		template<bool TFmt> inline static void impl(std::ostream& mStream, const svj::Value& mValue)
		{
			Internal::printNonBold<TFmt>(mStream, "JSON Value:\n", Console::Color::Green);
			svj::template writeToStream<svj::WriterMode::Pretty, true>(mValue, mStream);
		}
	};
}

#endif
