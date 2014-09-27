// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SVJ_VALUE
#define SVJ_VALUE

namespace svj
{
	namespace Internal
	{
		template<typename T> struct ValueHelper;
	}

	class Value
	{
		template<typename T> friend struct Internal::ValueHelper;

		public:
			enum class Type{Object, Array, String, Number, Bool, Null};

		private:
			Type type{Type::Null};

			union Holder
			{
				Internal::Maybe<ObjectImpl<Value>> hObject;
				Internal::Maybe<ArrayImpl<Value>> hArray;
				Internal::Maybe<std::string> hString;
				Number hNumber;
				bool hBool;
			} h;

			inline void setObject(const ObjectImpl<Value>& mValue)		{ type = Type::Object;	h.hObject.init(mValue); }
			inline void setArray(const ArrayImpl<Value>& mValue)	{ type = Type::Array;	h.hArray.init(mValue); }
			inline void setString(const std::string& mValue)		{ type = Type::String;	h.hString.init(mValue); }
			inline void setNumber(const Number& mValue) noexcept	{ type = Type::Number;	h.hNumber = mValue; }
			inline void setBool(bool mValue) noexcept				{ type = Type::Bool;	h.hBool = mValue; }
			inline void setNull() noexcept							{ type = Type::Null; }

			inline const auto& getObject() const	{ if(type != Type::Object) throw std::runtime_error{"Invalid getObject"};	return h.hObject.get(); }
			inline const auto& getArray() const		{ if(type != Type::Array) throw std::runtime_error{"Invalid getArray"};		return h.hArray.get(); }
			inline const auto& getString() const	{ if(type != Type::String) throw std::runtime_error{"Invalid getString"};	return h.hString.get(); }
			inline auto getNumber() const			{ if(type != Type::Number) throw std::runtime_error{"Invalid getNumber"};	return h.hNumber; }
			inline auto getBool() const				{ if(type != Type::Bool) throw std::runtime_error{"Invalid getBool"};		return h.hBool; }
			inline auto getNull() const				{ if(type != Type::Null) throw std::runtime_error{"Invalid getNull"};		return Null{}; }

			inline auto getMoveObject()	{ if(type != Type::Object) throw std::runtime_error{"Invalid getMoveObject"};	return std::move(h.hObject.get()); }
			inline auto getMoveArray()	{ if(type != Type::Array) throw std::runtime_error{"Invalid getMoveArray"};		return std::move(h.hArray.get()); }
			inline auto getMoveString()	{ if(type != Type::String) throw std::runtime_error{"Invalid getMoveString"};	return std::move(h.hString.get()); }

			inline void deinitCurrent()
			{
				switch(type)
				{
					case Type::Object:	h.hObject.deinit(); break;
					case Type::Array:	h.hArray.deinit(); break;
					case Type::String:	h.hString.deinit(); break;
					default: break;
				}
			}

			inline void initCopy(const Value& mV)
			{
				type = mV.type;

				switch(type)
				{
					case Type::Object:	h.hObject.init(mV.getObject()); break;
					case Type::Array:	h.hArray.init(mV.getArray()); break;
					case Type::String:	h.hString.init(mV.getString()); break;
					case Type::Number:	h.hNumber = mV.getNumber(); break;
					case Type::Bool:	h.hBool = mV.getBool(); break;
					default: break;
				}
			}

			inline void initMove(Value&& mV)
			{
				type = mV.type;

				switch(type)
				{
					case Type::Object:	h.hObject.init(mV.getMoveObject()); break;
					case Type::Array:	h.hArray.init(mV.getMoveArray()); break;
					case Type::String:	h.hString.init(mV.getMoveString()); break;
					case Type::Number:	h.hNumber = mV.getNumber(); break;
					case Type::Bool:	h.hBool = mV.getBool(); break;
					default: break;
				}
			}


		public:
			inline Value() = default;

			template<typename T> inline Value(const T& mValue) { set<T>(mValue); }

			inline Value(const Value& mV) { DL("copy ctor"); initCopy(mV); }
			inline Value(Value&& mV) { DL("move ctor"); initMove(std::move(mV)); }

			inline auto& operator=(const Value& mV) { DL("copy assign"); initCopy(mV); return *this; }
			inline auto& operator=(Value&& mV) { DL("move assign"); initMove(std::move(mV)); return *this; }

			inline ~Value() { DL("dtor"); deinitCurrent(); }

			template<typename T> decltype(auto) set(const T& mValue) noexcept
			{
				deinitCurrent();
				return Internal::ValueHelper<T>::set(*this, mValue);
			}
			template<typename T> decltype(auto) get() const noexcept { return Internal::ValueHelper<T>::get(*this); }

			inline auto getType() const noexcept { return type; }

			//template<typename T> inline decltype(auto) get() const { return ValueHelper<T>::get(*this); }
			//template<typename T> inline decltype(auto) get(const std::string& mKey) const { return ValueHelper<T>::get((*this)[mKey]); }

			//const Value& operator[](const std::string& mKey) const;
	};

	using Object = ObjectImpl<Value>;
	using Array = ArrayImpl<Value>;

	namespace Internal
	{
		#define SVJ_VALUEHELPER_NUMBER(mX) \
			template<> struct ValueHelper<mX> \
			{ \
				inline static decltype(auto) set(Value& mO, const mX& mV) { mO.setNumber(Number{mV}); } \
				inline static decltype(auto) get(const Value& mO) { return mO.getNumber().get<mX>(); } \
			};

		SVJ_VALUEHELPER_NUMBER(char)
		SVJ_VALUEHELPER_NUMBER(unsigned char)
		SVJ_VALUEHELPER_NUMBER(int)
		SVJ_VALUEHELPER_NUMBER(unsigned int)
		SVJ_VALUEHELPER_NUMBER(float)
		SVJ_VALUEHELPER_NUMBER(double)

		#undef SVJ_VALUEHELPER_NUMBER

		template<> struct ValueHelper<Object>
		{
			inline static decltype(auto) set(Value& mO, const Object& mV) { mO.setObject(mV); }
			inline static decltype(auto) get(const Value& mO) { return mO.getObject(); }
		};

		template<> struct ValueHelper<Array>
		{
			inline static decltype(auto) set(Value& mO, const Array& mV) { mO.setArray(mV); }
			inline static decltype(auto) get(const Value& mO) { return mO.getArray(); }
		};

		template<std::size_t TS> struct ValueHelper<char[TS]>
		{
			inline static decltype(auto) set(Value& mO, const char(&mV)[TS]) { mO.setString(mV); }
			inline static decltype(auto) get(const Value& mO) { return mO.getString(); }
		};

		template<> struct ValueHelper<std::string>
		{
			inline static decltype(auto) set(Value& mO, const std::string& mV) { mO.setString(mV); }
			inline static decltype(auto) get(const Value& mO) { return mO.getString(); }
		};

		template<> struct ValueHelper<Number>
		{
			inline static decltype(auto) set(Value& mO, const Number& mV) { mO.setNumber(mV); }
			inline static decltype(auto) get(const Value& mO) { return mO.getNumber(); }
		};

		template<> struct ValueHelper<bool>
		{
			inline static decltype(auto) set(Value& mO, bool mV) { mO.setBool(mV); }
			inline static decltype(auto) get(const Value& mO) { return mO.getBool(); }
		};

		template<> struct ValueHelper<Null>
		{
			inline static decltype(auto) set(Value& mO, Null) { mO.setNull(); }
			inline static decltype(auto) get(const Value& mO) { return mO.getNull(); }
		};
	}
}

#endif
