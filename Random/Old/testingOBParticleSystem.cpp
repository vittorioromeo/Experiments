// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVOB_PARTICLES_PARTICLESYSTEM
#define SSVOB_PARTICLES_PARTICLESYSTEM

#include "SSVBloodshed/Particles/OBParticle.hpp"
#include "SSVBloodshed/OBCommon.hpp"
#include "SSVBloodshed/OBConfig.hpp"

namespace ob
{
	namespace t
	{
		using Idx = std::size_t;
		using Ctr = int;

		template<typename> class HManager;

		namespace Internal
		{
			template<typename T> class Uncertain
			{
				private:
					ssvu::AlignedStorageBasic<T> storage;

				public:
					template<typename... TArgs> inline void init(TArgs&&... mArgs) noexcept(ssvu::isNothrowConstructible<T>())
					{
						new (&storage) T(std::forward<TArgs>(mArgs)...);
					}
					inline void deinit() noexcept(ssvu::isNothrowDestructible<T>()) { get().~T(); }

					inline T& get() noexcept 				{ return reinterpret_cast<T&>(storage); }
					inline const T& get() const noexcept 	{ return reinterpret_cast<const T&>(storage); }
			};

			template<typename T> class Atom
			{
				template<typename> friend class ::HManager;

				private:
					Uncertain<T> data;
					Idx markIdx;
					bool alive{false};

					// Initializes the internal data
					template<typename... TArgs> inline void initData(TArgs&&... mArgs)
						noexcept(noexcept(data.init(std::forward<TArgs>(mArgs)...)))
					{
						SSVU_ASSERT(!alive);
						data.init(std::forward<TArgs>(mArgs)...);
					}

					// Deinitializes the internal data
					inline void deinitData() noexcept(noexcept(data.deinit()))
					{
						SSVU_ASSERT(!alive);
						data.deinit();
					}

				public:
					inline Atom() = default;
					inline Atom(Atom&&) = default;
					inline Atom& operator=(Atom&&) = default;

					inline T& getData() noexcept 				{ SSVU_ASSERT(alive); return data.get(); }
					inline const T& getData() const noexcept 	{ SSVU_ASSERT(alive); return data.get(); }
					inline void setDead() noexcept 				{ alive = false; }

					// Disallow copies
					inline Atom(const Atom&) = delete;
					inline Atom& operator=(const Atom&) = delete;
			};
		}

		template<typename T> class Handle
		{
			template<typename> friend class HManager;

			public:
				using AtomType = typename Internal::Atom<T>;

			private:
				HManager<T>* manager;
				Idx markIdx;
				Ctr ctr;

				inline Handle(HManager<T>& mManager, Idx mMarkIdx, Ctr mCtr) noexcept
					: manager(&mManager), markIdx{mMarkIdx}, ctr{mCtr} { }

				template<typename TT> inline TT getAtomImpl() noexcept
				{
					SSVU_ASSERT(isAlive());
					return manager->getAtomFromMark(manager->marks[markIdx]);
				}

			public:
				inline AtomType& getAtom() noexcept 			{ return getAtomImpl<AtomType&>(); }
				inline const AtomType& getAtom() const noexcept { return getAtomImpl<const AtomType&>(); }
				inline T& get() noexcept						{ return getAtom().getData(); }
				inline const T& get() const noexcept			{ return getAtom().getData(); }
				bool isAlive() const noexcept;
				void destroy() noexcept;

				inline T& operator*() noexcept 				{ return get(); }
				inline const T& operator*() const noexcept 	{ return get(); }
				inline T* operator->() noexcept 			{ return &(get()); }
				inline const T* operator->() const noexcept { return &(get()); }
		};

