// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef CESYSTEM_COMMON
#define CESYSTEM_COMMON

namespace ssvces
{
	// Component class declaration and definition
	struct Component { inline virtual ~Component() noexcept { } };

	// Forward declarations
	class Entity;
	class Manager;
	class EntityHandle;
	template<typename, typename, typename> class System;

	// Constants
	static constexpr std::size_t maxEntities{1'000'000};
	static constexpr std::size_t maxComponents{32};
	static constexpr std::size_t maxGroups{32};

	// Entity typedefs
	using EntityId = int;
	using EntityIdCtr = int;
	struct EntityStat { EntityId id; EntityIdCtr ctr; };

	// Type index typedefs
	using TypeIdx = std::size_t;
	using TypeIdxBitset = std::bitset<maxComponents>;

	// Group typedefs
	using Group = std::size_t;
	using GroupBitset = std::bitset<maxGroups>;

	// Recycler typedefs
	using EntityRecycler = ssvu::MonoRecycler<Entity>;
	using EntityRecyclerPtr = EntityRecycler::PtrType;
	using ComponentRecycler = ssvu::PolyFixedRecycler<Component, maxComponents>;
	using ComponentRecyclerPtr = ComponentRecycler::PtrType;

	namespace Impl
	{
		class SystemBase;

		// Returns the next unique bit index for a type
		inline TypeIdx getLastTypeIdx() noexcept
		{
			static TypeIdx lastIdx{0};
			SSVU_ASSERT(lastIdx < maxComponents);
			return lastIdx++;
		}

		// Stores a specific bit index for a Component type
		template<typename T> struct TypeIdxInfo { static TypeIdx idx; };
		template<typename T> TypeIdx TypeIdxInfo<T>::idx{getLastTypeIdx()};

		// Shortcut to get the bit index of a Component type
		template<typename T> inline const TypeIdx& getTypeIdx() noexcept
		{
			SSVU_ASSERT_STATIC(ssvu::isBaseOf<Component, T>(), "`T` must derive from `Component`");
			return TypeIdxInfo<T>::idx;
		}

		// These functions use variadic template recursion to "build" a bitset for a set of Component types
		template<typename T> inline void buildBitsetHelper(TypeIdxBitset& mBitset) noexcept { mBitset[getTypeIdx<T>()] = true; }
		template<typename T1, typename T2, typename... TArgs> inline void buildBitsetHelper(TypeIdxBitset& mBitset) noexcept { buildBitsetHelper<T1>(mBitset); buildBitsetHelper<T2, TArgs...>(mBitset); }
		template<typename... TArgs> inline TypeIdxBitset getBuildBitset() noexcept { TypeIdxBitset result; buildBitsetHelper<TArgs...>(result); return result; }
		template<> inline TypeIdxBitset getBuildBitset<>() noexcept { static TypeIdxBitset nullBitset; return nullBitset; }

		SSVU_DEFINE_MEMFN_CALLER(callAdded, added, void(TArgs...))		// `callAdded(...)` only calls `T::added` if it exists
		SSVU_DEFINE_MEMFN_CALLER(callRemoved, removed, void(TArgs...))	// `callRemoved(...)` only calls `T::removed` if it exists

		// Shortcut to get the static Bitset of a pack of Component types
		template<typename... TArgs> inline const TypeIdxBitset& getTypeIdxBitset() noexcept { static TypeIdxBitset bitset{Impl::getBuildBitset<TArgs...>()}; return bitset; }

		// Returns whether the first bitset contains all the value of the second one
		inline bool containsAll(const TypeIdxBitset& mA, const TypeIdxBitset& mB) noexcept { return (mA & mB) == mB; }

		// Returns whether a type id bitset matches a system's type id bitset
		inline bool matchesSystem(const TypeIdxBitset& mTypeIds, const SystemBase& mSystem) noexcept;
	}
}

#endif
