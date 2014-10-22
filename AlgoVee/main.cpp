#include <SSVStart/SSVStart.hpp>
#include <SSVUtils/Easing/Easing.hpp>

using SizeT = ssvu::SizeT;
template<typename T> using Vec2 = ssvs::Vec2<T>;
template<typename T, typename TD = ssvu::DefDel<T>> using UPtr = ssvs::UPtr<T, TD>;
using Vec2i = ssvs::Vec2i;
using Vec2f = ssvs::Vec2f;
using Vec2u = ssvs::Vec2u;
using FT = ssvu::FT;
using Trigger = ssvs::Input::Trigger;

namespace Boilerplate
{
	class App
	{
		protected:
			using IK = ssvs::KKey;
			using IB = ssvs::MBtn;
			using IT = ssvs::Input::Type;
			using IM = ssvs::Input::Mode;

			ssvs::GameState gameState;
			ssvs::Camera gameCamera;
			ssvs::GameWindow* gameWindow;

		public:
			inline App(ssvs::GameWindow& mGameWindow) : gameCamera{mGameWindow, 1.f}, gameWindow{&mGameWindow} { }

			inline auto& getGameState() noexcept				{ return gameState; }
			inline const auto& getGameState() const noexcept	{ return gameState; }
			inline auto& getGameCamera() noexcept				{ return gameCamera; }
			inline const auto& getGameCamera() const noexcept	{ return gameCamera; }
			inline auto& getGameWindow() noexcept				{ SSVU_ASSERT(gameWindow != nullptr); return *gameWindow; }
			inline const auto& getGameWindow() const noexcept	{ SSVU_ASSERT(gameWindow != nullptr); return *gameWindow; }
	};

	template<typename T> class AppRunner
	{
		private:
			ssvs::GameWindow gameWindow;
			ssvu::AlignedStorageFor<T> app;

		public:
			inline AppRunner(const std::string& mTitle, SizeT mWidth, SizeT mHeight)
			{
				gameWindow.setTitle(mTitle);
				gameWindow.setTimer<ssvs::TimerStatic>(0.5f, 0.5f);
				gameWindow.setSize(mWidth, mHeight);
				gameWindow.setFullscreen(false);
				gameWindow.setFPSLimited(true);
				gameWindow.setMaxFPS(200);
				gameWindow.setPixelMult(1);

				new(&app) T{gameWindow};

				gameWindow.setGameState(reinterpret_cast<T&>(app).getGameState());
				gameWindow.run();
			}
			inline ~AppRunner() { reinterpret_cast<T&>(app).~T(); }
	};
}

namespace avz
{
	struct Assets
	{
		inline static auto& getAM()
		{
			static bool loaded{false};
			static ssvs::AssetManager assetManager;

			if(!loaded)
			{
				assetManager.load<sf::Font>("testFont", "/usr/share/fonts/TTF/LiberationSans-Regular.ttf");
				loaded = true;
			}

			return assetManager;
		}
	};

	class Ctx;

	namespace Internal
	{
		class TACtx;
		class Widget;

		class TimedAction
		{
			friend class avz::Ctx;
			friend class avz::Internal::TACtx;

			private:
				FT current{0.f}, target{0.f};
				ssvu::Delegate<void(TimedAction&, FT)> action;
				bool simultaneous{false};

			public:
				inline TimedAction() = default;
				inline TimedAction(FT mTarget) : target{mTarget} { }

				inline TimedAction(const TimedAction&) = default;
				inline TimedAction(TimedAction&&) = default;

				inline void update(FT mFT) 
				{ 
					current = ssvu::getClampedMax(current + mFT, target);
					action(*this, mFT); 
				}
				inline bool isDone() const noexcept { return current >= target; }

				template<typename T> inline auto& operator+=(T&& mX)
				{
					action += ssvu::fwd<T>(mX);
					return *this;
				}

				inline float ease(float mA, float mB)
				{
					return ssvu::getMapEased<ssvu::Easing::Quad, ssvu::Easing::InOut>(current, 0.f, target, mA, mB);
				}
		};
	}

	using TARecycler = ssvu::MonoRecycler<Internal::TimedAction>;
	using TAPtr = TARecycler::PtrType;
	using WRecycler = ssvu::PolyRecycler<Internal::Widget>;
	using WPtr = WRecycler::PtrType;

	namespace Internal
	{
		class TACtx
		{
			friend class avz::Ctx;

			private:
				std::vector<TAPtr> taQueue, taInExec;

				void simultaneously()
				{
					taQueue.back()->simultaneous = true;
				}

