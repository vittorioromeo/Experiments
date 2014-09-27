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

				inline void indent()
				{
					for(auto i(0u); i < depth; ++i) out += "    ";
					needIndent = false;
				}

				inline void wNL()
				{
					out += "\n";
					needIndent = true;
				}

				inline void wOut(const std::string& mStr)
				{
					if(needIndent) indent();
					out += mStr;
				}

				inline void writeObject(const Object& mObject)
				{
					wOut("{");
					wNL();

					++depth;

					auto itr(std::begin(mObject));
					for(; itr != std::prev(std::end(mObject)); ++itr)
					{
						writeString(itr->first);
						wOut(": ");

						if(itr->second.getType() == Value::Type::Object || itr->second.getType() == Value::Type::Array) wNL();
						writeValue(itr->second);

						wOut(", ");
						wNL();
					}

					writeString(itr->first);
					wOut(": ");
					writeValue(itr->second);

					--depth;

					wNL();
					wOut("}");
				}

				inline void writeArray(const Array& mArray)
				{
					wOut("[");
					wNL();

					++depth;

					for(auto i(0u); i < mArray.size() - 1; ++i)
					{
						writeValue(mArray.at(i));
						wOut(", ");
						wNL();
					}

					writeValue(mArray.back());

					--depth;

					wNL();
					wOut("]");
				}

				inline void writeString(const std::string& mStr)
				{
					wOut("\"" + mStr + "\"");
				}

				inline void writeNumber(const Number& mNumber)
				{
					switch(mNumber.getType())
					{
						case Number::Type::IntS:	wOut(ssvu::toStr(mNumber.getIntS())); break;
						case Number::Type::IntU:	wOut(ssvu::toStr(mNumber.getIntU())); break;
						case Number::Type::Real:	wOut(ssvu::toStr(mNumber.getReal())); break;
					}
				}

				inline void writeBool(bool mBool)
				{
					wOut(mBool ? "true" : "false");
				}

				inline void writeNull()
				{
					wOut("null");
				}

				inline void writeValue(const Value& mValue)
				{
					switch(mValue.getType())
					{
						case Value::Type::Object:	writeObject(mValue.get<Object>()); break;
						case Value::Type::Array:	writeArray(mValue.get<Array>()); break;
						case Value::Type::String:	writeString(mValue.get<std::string>()); break;
						case Value::Type::Number:	writeNumber(mValue.get<Number>()); break;
						case Value::Type::Bool:		writeBool(mValue.get<bool>()); break;
						case Value::Type::Null:		writeNull(); break;
					}
				}

			public:
				inline void write(const Value& mValue, std::ostream& mStream)
				{
					writeValue(mValue);
					mStream << out;
				}
		};
	}
}

#endif
