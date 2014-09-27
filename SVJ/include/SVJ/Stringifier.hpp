// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SVJ_STRINGIFIER
#define SVJ_STRINGIFIER

namespace ssvu
{
	/*template<> struct Stringifier<svj::Number>
	{
		template<bool TFmt> inline static void impl(std::ostream& mStream, const svj::Number& mValue)
		{
			switch(mValue.getType())
			{
				case svj::Number::Type::IntS:	Internal::callStringifyImpl<TFmt>(mStream, mValue.get<int>()); break;
				case svj::Number::Type::IntU:	Internal::callStringifyImpl<TFmt>(mStream, mValue.get<unsigned int>()); break;
				case svj::Number::Type::Real:	Internal::callStringifyImpl<TFmt>(mStream, mValue.get<float>()); break;
			}
		}
	};*/

	template<> struct Stringifier<svj::Value>
	{
		template<bool TFmt> inline static void impl(std::ostream& mStream, const svj::Value& mValue)
		{
			mStream << "\n" << svj::getWriteToString(mValue);

			return;

			//Internal::printBold<TFmt>(mStream, "[", Console::Color::Green);

			/*switch(mValue.getType())
			{
				case svj::Value::Type::Object:	mStream << "\n"; Internal::callStringifyImpl<TFmt>(mStream, mValue.get<svj::Object>()); mStream << "\n"; break;
				case svj::Value::Type::Array:	mStream << "\n"; Internal::callStringifyImpl<TFmt>(mStream, mValue.get<svj::Array>()); mStream << "\n"; break;
				case svj::Value::Type::String:	Internal::callStringifyImpl<TFmt>(mStream, mValue.get<svj::String>()); break;
				case svj::Value::Type::Number:	Internal::callStringifyImpl<TFmt>(mStream, mValue.get<svj::Number>()); break;
				case svj::Value::Type::Bool:	Internal::callStringifyImpl<TFmt>(mStream, mValue.get<bool>()); break;
				case svj::Value::Type::Null:	Internal::printNonBold<TFmt>(mStream, "null", Console::Color::Green); break;
			}*/

			//Internal::printBold<TFmt>(mStream, "]", Console::Color::Green);
		}
	};
}

#endif