				void execFirstTA()
				{
					auto ptr(taQueue.front().get());
					taInExec.emplace_back(std::move(taQueue.front()));
					taQueue.erase(std::begin(taQueue));

					if(ptr->simultaneous) execFirstTA();
				}

			public:
				inline void update(FT mFT, float mSpeedFactor)
				{
					if(!taQueue.empty() && taInExec.empty())
					{
						execFirstTA();
					}

					for(auto& ta : taInExec)
					{
						ta->update(mFT * mSpeedFactor);
					}

					ssvu::eraseRemoveIf(taInExec, [](const auto& mTA){ return mTA->isDone(); });
				}

				inline void skipAnim()
				{
					for(auto& ta : taInExec)
					{
						ta->current = ta->target - 0.001f;
					}
				}
		};
	}


	namespace Internal
	{
		struct Transform
		{
			Vec2f pos;
			Vec2f scale;
			float rot;
		};

		class Widget
		{
			friend class avz::Ctx;

			private:
				Ctx* ctx{nullptr};
				std::vector<WPtr> children;

				inline void transformHierarchyImpl(Widget* mParent)
				{
					tFinal.rot = tLocal.rot + mParent->tFinal.rot;
					tFinal.scale = tLocal.scale + mParent->tFinal.scale;

					tFinal.pos = tLocal.pos + mParent->tFinal.pos;
					ssvs::rotateRadAround(tFinal.pos, mParent->tFinal.pos, tFinal.rot);
				
					for(auto& c : children) c->transformHierarchyImpl(this);
				}

			public:
				Transform tLocal, tFinal;


			public:
				template<typename T> void render(const T& mX);
				template<typename... TArgs> TimedAction& createTA(TArgs&&...);
				template<typename T, typename... TArgs> T& create(TArgs&&...);
				void simultaneously();

				inline Widget(Ctx& mCtx) : ctx{&mCtx} { }

				inline virtual void update(FT) { }
				inline virtual void draw() { }

				inline void transformHierarchy()
				{		
					for(auto& c : children) c->transformHierarchyImpl(this);
				}

				inline void updateHierarchy(FT mFT)
				{
					update(mFT);
					for(auto& c : children) c->updateHierarchy(mFT);
				}

				inline void drawHierarchy()
				{
					draw();
					for(auto& c : children) c->drawHierarchy();
				}
		};
	}



	namespace w
	{
		class Base : public Internal::Widget
		{
			public:
				inline void taAppear()
				{
					createTA(25.f) += [this](auto& mTA, FT)
					{
						auto s(mTA.ease(0.f, 1.f));
						tLocal.scale = Vec2f{s, s};
					};
				}

				inline void taJump()
				{
					createTA(6.f) += [this](auto& mTA, FT)
					{
						auto s(mTA.ease(1.f, 1.3f));
						tLocal.scale = Vec2f{s, s};
					};

					createTA(6.f) += [this](auto& mTA, FT)
					{
						auto s(mTA.ease(1.3f, 1.f));
						tLocal.scale = Vec2f{s, s};
					};
				}

				inline void taTranslate(const Vec2f& mPos)
				{
					createTA(35.f) += [this, mPos](auto& mTA, FT)
					{
						tLocal.pos.x = mTA.ease(tLocal.pos.x, mPos.x);
						tLocal.pos.y = mTA.ease(tLocal.pos.y, mPos.y);
					};
				}

				inline void taColorize(sf::Color& mColor, const sf::Color& mCStart, const sf::Color& mCEnd)
				{
					createTA(25.f) += [this, &mColor, mCStart, mCEnd](auto& mTA, FT)
					{
						mColor.r = mTA.ease(mCStart.r, mCEnd.r);
						mColor.g = mTA.ease(mCStart.g, mCEnd.g);
						mColor.b = mTA.ease(mCStart.b, mCEnd.b);
						mColor.a = mTA.ease(mCStart.a, mCEnd.a);
					};
				}

				template<typename T> inline void apply(T& mX)
				{
					mX.setOrigin(ssvs::getLocalCenter(mX));
					mX.setPosition(tFinal.pos);
					mX.setScale(tFinal.scale);
					mX.setRotation(ssvu::toDeg(tFinal.rot));
				}

			protected:
				inline Base(Ctx& mCtx, const Vec2f& mPos) : Internal::Widget{mCtx}
				{
					tLocal.pos = mPos;
					tLocal.scale = ssvs::zeroVec2f;
					tLocal.rot = 0.f;
				}
		};

		class Text : public Base
		{
			private:
				sf::Text impl;

				inline void setStr(const std::string& mStr)
				{
					if(impl.getString() == "")
					{
						impl.setString(mStr);
						taAppear();
						taJump();
						createTA(10.f) += [this, mStr](auto&, FT){ };

						return;
					}

					createTA() += [this, mStr](auto&, FT) { impl.setString(mStr); };
					taJump();
				}

