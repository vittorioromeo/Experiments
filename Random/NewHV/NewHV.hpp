// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_NEWHV
#define SSVU_NEWHV

#define SSVU_API_HEADERONLY 1

#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/Range/Range.hpp"
#include "SSVUtils/GrowableArray/GrowableArray.hpp"
#include "SSVUtils/UnionVariant/Internal/CTUtils.hpp"
#include "SSVUtils/Internal/SharedFuncs.hpp"

#include "../NewHV/Inc/Common.hpp"
#include "../NewHV/Inc/Fwd.hpp"
#include "../NewHV/Inc/Mark.hpp"
#include "../NewHV/Inc/Stat.hpp"
#include "../NewHV/Inc/Handle/HandleBase.hpp"
#include "../NewHV/Inc/Handle/HandleSingle.hpp"
#include "../NewHV/Inc/Handle/HandleMulti.hpp"
#include "../NewHV/Inc/Iterator.hpp"
#include "../NewHV/Inc/HV/HVImpl.hpp"
#include "../NewHV/Inc/HV/HVSingle.hpp"
#include "../NewHV/Inc/HV/HVMulti.hpp"

#include "../NewHV/Inc/Handle/HandleBase.inl"

#if defined(SSVU_API_HEADERONLY)
	#include "../NewHV/Src/Mark.cpp"
	#include "../NewHV/Src/Stat.cpp"
#endif

#endif
