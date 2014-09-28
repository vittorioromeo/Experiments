// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SVJ_VALUE
#define SVJ_VALUE

namespace ssvu
{
	namespace Json
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

				using Object = ObjectImpl<Value>;
				using Array = ArrayImpl<Value>;

			private:
				Type type{Type::Null};

				union Holder
				{
					Internal::Maybe<ObjectImpl<Value>> hObject;
					Internal::Maybe<ArrayImpl<Value>> hArray;
					Internal::Maybe<String> hString;
					Number hNumber;
					Bool hBool;

					inline Holder() noexcept
					{
						#if !(SSVU_IMPL_ASSERT_DISABLED)
							hObject.alive = false;
							hArray.alive = false;
							hString.alive = false;
						#endif
					}
				} h;

				// These `setX` functions must only be called after calling `deinitCurrent()`
				inline void setObject(const ObjectImpl<Value>& mX)	{ type = Type::Object;	h.hObject.init(mX); }
				inline void setArray(const ArrayImpl<Value>& mX)	{ type = Type::Array;	h.hArray.init(mX); }
				inline void setString(const String& mX)				{ type = Type::String;	h.hString.init(mX); }
				inline void setNumber(const Number& mX) noexcept	{ type = Type::Number;	h.hNumber = mX; }
				inline void setBool(Bool mX) noexcept				{ type = Type::Bool;	h.hBool = mX; }
				inline void setNull() noexcept						{ type = Type::Null; }

				inline void setObject(ObjectImpl<Value>&& mX) noexcept	{ type = Type::Object;	h.hObject.init(std::move(mX)); }
				inline void setArray(ArrayImpl<Value>&& mX) noexcept	{ type = Type::Array;	h.hArray.init(std::move(mX)); }
				inline void setString(String&& mX) noexcept				{ type = Type::String;	h.hString.init(std::move(mX)); }

				inline const auto& getObject() const noexcept	{ SSVU_ASSERT(is<Object>());	return h.hObject.get(); }
				inline const auto& getArray() const noexcept	{ SSVU_ASSERT(is<Array>());		return h.hArray.get(); }
				inline const auto& getString() const noexcept	{ SSVU_ASSERT(is<String>());	return h.hString.get(); }
				inline auto getNumber() const noexcept			{ SSVU_ASSERT(is<Number>());	return h.hNumber; }
				inline auto getBool() const noexcept			{ SSVU_ASSERT(is<Bool>());		return h.hBool; }
				inline auto getNull() const noexcept			{ SSVU_ASSERT(is<Null>());		return Null{}; }

				inline auto getMoveObject() noexcept	{ SSVU_ASSERT(is<Object>());	return std::move(h.hObject.get()); }
				inline auto getMoveArray() noexcept		{ SSVU_ASSERT(is<Array>());		return std::move(h.hArray.get()); }
				inline auto getMoveString() noexcept	{ SSVU_ASSERT(is<String>());	return std::move(h.hString.get()); }

				inline auto& getObject() noexcept	{ SSVU_ASSERT(is<Object>());	return h.hObject.get(); }
				inline auto& getArray() noexcept	{ SSVU_ASSERT(is<Array>());		return h.hArray.get(); }

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

				inline void init(const Value& mV)
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

				inline void init(Value&& mV)
				{
					type = mV.type;

					switch(type)
					{
						case Type::Object:	h.hObject.init(std::move(mV.getMoveObject())); break;
						case Type::Array:	h.hArray.init(std::move(mV.getMoveArray())); break;
						case Type::String:	h.hString.init(std::move(mV.getMoveString())); break;
						case Type::Number:	h.hNumber = mV.getNumber(); break;
						case Type::Bool:	h.hBool = mV.getBool(); break;
						default: break;
					}
				}


			public:
				inline Value() = default;
				inline Value(const Value& mV) { init(mV); }
				inline Value(Value&& mV) { init(std::move(mV)); }

				template<typename T, EnableIf<!isSame<RemoveAll<T>, Value>()>* = nullptr> inline Value(T&& mX) { set(fwd<T>(mX)); }

				inline ~Value() { deinitCurrent(); }

				// "Explicit" `set` function set the inner contents of the value
				template<typename T> inline void set(T&& mX) { deinitCurrent(); Internal::ValueHelper<RemoveAll<T>>::set(*this, fwd<T>(mX)); }

				// "Implicit" `set` function done via `operator=` overloading
				template<typename T> inline auto& operator=(T&& mX) { set(fwd<T>(mX)); return *this; }

				// "Explicit" `get` function gets the inner contents of the value
				template<typename T> decltype(auto) get() const noexcept { return Internal::ValueHelper<T>::get(*this); }

				// "Implicit" Value from Object by Key getters
				inline auto& operator[](const Key& mKey)				{ return getObject()[mKey]; }
				inline const auto& operator[](const Key& mKey) const	{ return getObject().at(mKey); }

