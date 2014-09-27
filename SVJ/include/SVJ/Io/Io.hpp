// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SVJ_IO
#define SVJ_IO

#include "../Io/Reader.hpp"
#include "../Io/Writer.hpp"

namespace svj
{
	namespace Internal
	{
		inline bool tryParse(Value& mValue, Internal::Reader& mReader)
		{
			try
			{
				mValue = mReader.parseDocument();
			}
			catch(std::runtime_error& mEx)
			{
				ssvu::lo("SVJ reading error") << mEx.what() << std::endl;
				return false;
			}

			return true;
		}
	}

	inline void readFromString(Value& mValue, std::string mStr)			{ Internal::Reader r{std::move(mStr)};							Internal::tryParse(mValue, r); }
	inline void readFromFile(Value& mValue, const ssvufs::Path& mPath)	{ Internal::Reader r{std::move(mPath.getContentsAsString())};;	Internal::tryParse(mValue, r); }

	inline Value getFromString(const std::string& mStr)	{ Value result; readFromString(result, mStr); return result; }
	inline Value getFromFile(const ssvufs::Path& mPath)	{ Value result; readFromFile(result, mPath); return result; }

	template<WriterMode TWS = WriterMode::Pretty, bool TFmt = false> inline void writeToStream(const Value& mValue, std::ostream& mStream)		{ Internal::Writer<TWS, TFmt> w; w.write(mValue, mStream); mStream.flush(); }
	template<WriterMode TWS = WriterMode::Pretty, bool TFmt = false> inline void writeToString(const Value& mValue, std::string& mStr)			{ std::ostringstream o; writeToStream<TWS, TFmt>(mValue, o); mStr = o.str(); }
	template<WriterMode TWS = WriterMode::Pretty, bool TFmt = false> inline void writeToFile(const Value& mValue, const ssvufs::Path& mPath)	{ std::ofstream o{mPath}; writeToStream<TWS, TFmt>(mValue, o); o.close(); }
	template<WriterMode TWS = WriterMode::Pretty, bool TFmt = false> inline auto getWriteToString(const Value& mValue)							{ std::string result; writeToString<TWS, TFmt>(mValue, result); return result; }
}

#endif
