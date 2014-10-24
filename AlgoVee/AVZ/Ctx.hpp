#ifndef ALGOVEE_AVZ_CTX
#define ALGOVEE_AVZ_CTX

namespace avz
{
	class Ctx
	{
		friend class Internal::Widget;

		private:
			Internal::Widget root;

			WRecycler wRecycler;
			TARecycler taRecycler;

			ssvu::VecUPtr<Internal::TACtx> taCtxStack;
			int toPop{0};

			ssvs::GameWindow* gameWindow;
			float speedFactor{1.f};

			inline auto& getCurrentTACtx() noexcept { return *taCtxStack.back(); }

			template<typename T> inline void render(T& mX)
			{
				gameWindow->draw(mX);
			}
			template<typename... TArgs> inline auto& createTA(TArgs&&... mArgs)
			{
				return taRecycler.getCreateEmplace(getCurrentTACtx().taQueue, ssvu::fwd<TArgs>(mArgs)...);
			}

			inline void simultaneously() { getCurrentTACtx().simultaneously(); }


			inline void makeTACtx()		{ taCtxStack.emplace_back(ssvu::makeUPtr<Internal::TACtx>()); }
			inline void restoreTACtx()	{ ++toPop; }

		public:
			inline Ctx(ssvs::GameWindow& mGameWindow) : root{*this}, gameWindow{&mGameWindow} 
			{ 
				//taCtxStack.reserve(10);
				makeTACtx();
			}

			inline void setSpeedFactor(float mX) noexcept { speedFactor = mX; }

			inline void skipAnim()
			{
				getCurrentTACtx().skipAnim();
			}

			inline void update(FT mFT)
			{				
				for(; toPop > 0; --toPop) { taCtxStack.pop_back(); ssvu::lo("BACK TO CONTEXT") << taCtxStack.size() - 1 << "\n"; }
				getCurrentTACtx().update(mFT, speedFactor);

				root.transformHierarchy();
				root.updateHierarchy(mFT);
			}
			inline void draw() { root.drawHierarchy(); }

			template<typename T, typename... TArgs> inline auto& create(TArgs&&... mArgs)
			{
				return root.create<T>(ssvu::fwd<TArgs>(mArgs)...);
			}
	};
}

#endif