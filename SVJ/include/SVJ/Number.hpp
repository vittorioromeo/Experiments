// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SVJ_NUMBER
#define SVJ_NUMBER

namespace svj
{
	namespace Internal
	{
		template<typename T> struct NumberHelper;
		class Writer;
	}

	class Number
	{
		template<typename T> friend struct Internal::NumberHelper;
		friend class Internal::Writer;

		public:
			enum class Type{IntS, IntU, Real};

			using IntS = int;
			using IntU = unsigned int;
			using Real = double;

		private:
			Type type;

			union Holder
			{
				IntS hIntS;
				IntU hIntU;
				Real hReal;
			} h;

			inline void setIntS(IntU mValue) noexcept { type = Type::IntS; h.hIntS = mValue; }
			inline void setIntU(IntS mValue) noexcept { type = Type::IntU; h.hIntU = mValue; }
			inline void setReal(Real mValue) noexcept { type = Type::Real; h.hReal = mValue; }

			inline IntS getIntS() const noexcept
			{
				switch(type)
				{
					case Type::IntS: return h.hIntS;
					case Type::IntU: return static_cast<IntS>(h.hIntU);
					case Type::Real: return static_cast<IntS>(h.hReal);
				}
			}

			inline IntU getIntU() const noexcept
			{
				switch(type)
				{
					case Type::IntS: return static_cast<IntU>(h.hIntS);
					case Type::IntU: return h.hIntU;
					case Type::Real: return static_cast<IntU>(h.hReal);
				}
			}

			inline Real getReal() const noexcept
			{
				switch(type)
				{
					case Type::IntS: return static_cast<Real>(h.hIntS);
					case Type::IntU: return static_cast<Real>(h.hIntU);
					case Type::Real: return h.hReal;
				}
			}

		public:
			inline Number() noexcept = default;
			inline Number(IntS mValue) noexcept { setIntS(mValue); }
			inline Number(IntU mValue) noexcept { setIntU(mValue); }
			inline Number(Real mValue) noexcept { setReal(mValue); }

			template<typename T> decltype(auto) set(T mValue) noexcept	{ return Internal::NumberHelper<T>::set(*this, mValue); }
			template<typename T> decltype(auto) get() const noexcept	{ return Internal::NumberHelper<T>::get(*this); }

			inline auto getType() const noexcept { return type; }
	};

	namespace Internal
	{
		template<> struct NumberHelper<char>
		{
			inline static void set(Number& mN, char mV) noexcept { return mN.setIntS(mV); }
			inline static auto get(const Number& mN) noexcept { return mN.getIntS(); }
		};

		template<> struct NumberHelper<unsigned char>
		{
			inline static void set(Number& mN, unsigned char mV) noexcept { return mN.setIntU(mV); }
			inline static auto get(const Number& mN) noexcept { return mN.getIntU(); }
		};

		template<> struct NumberHelper<int>
		{
			inline static void set(Number& mN, int mV) noexcept { return mN.setIntS(mV); }
			inline static auto get(const Number& mN) noexcept { return mN.getIntS(); }
		};

		template<> struct NumberHelper<unsigned int>
		{
			inline static void set(Number& mN, unsigned int mV) noexcept { return mN.setIntU(mV); }
			inline static auto get(const Number& mN) noexcept { return mN.getIntU(); }
		};

		template<> struct NumberHelper<float>
		{
			inline static void set(Number& mN, float mV) noexcept { return mN.setReal(mV); }
			inline static auto get(const Number& mN) noexcept { return mN.getReal(); }
		};

		template<> struct NumberHelper<double>
		{
			inline static void set(Number& mN, double mV) noexcept { return mN.setReal(mV); }
			inline static auto get(const Number& mN) noexcept { return mN.getReal(); }
		};
	}
}

#endif
