#define SSVU_DEBUG 1

#include <SSVUtils/Core/Core.hpp>
#include <SSVUtils/UnionVariant/UnionVariant.hpp>

namespace ssvu
{
    namespace Impl
    {
        template <typename T, template <typename...> class TUV>
        class MaybeImpl
        {
        protected:
            TUV<T> uv;

        public:
            template <typename... TArgs>
            inline void init(TArgs&&... mArgs) noexcept(
                noexcept(uv.template init<T>(SSVU_FWD(mArgs)...)))
            {
                uv.template init<T>(SSVU_FWD(mArgs)...);
            }

            inline T& get() & noexcept { return uv.template get<T>(); }
            inline const T& get() const& noexcept
            {
                return uv.template get<T>();
            }
            inline const T get() && noexcept
            {
                return std::move(uv.template get<T>());
            }
        };
    }

    template <typename T>
    struct Maybe : public Impl::MaybeImpl<T, UnionVariant>
    {
        inline void deinit() noexcept(isNothrowDtor<T>())
        {
            this->uv.template deinit<T>();
        }
    };

    template <typename T>
    using MaybePOD = Impl::MaybeImpl<T, UnionVariantPOD>;
}

int main()
{
    using namespace ssvu;

    Maybe<int> mbInt;
    Maybe<std::vector<int>> mbVecInt;

    mbInt.init(3);
    mbInt.deinit();

    MaybePOD<int> mbpInt;
    mbpInt.init(5);

    return 0;
}