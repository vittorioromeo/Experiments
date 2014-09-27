// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SVJ_IO_WRITER
#define SVJ_IO_WRITER

namespace svj
{
	namespace Internal
	{
		class Writer
		{
			private:
				std::string out;
				std::size_t depth{0};
				bool needIndent{false};

				inline bool isObjectOrArray(const Value& mValue)
				{
					return mValue.getType() == Value::Type::Object || mValue.getType() == Value::Type::Array;
				}

				inline void indent()
				{
					for(auto i(0u); i < depth; ++i) out += "    ";
					needIndent = false;
				}

				inline void wNL() { out += "\n"; needIndent = true; }

				inline void wOut(const std::string& mStr)
				{
					if(needIndent) indent();
					out += mStr;
				}

				template<typename TItr, typename TF1, typename TF2> inline void repeatWithSeparator(TItr mBegin, TItr mEnd, TF1 mF1, TF2 mF2)
				{
					for(; mBegin != std::prev(mEnd); ++mBegin) { mF1(mBegin); mF2(); }
					mF1(mBegin);
				}



				inline void write(const Object& mObject)
				{
					wOut("{"); wNL();

					++depth;

					repeatWithSeparator(std::begin(mObject), std::end(mObject), [this](auto mItr)
					{
						write(mItr->first);
						wOut(": ");
						if(isObjectOrArray(mItr->second)) wNL();
						write(mItr->second);
					}, [this]{ wOut(", "); wNL(); });

					--depth;

					wNL(); wOut("}");
				}

				inline void write(const Array& mArray)
				{
					wOut("["); wNL();

					++depth;

					repeatWithSeparator(std::begin(mArray), std::end(mArray), [this](auto mItr)
					{
						write(*mItr);
					}, [this]{ wOut(", "); wNL(); });

					--depth;

					wNL(); wOut("]");
				}

				inline void write(const String& mStr) { wOut("\"" + mStr + "\""); }

				inline void write(const Number& mNumber)
				{
					switch(mNumber.getType())
					{
						case Number::Type::IntS:	wOut(ssvu::toStr(mNumber.getIntS())); break;
						case Number::Type::IntU:	wOut(ssvu::toStr(mNumber.getIntU())); break;
						case Number::Type::Real:	wOut(ssvu::toStr(mNumber.getReal())); break;
					}
				}

				inline void write(bool mBool) { wOut(mBool ? "true" : "false"); }
				inline void write(Null) { wOut("null"); }

				inline void write(const Value& mValue)
				{
					switch(mValue.getType())
					{
						case Value::Type::Object:	write(mValue.get<Object>()); break;
						case Value::Type::Array:	write(mValue.get<Array>()); break;
						case Value::Type::String:	write(mValue.get<String>()); break;
						case Value::Type::Number:	write(mValue.get<Number>()); break;
						case Value::Type::Bool:		write(mValue.get<bool>()); break;
						case Value::Type::Null:		write(Null{}); break;
					}
				}

			public:
				inline void write(const Value& mValue, std::ostream& mStream) { write(mValue); mStream << out; }
		};
	}
}

#endif
