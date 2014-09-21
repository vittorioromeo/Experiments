// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef CESYSTEM_SYSTEMBASE
#define CESYSTEM_SYSTEMBASE

namespace ssvces
{
	namespace Internal
	{
		class SystemBase : ssvu::NoCopy
		{
			friend bool matchesSystem(const TypeIdxBitset&, const SystemBase&) noexcept;
			friend ssvces::Manager;

			private:
				TypeIdxBitset typeIdsReq, typeIdsNot;

			protected:
				inline SystemBase(TypeIdxBitset mTypeIdsReq) : typeIdsReq{std::move(mTypeIdsReq)} { }
				inline SystemBase(TypeIdxBitset mTypeIdsReq, TypeIdxBitset mTypeIdsNot) : typeIdsReq{std::move(mTypeIdsReq)}, typeIdsNot{std::move(mTypeIdsNot)} { }
				inline virtual ~SystemBase() noexcept { }

				virtual void registerEntity(Entity&) = 0;
				virtual void refresh() = 0;
		};
	}
}

#endif