				// "Implicit" Value from Array by Idx getters
				inline auto& operator[](Idx mIdx)				{ return getArray()[mIdx]; }
				inline const auto& operator[](Idx mIdx) const	{ return getArray().at(mIdx); }

				inline auto getType() const noexcept { return type; }

				template<typename T> inline bool is() const noexcept { return Internal::ValueHelper<T>::is(*this); }

				template<WriterMode TWS = WriterMode::Pretty, bool TFmt = false> void writeToStream(std::ostream& mStream) const;
				template<WriterMode TWS = WriterMode::Pretty, bool TFmt = false> void writeToString(std::string& mStr) const;
				template<WriterMode TWS = WriterMode::Pretty, bool TFmt = false> void writeToFile(const ssvufs::Path& mPath) const;
				template<WriterMode TWS = WriterMode::Pretty, bool TFmt = false> auto getWriteToString() const;

				void readFromString(std::string mStr);
				void readFromFile(const ssvufs::Path& mPath);

				static inline Value fromString(const std::string& mStr)	{ Value result; result.readFromString(mStr); return result; }
				static inline Value fromFile(const ssvufs::Path& mPath)	{ Value result; result.readFromFile(mPath); return result; }
		};

		using Object = Value::Object;
		using Array = Value::Array;

		namespace Internal
		{
			#define SVJ_VALUEHELPER_NUMBER(mType) \
				template<> struct ValueHelper<mType> \
				{ \
					inline static void set(Value& mV, const mType& mX) { mV.setNumber(Number{mX}); } \
					inline static void set(Value& mV, mType&& mX) { mV.setNumber(Number{mX}); } \
					inline static decltype(auto) get(const Value& mV) { return mV.getNumber().get<mType>(); } \
					inline static auto is(const Value& mV) noexcept { return mV.getType() == Value::Type::Number; } \
				};

			SVJ_VALUEHELPER_NUMBER(char)
			SVJ_VALUEHELPER_NUMBER(unsigned char)
			SVJ_VALUEHELPER_NUMBER(int)
			SVJ_VALUEHELPER_NUMBER(unsigned int)
			SVJ_VALUEHELPER_NUMBER(float)
			SVJ_VALUEHELPER_NUMBER(double)

			#undef SVJ_VALUEHELPER_NUMBER

			template<> struct ValueHelper<Value>
			{
				inline static void set(Value& mV, const Value& mX) { mV.init(mX); }
				inline static void set(Value& mV, Value&& mX) { mV.init(std::move(mX)); }
			};

			template<> struct ValueHelper<Object>
			{
				inline static void set(Value& mV, const Object& mX) { mV.setObject(mX); }
				inline static void set(Value& mV, Object&& mX) { mV.setObject(std::move(mX)); }
				inline static decltype(auto) get(const Value& mV) { return mV.getObject(); }
				inline static auto is(const Value& mV) noexcept { return mV.getType() == Value::Type::Object; }
			};

			template<> struct ValueHelper<Array>
			{
				inline static void set(Value& mV, const Array& mX) { mV.setArray(mX); }
				inline static void set(Value& mV, Array&& mX) { mV.setArray(std::move(mX)); }
				inline static decltype(auto) get(const Value& mV) { return mV.getArray(); }
				inline static auto is(const Value& mV) noexcept { return mV.getType() == Value::Type::Array; }
			};

			template<std::size_t TS> struct ValueHelper<char[TS]>
			{
				inline static void set(Value& mV, const char(&mX)[TS]) { mV.setString(mX); }
				inline static decltype(auto) get(const Value& mV) { return mV.getString(); }
				inline static auto is(const Value& mV) noexcept { return mV.getType() == Value::Type::String; }
			};

			template<> struct ValueHelper<String>
			{
				inline static void set(Value& mV, const String& mX) { mV.setString(mX); }
				inline static void set(Value& mV, String&& mX) { mV.setString(std::move(mX)); }
				inline static decltype(auto) get(const Value& mV) { return mV.getString(); }
				inline static auto is(const Value& mV) noexcept { return mV.getType() == Value::Type::String; }
			};

			template<> struct ValueHelper<Number>
			{
				inline static void set(Value& mV, const Number& mX) { mV.setNumber(mX); }
				inline static decltype(auto) get(const Value& mV) { return mV.getNumber(); }
				inline static auto is(const Value& mV) noexcept { return mV.getType() == Value::Type::Number; }
			};

			template<> struct ValueHelper<Bool>
			{
				inline static void set(Value& mV, Bool mX) { mV.setBool(mX); }
				inline static decltype(auto) get(const Value& mV) { return mV.getBool(); }
				inline static auto is(const Value& mV) noexcept { return mV.getType() == Value::Type::Bool; }
			};

			template<> struct ValueHelper<Null>
			{
				inline static void set(Value& mV, Null) { mV.setNull(); }
				inline static decltype(auto) get(const Value& mV) { return mV.getNull(); }
				inline static auto is(const Value& mV) noexcept { return mV.getType() == Value::Type::Null; }
			};
		}
	}
}

#endif
