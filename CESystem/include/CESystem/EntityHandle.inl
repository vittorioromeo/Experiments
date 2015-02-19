// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef CESYSTEM_ENTITYHANDLE_INL
#define CESYSTEM_ENTITYHANDLE_INL

namespace ssvces
{
	inline bool EntityHandle::isAlive() const noexcept { return manager.entityIdPool.isAlive(stat); }
}

#endif
