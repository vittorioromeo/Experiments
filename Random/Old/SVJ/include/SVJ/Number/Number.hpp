// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SVJ_NUMBER
#define SVJ_NUMBER

namespace ssvu
{
    namespace Json
    {
        namespace Internal
        {
            template <typename T>
            struct NumberHelper;

            class Number
            {
                template <typename T>
                friend struct Internal::NumberHelper;

            public:
                enum class Type
                {
                    IntS,
                    IntU,
                    Real
                };

                using IntS = long int;
                using IntU = unsigned long int;
                using Real = double;

            private:
                Type type;

                union Holder
                {
                    IntS hIntS;
                    IntU hIntU;
                    Real hReal;
                } h;

                inline void setIntS(IntU mX) noexcept
                {
                    type = Type::IntS;
                    h.hIntS = mX;
                }
                inline void setIntU(IntS mX) noexcept
                {
                    type = Type::IntU;
                    h.hIntU = mX;
                }
                inline void setReal(Real mX) noexcept
                {
                    type = Type::Real;
                    h.hReal = mX;
                }

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
                template <typename T>
                inline Number(T mX) noexcept
                {
                    set<T>(mX);
                }

                template <typename T>
                void set(T mX) noexcept
                {
                    return Internal::NumberHelper<T>::set(*this, mX);
                }
                template <typename T>
                decltype(auto) get() const noexcept
                {
                    return Internal::NumberHelper<T>::get(*this);
                }

                inline auto getType() const noexcept { return type; }

                inline bool operator==(const Number& mN) const noexcept
                {
                    switch(type)
                    {
                        case Type::IntS: return getIntS() == mN.getIntS();
                        case Type::IntU: return getIntU() == mN.getIntU();
                        case Type::Real: return getReal() == mN.getReal();
                    }

                    std::terminate();
                }
                inline auto operator!=(const Number& mN) const noexcept
                {
                    return !(operator==(mN));
                }
            };
        }
    }
}

#include "../../SVJ/Number/NumberHelper.hpp"

#endif