			public:
				inline Text(Ctx& mCtx, const Vec2f& mPos)
					: Base{mCtx, mPos}, impl{"", Assets::getAM().get<sf::Font>("testFont"), 30}
				{
					impl.setScale(0.f, 0.f);
					impl.setColor(sf::Color::White);
				}

				inline auto& operator=(const std::string& mStr) { setStr(mStr); return *this; }
				inline auto& operator+=(const std::string& mStr) { setStr(impl.getString() + mStr); return *this; }

				inline void update(FT) override { apply(impl); }
				inline void draw() override { render(impl); }
		};

		class TextSquare : public Base
		{
			private:
				sf::RectangleShape bg;
				sf::Color bgColorDef{95, 95, 95, 255};
				sf::Color bgColorHgl{175, 95, 175, 255};
				sf::Color bgColor{bgColorDef};
				Text* text;

			public:
				inline TextSquare(Ctx& mCtx, const Vec2f& mPos)
					: Base{mCtx, mPos}
				{
					text = &create<Text>(ssvs::zeroVec2f);
					bg.setSize(Vec2f{65, 65});
					bg.setOutlineColor(sf::Color::White);
					bg.setOutlineThickness(3);
				}

				inline void taShow()
				{
					taAppear();
					taJump();
				}

				inline void update(FT) override
				{
					apply(bg);
					bg.setFillColor(bgColor);
					//text->pos = pos;
				}
				inline void draw() override { render(bg); }

				inline auto& operator=(const std::string& mStr)		{ *text = mStr; return *this; }
				inline auto& operator+=(const std::string& mStr)	{ *text += mStr; return *this; }

				inline void taHightlight()		{ taColorize(bgColor, bgColorDef, bgColorHgl); }
				inline void taUnhightlight()	{ taColorize(bgColor, bgColorHgl, bgColorDef); }
		};

		class Vector : public Base
		{
			private:
				std::vector<TextSquare*> tss;

				inline void refreshPositions()
				{
					if(tss.empty()) return;

					tss[0]->tLocal.pos = ssvs::zeroVec2f;
					for(auto i(1u); i < tss.size(); ++i)
					{
						tss[i]->tLocal.pos = tss[i - 1]->tLocal.pos + Vec2f{65.f, 0.f};
					}
				}

			public:
				inline Vector(Ctx& mCtx, const Vec2f& mPos) : Base{mCtx, mPos} { }

				inline void taPushFront(const std::string& mStr)
				{
					auto& ts(create<TextSquare>(ssvs::zeroVec2f));
					
					this->createTA() += [this, &ts](auto&, FT){ tss.emplace(std::begin(tss), &ts); };
					this->createTA() += [this](auto&, FT){ this->refreshPositions(); };
					ts.taShow();
					ts = mStr;					
				}
				inline void taPushBack(const std::string& mStr)
				{
					auto& ts(create<TextSquare>(ssvs::zeroVec2f));
					
					this->createTA() += [this, &ts](auto&, FT){ tss.emplace_back(&ts); };
					this->createTA() += [this](auto&, FT){ this->refreshPositions(); };
					ts.taShow();
					ts = mStr;					
				}

				inline void update(FT) override
				{
					//tLocal.rot += 0.01f * mFT;
				}

				inline void taSwap(SizeT mA, SizeT mB)
				{
					createTA() += [this, mA, mB](auto&, FT)
					{
						auto& tsA(*tss[mA]);
						auto& tsB(*tss[mB]);

						auto pTSA(tsA.tLocal.pos);
						auto pTSB(tsB.tLocal.pos);

						tsA.taHightlight();
						tsB.taHightlight();

						tsA.taTranslate(tsA.tLocal.pos + Vec2f{0, -100});
						tsB.taTranslate(tsB.tLocal.pos + Vec2f{0, -100});

						tsA.taTranslate(Vec2f{pTSB.x, tsA.tLocal.pos.y - 100});
						this->simultaneously();
						tsB.taTranslate(Vec2f{pTSA.x, tsB.tLocal.pos.y - 100});

						tsA.taTranslate(pTSB);
						tsB.taTranslate(pTSA);

						tsA.taUnhightlight();
						simultaneously();
						tsB.taUnhightlight();

						std::swap(tss[mA], tss[mB]);
					};				
				}
		};
	}

	class Ctx
	{
		friend class Internal::Widget;

		private:
			Internal::Widget root;

			WRecycler wRecycler;
			TARecycler taRecycler;

			Internal::TACtx rootTACtx;
			Internal::TACtx* currentTACtx;

