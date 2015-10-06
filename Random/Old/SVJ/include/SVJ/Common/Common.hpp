// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SVJ_COMMON
#define SVJ_COMMON

#define SVJ_ENABLE_IF_IS(mT, mType) \
    EnableIf<isSame<RemoveAll<mT>, mType>()>* = nullptr
#define SVJ_ENABLE_IF_IS_NOT(mT, mType) \
    EnableIf<!isSame<RemoveAll<mT>, mType>()>* = nullptr

#include "../../SVJ/Common/VecMap.hpp"

namespace ssvu
{
    namespace Json
    {
        using Idx = std::size_t;
        using Key = std::string;
        using String = std::string;
        using Bool = bool;

        namespace Internal
        {
            template <typename T>
            using ObjectImpl = Internal::VecMap<Key, T>;
            template <typename T>
            using ArrayImpl = std::vector<T>;
        }

        struct Null
        {
        };

        enum class WriterMode
        {
            Pretty,
            Minified
        };
    }
}

namespace svj = ssvu::Json;

#endif
