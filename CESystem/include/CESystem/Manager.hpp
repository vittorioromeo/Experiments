// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef CESYSTEM_MANAGER
#define CESYSTEM_MANAGER

namespace ssvces
{
	class Manager
	{
		friend Entity;
		friend EntityHandle;

		private:
			EntityRecycler entityRecycler;
			ComponentRecycler componentRecycler;

			Impl::IdPool entityIdPool;
			std::vector<Impl::SystemBase*> systems;
			std::vector<EntityRecyclerPtr> entities;
			std::array<std::vector<Entity*>, maxGroups> grouped;

			inline auto& create(Manager& mManager, Impl::IdPool& mIdPool)
			{
				return entityRecycler.getCreateEmplace(entities, mManager, mIdPool.getAvailable());
			}

			inline void addToGroup(Entity* mEntity, Group mGroup) { SSVU_ASSERT(mGroup <= maxGroups); grouped[mGroup].emplace_back(mEntity); }

		public:
			inline Manager() = default;

			inline Manager(const Manager&) = delete;
			inline Manager& operator=(const Manager&) = delete;

			inline void refresh()
			{
				for(auto& s : systems) s->refresh();

				for(auto i(0u); i < maxGroups; ++i) ssvu::eraseRemoveIf(grouped[i], [i](const Entity* mEntity){ return mEntity->mustDestroy || !mEntity->hasGroup(i); });

				// This loop below is roughly implemented like std::remove_if
				auto itr(std::begin(entities)), last(std::end(entities)), result(itr);
				for(; itr != last; ++itr)
				{
					auto& e(**itr);

					if(e.mustDestroy) continue;
					if(e.mustRematch)
					{
						for(auto& s : systems) if(Impl::matchesSystem(e.typeIds, *s)) s->registerEntity(e);
						e.mustRematch = false;
					}

					*result++ = std::move(*itr);
				}
				entities.erase(result, last);
			}

			inline EntityHandle createEntity() { return {create(*this, entityIdPool)}; }
			template<typename T> inline void registerSystem(T& mSystem)
			{
				SSVU_ASSERT_STATIC(ssvu::isBaseOf<Impl::SystemBase, T>(), "`T` must derive from `SystemBase`");
				systems.emplace_back(&mSystem);
			}

			inline const decltype(entities)& getEntities() const noexcept				{ return entities; }
			inline decltype(entities)& getEntities() noexcept							{ return entities; }
			inline const std::vector<Entity*>& getEntities(Group mGroup) const noexcept	{ SSVU_ASSERT(mGroup <= maxGroups); return grouped[mGroup]; }
			inline std::vector<Entity*>& getEntities(Group mGroup) noexcept				{ SSVU_ASSERT(mGroup <= maxGroups); return grouped[mGroup]; }
			inline std::vector<EntityHandle> getEntityHandles(Group mGroup) noexcept
			{
				std::vector<EntityHandle> result;
				for(const auto& e : getEntities(mGroup)) result.emplace_back(*e);
				return result;
			}

			inline bool hasEntity(Group mGroup) const noexcept			{ return !getEntities(mGroup).empty(); }
			inline SizeT getEntityCount() const noexcept				{ return entities.size(); }
			inline SizeT getEntityCount(Group mGroup) const noexcept	{ return getEntities(mGroup).size(); }

			// Have a manager.componentCount too?
			inline SizeT getComponentCount() const noexcept			{ SizeT result{0}; for(auto& e : getEntities()) result += e->componentCount; return result; }
	};

	namespace Impl
	{
		inline bool matchesSystem(const TypeIdxBitset& mTypeIds, const SystemBase& mSystem) noexcept
		{
			return (mTypeIds & mSystem.typeIdsNot).none() && containsAll(mTypeIds, mSystem.typeIdsReq);
		}
	}
}

#endif