			ssvs::GameWindow* gameWindow;
			float speedFactor{1.f};

			template<typename T> inline void render(T& mX)
			{
				gameWindow->draw(mX);
			}
			template<typename... TArgs> inline auto& createTA(TArgs&&... mArgs)
			{
				return taRecycler.getCreateEmplace(currentTACtx->taQueue, ssvu::fwd<TArgs>(mArgs)...);
			}

			inline void simultaneously() { currentTACtx->simultaneously(); }

		public:
			inline Ctx(ssvs::GameWindow& mGameWindow) : root{*this}, currentTACtx{&rootTACtx}, gameWindow{&mGameWindow} { }

			inline void setSpeedFactor(float mX) noexcept { speedFactor = mX; }

			inline void skipAnim()
			{
				currentTACtx->skipAnim();
			}

			inline void update(FT mFT)
			{
				currentTACtx->update(mFT, speedFactor);

				root.transformHierarchy();
				root.updateHierarchy(mFT);
			}
			inline void draw() { root.drawHierarchy(); }

			template<typename T, typename... TArgs> inline auto& create(TArgs&&... mArgs)
			{
				return root.create<T>(ssvu::fwd<TArgs>(mArgs)...);
			}
	};

	template<typename T> inline void Internal::Widget::render(const T& mX)
	{
		ctx->render(mX);
	}
	template<typename... TArgs> inline Internal::TimedAction& Internal::Widget::createTA(TArgs&&... mArgs)
	{
		return ctx->createTA(ssvu::fwd<TArgs>(mArgs)...);
	}
	template<typename T, typename... TArgs> inline T& Internal::Widget::create(TArgs&&... mArgs)
	{
		return ctx->wRecycler.getCreateEmplace<T>(children, *ctx, ssvu::fwd<TArgs>(mArgs)...);
	}
	inline void Internal::Widget::simultaneously()
	{
		ctx->simultaneously();
	}
}

class AlgoVizTestApp : public Boilerplate::App
{
	private:
		avz::Ctx ctx;

		inline void initInput()
		{
			auto& gs(gameState);

			gs.addInput({{IK::Escape}}, [this](FT){ gameWindow->stop(); });
			gs.addInput({{IK::Space}}, [this](FT){ ctx.skipAnim(); }, IT::Once);
			gs.addInput({{IK::X}}, [this](FT){ ctx.setSpeedFactor(2.5f); }, [this](FT){ ctx.setSpeedFactor(1.f); });

			gs.addInput({{IK::A}}, [this](FT){ gameCamera.pan(-4, 0); });
			gs.addInput({{IK::D}}, [this](FT){ gameCamera.pan(4, 0); });
			gs.addInput({{IK::W}}, [this](FT){ gameCamera.pan(0, -4); });
			gs.addInput({{IK::S}}, [this](FT){ gameCamera.pan(0, 4); });
			gs.addInput({{IK::Q}}, [this](FT){ gameCamera.zoomOut(1.1f); });
			gs.addInput({{IK::E}}, [this](FT){ gameCamera.zoomIn(1.1f); });
		}

		inline void initTest()
		{
			/*
			auto& t(ctx.create<avz::w::Text>(Vec2f{100.f, 100.f}));
			t.setStr("hello");
			t.setStr("world!");
			t.setStr("im");
			t.setStr("a");
			t.setStr("fucking");
			t.setStr("text");

			auto& ts(ctx.create<avz::w::TextSquare>(Vec2f{250.f, 100.f}));
			ts.getText().setStr("0");
			ts.getText().setStr("1");
			ts.getText().setStr("2");
			ts.getText().setStr("3");
			ts.getText().setStr("4");
			*/

			auto& v(ctx.create<avz::w::Vector>(Vec2f{100.f, 100.f}));
			v.taPushBack("C");
			v.taPushBack("D");
			v.taPushBack("E");
			v.taPushFront("A");
			v.taPushFront("B");

			v.taSwap(0, 3);
			v.taSwap(1, 2);
		}

		inline void update(FT mFT)
		{
			gameCamera.update<float>(mFT);
			ctx.update(mFT);
		}
		inline void draw()
		{
			gameCamera.apply();
			ctx.draw();
			gameCamera.unapply();
		}

	public:
		inline AlgoVizTestApp(ssvs::GameWindow& mGameWindow) : Boilerplate::App{mGameWindow}, ctx{mGameWindow}
		{
			gameState.onUpdate += [this](FT mFT){ update(mFT); };
			gameState.onDraw += [this]{ draw(); };

			initInput();
			initTest();
		}
};

int main()
{
	Boilerplate::AppRunner<AlgoVizTestApp>{"AlgoVee tests", 1440, 900};
	return 0;
}
