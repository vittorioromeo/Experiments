#ifndef ALGOVEE_AVZ_FNS_INL
#define ALGOVEE_AVZ_FNS_INL

namespace avz
{
    template <typename T>
    inline void Internal::Widget::render(const T& mX)
    {
        ctx->render(mX);
    }
    template <typename... TArgs>
    inline Internal::TimedAction& Internal::Widget::createTA(TArgs&&... mArgs)
    {
        return ctx->createTA(ssvu::fwd<TArgs>(mArgs)...);
    }
    template <typename T, typename... TArgs>
    inline T& Internal::Widget::create(TArgs&&... mArgs)
    {
        return ctx->wRecycler.getCreateEmplace<T>(
            children, *ctx, ssvu::fwd<TArgs>(mArgs)...);
    }

    inline void Internal::Widget::simultaneously() { ctx->simultaneously(); }
    inline void Internal::Widget::makeTACtx() { ctx->makeTACtx(); }
    inline void Internal::Widget::restoreTACtx() { ctx->restoreTACtx(); }
}

#endif