		template<typename T> class HManager
		{
			template<typename> friend class Handle;

			private:
				struct Mark { Idx atomIdx; Ctr ctr; };

			public:
				using AtomType = typename Internal::Atom<T>;

			private:
				std::vector<AtomType> atoms;
				std::vector<Mark> marks;
				Idx size{0u}, sizeNext{0u};

				inline std::size_t getCapacity() const noexcept { return atoms.size(); }

				inline void growCapacityBy(std::size_t mAmount)
				{
					auto i(getCapacity()), newCapacity(getCapacity() + mAmount);
					SSVU_ASSERT(newCapacity >= 0 && newCapacity >= getCapacity());

					atoms.resize(newCapacity);
					marks.resize(newCapacity);

					// Initialize resized storage
					for(; i < newCapacity; ++i) atoms[i].markIdx = marks[i].atomIdx = i;
				}

				inline void growCapacityTo(std::size_t mCapacity)
				{
					SSVU_ASSERT(getCapacity() < mCapacity);
					growCapacityBy(mCapacity - getCapacity());
				}

				inline void growIfNeeded()
				{
					constexpr float growMultiplier{2.f};
					constexpr std::size_t growAmount{5};

					if(getCapacity() <= sizeNext) growCapacityTo((getCapacity() + growAmount) * growMultiplier);
				}

				inline void destroy(Idx mMarkIdx) noexcept
				{
					getAtomFromMark(marks[mMarkIdx]).setDead();
				}

				inline Mark& getMarkFromAtom(const AtomType& mAtom)	noexcept { return marks[mAtom.markIdx]; }
				inline AtomType& getAtomFromMark(const Mark& mMark) noexcept { return atoms[mMark.atomIdx]; }

			public:
				inline HManager() = default;
				inline ~HManager() { clear(); }

				inline void clear() noexcept
				{
					refresh();

					for(auto i(0u); i < size; ++i)
					{
						auto& atom(atoms[i]);
						auto& mark(marks[i]);

						SSVU_ASSERT(atom.alive);
						atom.alive = false;
						atom.deinitData();
						++mark.ctr;
					}

					size = sizeNext = 0u;
				}

				inline void reserve(std::size_t mCapacity) { if(getCapacity() < mCapacity) growCapacityTo(mCapacity); }

				inline Handle<T> createHandleFromAtom(AtomType& mAtom) noexcept
				{
					return {*this, mAtom.markIdx, getMarkFromAtom(mAtom).ctr};
				}

				template<typename... TArgs> inline AtomType& createAtom(TArgs&&... mArgs)
				{
					// `sizeNext` may be greater than the sizes of the vectors - resize vectors if needed
					growIfNeeded();

					// `sizeNext` now is the first empty valid index - we create our atom there
					auto& atom(atoms[sizeNext]);
					atom.initData(std::forward<TArgs>(mArgs)...);
					atom.alive = true;

					// Update the mark
					auto& mark(getMarkFromAtom(atom));
					mark.atomIdx = sizeNext;

					// Update next size
					++sizeNext;

					return atom;
				}

				template<typename... TArgs> inline Handle<T> create(TArgs&&... mArgs)
				{
					return createHandleFromAtom(createAtom(std::forward<TArgs>(mArgs)...));
				}

				inline void refresh()
				{
					// Type must be signed, to check with negative values later
					int iDead{0};

					// Convert sizeNext to int to avoid warnings/issues
					const int intSizeNext(sizeNext);

					// Find first alive and first dead atoms
					while(iDead < intSizeNext && atoms[iDead].alive) ++iDead;
					int iAlive{iDead - 1};

					for(int iD{iDead}; iD < intSizeNext; ++iD)
					{
						// Skip alive atoms
						if(atoms[iD].alive) continue;

						// Found a dead atom - `i` now stores its index
						// Look for an alive atom after the dead atom
						for(int iA{iDead + 1}; true; ++iA)
						{
							// No more alive atoms, continue
							if(iA == intSizeNext) goto finishRefresh;

							// Skip dead atoms
							if(!atoms[iA].alive) continue;

							// Found an alive atom after dead `iD` atom - swap and update mark
							std::swap(atoms[iA], atoms[iD]);
							getMarkFromAtom(atoms[iD]).atomIdx = iD;
							iAlive = iD; iDead = iA;

							break;
						}
					}

					finishRefresh:

					// [iAlive + 1, intSizeNext) contains only dead atoms, clean them up
					for(int iD{iAlive + 1}; iD < intSizeNext; ++iD)
					{
						atoms[iD].deinitData();
						++(getMarkFromAtom(atoms[iD]).ctr);
					}

					size = sizeNext = iAlive + 1; // Update size
				}

				template<typename TFunc> inline void forEach(TFunc mFunc)
				{
					for(auto i(0u); i < size; ++i) mFunc(atoms[i].getData());
				}
				template<typename TFunc> inline void forEachAtom(TFunc mFunc)
				{
					for(auto i(0u); i < size; ++i) mFunc(atoms[i]);
				}

				inline AtomType& getAtomAt(Idx mIdx) noexcept 				{ SSVU_ASSERT(mIdx < atoms.size()); return atoms[mIdx]; }
				inline const AtomType& getAtomAt(Idx mIdx) const noexcept 	{ SSVU_ASSERT(mIdx < atoms.size()); return atoms[mIdx]; }
				inline T& getDataAt(Idx mIdx) noexcept 						{ return getAtomAt(mIdx).getData(); }
				inline const T& getDataAt(Idx mIdx) const noexcept 			{ return getAtomAt(mIdx).getData(); }

				inline std::size_t getSize() const noexcept 	{ return size; }
				inline std::size_t getSizeNext() const noexcept { return sizeNext; }
		};

