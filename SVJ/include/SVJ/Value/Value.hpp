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

				using Object = Internal::ObjectImpl<Value>;
				using Array = Internal::ArrayImpl<Value>;

			public: // private:
				using Number = Internal::Number;

				Type type{Type::Null};

				union Holder
				{
					Internal::Maybe<Object> hObject;
					Internal::Maybe<Array> hArray;
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
				inline void setObject(const Object& mX)				{ type = Type::Object;	h.hObject.init(mX); }
				inline void setArray(const Array& mX)				{ type = Type::Array;	h.hArray.init(mX); }
				inline void setString(const String& mX)				{ type = Type::String;	h.hString.init(mX); }
				inline void setNumber(const Number& mX) noexcept	{ type = Type::Number;	h.hNumber = mX; }
				inline void setBool(Bool mX) noexcept				{ type = Type::Bool;	h.hBool = mX; }
				inline void setNull(Null) noexcept					{ type = Type::Null; }

				inline void setObject(Object&& mX) noexcept	{ type = Type::Object;	h.hObject.init(std::move(mX)); }
				inline void setArray(Array&& mX) noexcept	{ type = Type::Array;	h.hArray.init(std::move(mX)); }
				inline void setString(String&& mX) noexcept	{ type = Type::String;	h.hString.init(std::move(mX)); }

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

				inline auto& operator=(const Value& mV) { set(mV); return *this; }
				inline auto& operator=(Value&& mV) { set(mV); return *this; }

				// "Implicit" `set` function done via `operator=` overloading
				template<typename T> inline auto& operator=(T&& mX) { set(fwd<T>(mX)); return *this; }

				// "Explicit" `get` function gets the inner contents of the value
				template<typename T> decltype(auto) get() noexcept { return Internal::ValueHelper<T>::get(*this); }
				template<typename T> decltype(auto) get() const noexcept { return Internal::ValueHelper<T>::get(*this); }

				// "Implicit" Value from Object by Key getters
				inline auto& operator[](const Key& mKey)				{ return getObject()[mKey]; }
				inline const auto& operator[](const Key& mKey) const	{ return getObject().at(mKey); }

				// "Implicit" Value from Array by Idx getters
				inline auto& operator[](Idx mIdx)				{ return getArray()[mIdx]; }
				inline const auto& operator[](Idx mIdx) const	{ return getArray().at(mIdx); }

				inline auto getType() const noexcept { return type; }

				template<typename T> inline bool is() const noexcept { return Internal::ValueHelper<T>::is(*this); }

				inline bool operator==(const Value& mV) const noexcept
				{
					if(type != mV.type) return false;

					switch(type)
					{
						case Type::Object:	return getObject() == mV.getObject();
						case Type::Array:	return getArray() == mV.getArray();
						case Type::String:	return getString() == mV.getString();
						case Type::Number:	return getNumber() == mV.getNumber();
						case Type::Bool:	return getBool() == mV.getBool();
						case Type::Null:	return true;
					}

					std::terminate();
				}
				inline auto operator!=(const Value& mV) const noexcept { return !(operator==(mV)); }

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
	}
}

#include "../../SVJ/Value/ValueHelper.hpp"

#endif
