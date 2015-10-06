// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SVJ_VALUE_INL
#define SVJ_VALUE_INL

namespace ssvu
{
    namespace Json
    {
        template <WriterMode TWS, bool TFmt>
        inline void Value::writeToStream(std::ostream& mStream) const
        {
            Internal::Writer<TWS, TFmt> w;
            w.write(*this, mStream);
            mStream.flush();
        }
        template <WriterMode TWS, bool TFmt>
        inline void Value::writeToString(std::string& mStr) const
        {
            std::ostringstream o;
            writeToStream<TWS, TFmt>(o);
            mStr = o.str();
        }
        template <WriterMode TWS, bool TFmt>
        inline void Value::writeToFile(const ssvufs::Path& mPath) const
        {
            std::ofstream o{mPath};
            writeToStream<TWS, TFmt>(o);
            o.close();
        }
        template <WriterMode TWS, bool TFmt>
        inline auto Value::getWriteToString() const
        {
            std::string result;
            writeToString<TWS, TFmt>(result);
            return result;
        }

        inline void Value::readFromString(std::string mStr)
        {
            Internal::Reader r{std::move(mStr)};
            Internal::tryParse(*this, r);
        }
        inline void Value::readFromFile(const ssvufs::Path& mPath)
        {
            Internal::Reader r{std::move(mPath.getContentsAsString())};
            ;
            Internal::tryParse(*this, r);
        }
    }
}

#endif