		template<typename T> inline bool Handle<T>::isAlive() const noexcept
		{
			return manager->marks[markIdx].ctr == ctr;
		}

		template<typename T> inline void Handle<T>::destroy() noexcept
		{
			return manager->destroy(markIdx);
		}
	}

	class OBParticleSystem : public sf::Drawable
	{
		private:
			ssvs::VertexVector<sf::PrimitiveType::Quads> vertices;
			t::HManager<OBParticle> particles;
			std::size_t currentCount{0};

		public:
			inline OBParticleSystem() { vertices.resize(OBConfig::getParticleMax() * 4); particles.reserve(OBConfig::getParticleMax()); }
			template<typename... TArgs> inline void emplace(TArgs&&... mArgs) { if(particles.getSizeNext() <= OBConfig::getParticleMax())  particles.createAtom(std::forward<TArgs>(mArgs)...); }
			inline void update(FT mFT)
			{
				// Remove excess particles
				//if(particles.size() > OBConfig::getParticleMax()) particles.erase(std::begin(particles) + OBConfig::getParticleMax(), std::end(particles));

				//ssvu::eraseRemoveIf(particles, [](const OBParticle& mParticle){ return mParticle.life <= 0; });
				particles.refresh();
				particles.forEachAtom([](decltype(particles)::AtomType& mA){ if(mA.getData().life <= 0) mA.setDead(); });

				currentCount = particles.getSize();

				for(auto i(0u); i < currentCount; ++i)
				{
					auto& p(particles.getDataAt(i)); p.update(mFT);
					const auto& vIdx(i * 4);

					auto& vNW(vertices[vIdx + 0]);
					auto& vNE(vertices[vIdx + 1]);
					auto& vSE(vertices[vIdx + 2]);
					auto& vSW(vertices[vIdx + 3]);

					vNW.position = p.nw;
					vNE.position = p.ne;
					vSE.position = p.se;
					vSW.position = p.sw;

					vNW.color = vNE.color = vSE.color = vSW.color = p.color;
				}
			}
			inline void draw(sf::RenderTarget& mRenderTarget, sf::RenderStates mRenderStates) const override { mRenderTarget.draw(&vertices[0], currentCount * 4, sf::PrimitiveType::Quads, mRenderStates); }
			inline void clear() noexcept { particles.clear(); currentCount = 0; }
	};
}

#endif
