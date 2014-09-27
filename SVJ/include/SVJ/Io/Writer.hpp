// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SVJ_IO_WRITER
#define SVJ_IO_WRITER

namespace svj
{
	enum class WriterMode{Pretty, Minified};

	namespace Internal
	{
		template<WriterMode TWS = WriterMode::Pretty, bool TFmt = false> class Writer
		{
			private:
				using FmtCC = ssvu::Console::Color;
				using FmtCS = ssvu::Console::Style;

				std::string out;
				std::size_t depth{0};
				bool needIndent{false};

				inline auto isObjectOrArray(const Value& mValue)
				{
					return mValue.getType() == Value::Type::Object || mValue.getType() == Value::Type::Array;
				}

				inline void indent()
				{
					for(auto i(0u); i < depth; ++i) out += "    ";
					needIndent = false;
				}

				inline void wFmt(FmtCC mColor, FmtCS mStyle = FmtCS::None)
				{
					if(!TFmt) return;

					out += ssvu::Console::resetFmt();
					out += ssvu::Console::setColorFG(mColor);
					out += ssvu::Console::setStyle(mStyle);
				}

				inline void wNL()
				{
					if(TWS == WriterMode::Pretty) out += "\n"; needIndent = true;
				}

				inline void wWS()
				{
					if(TWS == WriterMode::Pretty) out += " ";
				}

				inline void wOut(const std::string& mStr)
				{
					if(TWS == WriterMode::Pretty)
					{
						if(needIndent) indent();
					}

					out += mStr;
				}

				template<typename TItr, typename TF1, typename TF2> inline void repeatWithSeparator(TItr mBegin, TItr mEnd, TF1 mF1, TF2 mF2)
				{
					for(; mBegin != std::prev(mEnd); ++mBegin) { mF1(mBegin); mF2(); }
					mF1(mBegin);
				}



				inline void write(const Object& mObject)
				{
					wFmt(FmtCC::LightGray, FmtCS::Bold);
					wOut("{"); wNL();

					++depth;

					repeatWithSeparator(std::begin(mObject), std::end(mObject), [this](auto mItr)
					{
						writeKey(mItr->first);

						wFmt(FmtCC::LightGray, FmtCS::Bold);
						wOut(":"); wWS();

						if(isObjectOrArray(mItr->second)) wNL();

						write(mItr->second);
					}, [this]{ wOut(","); wWS(); wNL(); });

					--depth;

					wFmt(FmtCC::LightGray, FmtCS::Bold);
					wNL(); wOut("}");
				}

				inline void write(const Array& mArray)
				{
					wFmt(FmtCC::LightGray, FmtCS::Bold);
					wOut("["); wNL();

					++depth;

					repeatWithSeparator(std::begin(mArray), std::end(mArray), [this](auto mItr)
					{
						write(*mItr);
					}, [this]
					{
						wFmt(FmtCC::LightGray, FmtCS::Bold);
						wOut(","); wWS(); wNL();
					});

					--depth;

					wFmt(FmtCC::LightGray, FmtCS::Bold);
					wNL(); wOut("]");
				}

				inline void writeKey(const Key& mKey)
				{
					wFmt(FmtCC::LightGray);
					wOut("\"" + mKey + "\"");
				}

				inline void write(const String& mStr)
				{
					wFmt(FmtCC::LightYellow);
					wOut("\"" + mStr + "\"");
				}

				inline void write(const Number& mNumber)
				{
					wFmt(FmtCC::LightRed);

					switch(mNumber.getType())
					{
						case Number::Type::IntS:	wOut(ssvu::toStr(mNumber.get<int>())); break;
						case Number::Type::IntU:	wOut(ssvu::toStr(mNumber.get<unsigned int>())); break;
						case Number::Type::Real:	wOut(ssvu::toStr(mNumber.get<float>())); break;
					}
				}

				inline void write(Bool mBool)
				{
					wFmt(FmtCC::LightCyan);
					wOut(mBool ? "true" : "false");
				}

				inline void write(Null)
				{
					wFmt(FmtCC::LightMagenta);
					wOut("null");
				}

				inline void write(const Value& mValue)
				{
					switch(mValue.getType())
					{
						case Value::Type::Object:	write(mValue.get<Object>()); break;
						case Value::Type::Array:	write(mValue.get<Array>()); break;
						case Value::Type::String:	write(mValue.get<String>()); break;
						case Value::Type::Number:	write(mValue.get<Number>()); break;
						case Value::Type::Bool:		write(mValue.get<Bool>()); break;
						case Value::Type::Null:		write(Null{}); break;
					}
				}

			public:
				inline void write(const Value& mValue, std::ostream& mStream) { write(mValue); mStream << out; }
		};
	}
}

#endif
