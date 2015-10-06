// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVVM_BOUNDFUNCTION
#define SSVVM_BOUNDFUNCTION

namespace ssvvm
{
    namespace Impl
    {
        template <std::size_t TIdx, typename TArg, typename T>
        inline static void makeParamsTuple(T& mTpl, const Params& mParams)
        {
            std::get<TIdx>(mTpl) = mParams[TIdx].template get<TArg>();
        }
        template <std::size_t TIdx, typename TArg1, typename TArg2,
            typename... TArgs, typename T>
        inline static void makeParamsTuple(T& mTpl, const Params& mParams)
        {
            makeParamsTuple<TIdx, TArg1>(mTpl, mParams);
            makeParamsTuple<TIdx + 1, TArg2, TArgs...>(mTpl, mParams);
        }

        template <std::size_t TIdx, typename TArg, typename T>
        inline static void bfArrayFillHelper(T& mArray)
        {
            mArray[TIdx] = getVMVal<TArg>();
        }
        template <std::size_t TIdx, typename TArg1, typename TArg2,
            typename... TArgs, typename T>
        inline static void bfArrayFillHelper(T& mArray)
        {
            bfArrayFillHelper<TIdx, TArg1>(mArray);
            bfArrayFillHelper<TIdx + 1, TArg2, TArgs...>(mArray);
        }


        struct CFunctionBase
        {
            inline virtual Value call(const Params&) { throw; }
            inline virtual ~CFunctionBase() {}
        };

        template <typename TReturn, typename... TArgs>
        struct CFunction;
        template <typename TReturn, typename... TArgs>
        struct CFunction<TReturn(TArgs...)> : public CFunctionBase
        {
            using Ptr = TReturn (*)(TArgs...);

            Ptr ptr;
            inline CFunction(Ptr mPtr) noexcept : ptr{mPtr} {}
            inline Value call(const Params& mParams) override
            {
                ssvu::Tpl<TArgs...> paramsTuple;
                makeParamsTuple<0, TArgs...>(paramsTuple, mParams);
                return ssvu::explode(*ptr, paramsTuple);
            }
        };
        template <typename... TArgs>
        struct CFunction<void(TArgs...)> : public CFunctionBase
        {
            using Ptr = void (*)(TArgs...);

            Ptr ptr;
            inline CFunction(Ptr mPtr) noexcept : ptr{mPtr} {}
            inline Value call(const Params& mParams) override
            {
                ssvu::Tpl<TArgs...> paramsTuple;
                makeParamsTuple<0, TArgs...>(paramsTuple, mParams);
                ssvu::explode(*ptr, paramsTuple);
                return {};
            }
        };
    }

    class BoundFunction
    {
    private:
        VMVal returnType;
        std::array<VMVal, Params::valueCount> paramTypes;
        ssvu::UPtr<Impl::CFunctionBase> cFunction;

    public:
        template <typename TReturn, typename... TArgs>
        inline BoundFunction(TReturn (*mFnPtr)(TArgs...))
            : returnType{getVMVal<TReturn>()},
              cFunction{ssvu::UPtr<Impl::CFunctionBase>(
                  new Impl::CFunction<TReturn(TArgs...)>(mFnPtr))}
        {
            Impl::bfArrayFillHelper<0, TArgs...>(paramTypes);
        }

        inline Value call(const Params& mParams)
        {
            return cFunction->call(mParams);
        }
    };
}

#endif
