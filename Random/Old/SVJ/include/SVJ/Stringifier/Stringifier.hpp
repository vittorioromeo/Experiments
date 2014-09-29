// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SVJ_STRINGIFIER
#define SVJ_STRINGIFIER

namespace ssvu
{
	template<> struct Stringifier<Json::Value>
	{
		template<bool TFmt> inline static void impl(std::ostream& mStream, const Json::Value& mValue)
		{
			Internal::printNonBold<TFmt>(mStream, "JSON Value:\n", Console::Color::Green);
			mValue.writeToStream<Json::WriterMode::Pretty, true>(mStream);
		}
	};
}

#endif
