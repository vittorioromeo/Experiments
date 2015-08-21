#include <SSVUtils/Core/Core.hpp>
#include <SSVUtils/Union/Union.hpp>

namespace Exp
{
	using namespace ssvu;	

	namespace Impl
	{				
		enum class ForAction
		{
			Continue,
			Break
		};

		template<typename T> struct TypeWrapper { using Type = T; };
		template<typename T> using TypeOf = typename T::Type;

		#define TW_TYPEOF(mX) TypeOf<decltype(mX)>

		template<typename TF> inline void forTypes(TF&&) noexcept { }

		template<typename T, typename... Ts, typename TF>
		inline void forTypes(TF&& mFn) noexcept
		{
			if(mFn(TypeWrapper<T>{}) == ForAction::Continue)
				forTypes<Ts...>(mFn);	
		}
	}	

	namespace Impl
	{
		template<typename... Ts> struct VariantStorageUnion
		{
			private:
				Union<Ts...> data;

			public:
				template<typename T, typename... TArgs> inline void init(TArgs&&... mArgs)
				{
					data.template init<T>(FWD(mArgs)...);
				}

				template<typename T> inline void deinit()
				{
					data.template deinit<T>();
				}

				template<typename T> inline auto& get() & noexcept 
				{ 
					return data.template get<T>(); 
				}

				template<typename T> inline const auto& get() const& noexcept 
				{ 
					return data.template get<T>(); 
				}
				
				template<typename T> inline auto get() && noexcept 
				{ 
					return mv(mv(data).template get<T>()); 
				}
		};	

		struct VariantStorageUPtr
		{
			private:
				
				
			public:
		};	
	
		template<template<typename...> class TStorage, typename... Ts> struct TaggedVariant
		{
			SSVU_ASSERT_STATIC(MPL::isUnique<Ts...>(), "There must be no duplicate types");	

			public:
				using Storage = TStorage<Ts...>;
				using TypeID = int;
				static constexpr TypeID nullTypeID{-1};

			private:
				Storage storage;
				TypeID typeID{nullTypeID};

				template<typename T> inline static constexpr auto getTypeID() noexcept
				{
					return MPL::getIdxOf<T, Ts...>();
				}

				template<typename T, typename... TArgs> inline void init(TArgs&&... mArgs)
				{
					storage.template init<T>(FWD(mArgs)...);
					typeID = getTypeID<T>();
				}

				template<typename T, typename... TArgs> inline void deinitImpl()
				{
					if(typeID == getTypeID<T>()) 
					{
						storage.template deinit<T>();
						typeID = nullTypeID;
						return;
					}

					deinitImpl<TArgs...>();
				}

				inline void deinit()
				{
					forTypes<Ts...>([this](auto mT)
					{
						using Type = TW_TYPEOF(mT);

						if(typeID == getTypeID<Type>()) 
						{
							storage.template deinit<Type>();
							typeID = nullTypeID;
							return ForAction::Break;
						}

						return ForAction::Continue;
					});

					// SSVU_ASSERT(typeID == nullTypeID);
				}

			public:
				inline ~TaggedVariant() 
				{
					deinit();
				}	

				template<typename T> inline auto is() const noexcept
				{
					return typeID == getTypeID<T>();
				}

				inline void setNull()
				{
					deinit();
				}	

				inline auto isNull() const noexcept 
				{ 
					return typeID == nullTypeID; 
				}

				template<typename T> inline auto& operator=(T&& mX)
				{
					init<T>(FWD(mX));
					return *this;
				} 
		};
	}

	template<typename... Ts> using TaggedVariant = Impl::TaggedVariant<Impl::VariantStorageUnion, Ts...>;
}

template<ssvu::SizeT TS> struct STest
{
	inline STest() { ssvu::lo() << "ctor: " << TS << "\n"; }
	inline ~STest() { ssvu::lo() << "dtor: " << TS << "\n"; }
};

int main()
{
	Exp::TaggedVariant<STest<0>, STest<1>, STest<2>> test;
	SSVU_ASSERT(!test.is<STest<0>>());
	SSVU_ASSERT(test.isNull());

	test = STest<0>{};
	SSVU_ASSERT(test.is<STest<0>>());
	SSVU_ASSERT(!test.isNull());

	test.setNull();
	SSVU_ASSERT(!test.is<STest<0>>());
	SSVU_ASSERT(test.isNull());

	return 0;
}
