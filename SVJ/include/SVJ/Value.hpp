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

			inline void setMoveObject(ObjectImpl<Value>&& mX) noexcept	{ type = Type::Object;	h.hObject.init(std::move(mX)); }
			inline void setMoveArray(ArrayImpl<Value>&& mX) noexcept	{ type = Type::Array;	h.hArray.init(std::move(mX)); }
			inline void setMoveString(String&& mX) noexcept				{ type = Type::String;	h.hString.init(std::move(mX)); }

			inline const auto& getObject() const noexcept	{ SSVU_ASSERT(is<Object>());	return h.hObject.get(); }
			inline const auto& getArray() const noexcept	{ SSVU_ASSERT(is<Array>());		return h.hArray.get(); }
			inline const auto& getString() const noexcept	{ SSVU_ASSERT(is<String>());	return h.hString.get(); }
			inline auto getNumber() const noexcept			{ SSVU_ASSERT(is<Number>());	return h.hNumber; }
			inline auto getBool() const noexcept			{ SSVU_ASSERT(is<Bool>());		return h.hBool; }
			inline auto getNull() const noexcept			{ SSVU_ASSERT(is<Null>());		return Null{}; }

			inline auto getMoveObject() noexcept	{ SSVU_ASSERT(is<Object>());	return std::move(h.hObject.get()); }
			inline auto getMoveArray() noexcept		{ SSVU_ASSERT(is<Array>());		return std::move(h.hArray.get()); }
			inline auto getMoveString() noexcept	{ SSVU_ASSERT(is<String>());	return std::move(h.hString.get()); }

			inline auto& getNonConstObject()  noexcept	{ SSVU_ASSERT(is<Object>());	return h.hObject.get(); }
			inline auto& getNonConstArray() noexcept	{ SSVU_ASSERT(is<Array>());		return h.hArray.get(); }

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

			template<typename T> inline Value(const T& mX) { set<T>(mX); }

			inline Value(const Value& mV) { initCopy(mV); }
			inline Value(Value&& mV) { initMove(std::move(mV)); }

			inline auto& operator=(const Value& mV) { deinitCurrent(); initCopy(mV); return *this; }
			inline auto& operator=(Value&& mV) {deinitCurrent(); initMove(std::move(mV)); return *this; }

			inline ~Value() { deinitCurrent(); }

			// "Explicit" `set` function set the inner contents of the value
			template<typename T> decltype(auto) set(const T& mX)	{ deinitCurrent(); return Internal::ValueHelper<T>::set(*this, mX); }
			template<typename T> decltype(auto) set(T&& mX)			{ deinitCurrent(); return Internal::ValueHelper<T>::set(*this, mX); }

			// "Implicit" `set` function done via `operator=` overloading
			template<typename T> inline ssvu::EnableIf<!ssvu::isSame<ssvu::RemoveAll<T>, Value>> operator=(T&& mX) { set<ssvu::RemoveAll<T>>(ssvu::fwd<T>(mX)); return *this; }

			// "Explicit" `get` function gets the inner contents of the value
			template<typename T> decltype(auto) get() const noexcept { return Internal::ValueHelper<T>::get(*this); }

			// "Implicit" Value from Object by Key getters
			inline auto& operator[](const Key& mKey)				{ return getNonConstObject()[mKey]; }
			inline const auto& operator[](const Key& mKey) const	{ return getObject().at(mKey); }

			// "Implicit" Value from Array by Idx getters
			inline auto& operator[](Idx mIdx)				{ return getNonConstArray()[mIdx]; }
			inline const auto& operator[](Idx mIdx) const	{ return getArray().at(mIdx); }

			inline auto getType() const noexcept { return type; }

			//inline auto getSizeObject() const noexcept { return getObject().size(); }
			//inline auto getSizeArray() const noexcept { return getArray().size(); }

			template<typename T> inline bool is() const noexcept { return Internal::ValueHelper<T>::is(*this); }
	};

	using Object = Value::Object;
	using Array = Value::Array;

	namespace Internal
	{
		#define SVJ_VALUEHELPER_NUMBER(mType) \
			template<> struct ValueHelper<mType> \
			{ \
				inline static decltype(auto) set(Value& mV, const mType& mX) { mV.setNumber(Number{mX}); } \
				inline static decltype(auto) set(Value& mV, mType&& mX) { mV.setNumber(Number{mX}); } \
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

		template<> struct ValueHelper<Object>
		{
			inline static decltype(auto) set(Value& mV, const Object& mX) { mV.setObject(mX); }
			inline static decltype(auto) set(Value& mV, Object&& mX) { mV.setMoveObject(std::move(mX)); }
			inline static decltype(auto) get(const Value& mV) { return mV.getObject(); }
			inline static auto is(const Value& mV) noexcept { return mV.getType() == Value::Type::Object; }
		};

		template<> struct ValueHelper<Array>
		{
			inline static decltype(auto) set(Value& mV, const Array& mX) { mV.setArray(mX); }
			inline static decltype(auto) set(Value& mV, Array&& mX) { mV.setMoveArray(std::move(mX)); }
			inline static decltype(auto) get(const Value& mV) { return mV.getArray(); }
			inline static auto is(const Value& mV) noexcept { return mV.getType() == Value::Type::Array; }
		};

		template<std::size_t TS> struct ValueHelper<char[TS]>
		{
			inline static decltype(auto) set(Value& mV, const char(&mX)[TS]) { mV.setString(mX); }
			inline static decltype(auto) get(const Value& mV) { return mV.getString(); }
			inline static auto is(const Value& mV) noexcept { return mV.getType() == Value::Type::String; }
		};

		template<> struct ValueHelper<String>
		{
			inline static decltype(auto) set(Value& mV, const String& mX) { mV.setString(mX); }
			inline static decltype(auto) set(Value& mV, String&& mX) { mV.setMoveString(std::move(mX)); }
			inline static decltype(auto) get(const Value& mV) { return mV.getString(); }
			inline static auto is(const Value& mV) noexcept { return mV.getType() == Value::Type::String; }
		};

		template<> struct ValueHelper<Number>
		{
			inline static decltype(auto) set(Value& mV, const Number& mX) { mV.setNumber(mX); }
			inline static decltype(auto) get(const Value& mV) { return mV.getNumber(); }
			inline static auto is(const Value& mV) noexcept { return mV.getType() == Value::Type::Number; }
		};

		template<> struct ValueHelper<Bool>
		{
			inline static decltype(auto) set(Value& mV, Bool mX) { mV.setBool(mX); }
			inline static decltype(auto) get(const Value& mV) { return mV.getBool(); }
			inline static auto is(const Value& mV) noexcept { return mV.getType() == Value::Type::Bool; }
		};

		template<> struct ValueHelper<Null>
		{
			inline static decltype(auto) set(Value& mV, Null) { mV.setNull(); }
			inline static decltype(auto) get(const Value& mV) { return mV.getNull(); }
			inline static auto is(const Value& mV) noexcept { return mV.getType() == Value::Type::Null; }
		};
	}
}

#endif
