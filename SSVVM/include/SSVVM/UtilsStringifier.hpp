// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVVM_UTILS_STRINGIFIER
#define SSVVM_UTILS_STRINGIFIER

namespace ssvu
{
    template <>
    struct Stringifier<ssvvm::Value>
    {
        template <bool TFmt>
        inline static void impl(
            std::ostream& mStream, const ssvvm::Value& mValue)
        {
            if(mValue.getType() == ssvvm::VMVal::Void)
            {
                Impl::printBold<TFmt>(mStream, "VOID[");
            }
            else if(mValue.getType() == ssvvm::VMVal::Int)
            {
                Impl::printBold<TFmt>(mStream, "INT[");
                Impl::callStringifyImpl<TFmt>(mStream, mValue.get<int>());
            }
            else if(mValue.getType() == ssvvm::VMVal::Float)
            {
                Impl::printBold<TFmt>(mStream, "FLOAT[");
                Impl::callStringifyImpl<TFmt>(mStream, mValue.get<float>());
            }

            Impl::printBold<TFmt>(mStream, "]");
        }
    };
}

#endif
