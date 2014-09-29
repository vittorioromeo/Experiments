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
				template<typename T> inline void setObject(T&& mX)	{ type = Type::Object;	h.hObject.init(fwd<T>(mX)); }
				template<typename T> inline void setArray(T&& mX)	{ type = Type::Array;	h.hArray.init(fwd<T>(mX)); }
				template<typename T> inline void setString(T&& mX)	{ type = Type::String;	h.hString.init(fwd<T>(mX)); }
				inline void setNumber(const Number& mX) noexcept	{ type = Type::Number;	h.hNumber = mX; }
				inline void setBool(Bool mX) noexcept				{ type = Type::Bool;	h.hBool = mX; }
				inline void setNull(Null) noexcept					{ type = Type::Null; }

				inline auto& getObject() & noexcept				{ SSVU_ASSERT(is<Object>());	return h.hObject.get(); }
				inline auto& getArray() & noexcept				{ SSVU_ASSERT(is<Array>());		return h.hArray.get(); }
				inline auto& getString() & noexcept				{ SSVU_ASSERT(is<String>());	return h.hString.get(); }
				inline const auto& getObject() const& noexcept	{ SSVU_ASSERT(is<Object>());	return h.hObject.get(); }
				inline const auto& getArray() const& noexcept	{ SSVU_ASSERT(is<Array>());		return h.hArray.get(); }
				inline const auto& getString() const& noexcept	{ SSVU_ASSERT(is<String>());	return h.hString.get(); }
				inline auto getObject() && noexcept				{ SSVU_ASSERT(is<Object>());	return std::move(std::move(h.hObject).get()); }
				inline auto getArray() && noexcept				{ SSVU_ASSERT(is<Array>());		return std::move(std::move(h.hArray).get()); }
				inline auto getString() && noexcept				{ SSVU_ASSERT(is<String>());	return std::move(std::move(h.hString).get()); }

				inline auto getNumber() const noexcept			{ SSVU_ASSERT(is<Number>());	return h.hNumber; }
				inline auto getBool() const noexcept			{ SSVU_ASSERT(is<Bool>());		return h.hBool; }
				inline auto getNull() const noexcept			{ SSVU_ASSERT(is<Null>());		return Null{}; }


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

				template<typename T> inline void init(T&& mV)
				{
					type = mV.type;

					switch(type)
					{
						case Type::Object:	h.hObject.init(fwd<T>(mV).getObject()); break;
						case Type::Array:	h.hArray.init(fwd<T>(mV).getArray()); break;
						case Type::String:	h.hString.init(fwd<T>(mV).getString()); break;
						case Type::Number:	h.hNumber = fwd<T>(mV).getNumber(); break;
						case Type::Bool:	h.hBool = fwd<T>(mV).getBool(); break;
						default: break;
					}
				}

			public:
				inline Value() = default;
				inline Value(const Value& mV)	{ init(mV); }
				inline Value(Value&& mV)		{ init(std::move(mV)); }

				template<typename T, SVJ_ENABLE_IF_IS_NOT(T, Value)> inline Value(T&& mX) { set(fwd<T>(mX)); }

				inline ~Value() { deinitCurrent(); }

				// "Explicit" `set` function set the inner contents of the value
				template<typename T> inline void set(T&& mX) { deinitCurrent(); Internal::ValueHelper<RemoveAll<T>>::set(*this, fwd<T>(mX)); }

				// "Implicit" `set` function done via `operator=` overloading
				template<typename T> inline auto& operator=(T&& mX) { set(fwd<T>(mX)); return *this; }

				// "Explicit" `get` function gets the inner contents of the value
				template<typename T> decltype(auto) get() & noexcept		{ return Internal::ValueHelper<T>::get(*this); }
				template<typename T> decltype(auto) get() const& noexcept	{ return Internal::ValueHelper<T>::get(*this); }
				template<typename T> decltype(auto) get() && noexcept		{ return Internal::ValueHelper<T>::get(std::move(*this)); }

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
