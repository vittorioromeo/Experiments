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
			inline App(ssvs::GameWindow& mGameWindow) : gameWindow{&mGameWindow}, gameCamera{mGameWindow, 1.f} { }

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
		class TimedAction
		{
			friend class avz::Ctx;

			private:
				FT current{0.f}, target;
				ssvu::Delegate<void(TimedAction&, FT)> action;
				bool simultaneous{false};

			public:
				inline TimedAction(FT mTarget) : target{mTarget} { }

				inline void update(FT mFT) { current += mFT; action(*this, mFT); }
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
				std::vector<ssvu::PolyRecycler<Internal::Widget>::PtrType> children;

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

				inline void transformHierarchy(Widget* mParent = nullptr)
				{	
					if(mParent != nullptr)
					{
						tFinal.rot = tLocal.rot + mParent->tFinal.rot;
						tFinal.scale = tLocal.scale + mParent->tFinal.scale;
						
						tFinal.pos = tLocal.pos + mParent->tFinal.pos;
						ssvs::rotateRadAround(tFinal.pos, mParent->tFinal.pos, tFinal.rot);
					}
					for(auto& c : children) c->transformHierarchy(this);
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

	using WRecycler = ssvu::PolyRecycler<Internal::Widget>;
	using WPtr = WRecycler::PtrType;

	namespace w
	{
		class Base : public Internal::Widget
		{
			public:
				inline void appear()
				{
					createTA(25.f) += [this](auto& mTA, FT mFT)
					{
						auto s(mTA.ease(0.f, 1.f));
						tLocal.scale = Vec2f{s, s};
					};
				}

				inline void jump()
				{
					createTA(6.f) += [this](auto& mTA, FT mFT)
					{
						auto s(mTA.ease(1.f, 1.3f));
						tLocal.scale = Vec2f{s, s};
					};

					createTA(6.f) += [this](auto& mTA, FT mFT)
					{
						auto s(mTA.ease(1.3f, 1.f));
						tLocal.scale = Vec2f{s, s};
					};

					createTA(0.f) += [this](auto& mTA, FT mFT)
					{
						tLocal.scale = Vec2f{1.f, 1.f};
					};
				}

				inline void translate(const Vec2f& mPos)
				{
					createTA(35.f) += [this, mPos](auto& mTA, FT mFT)
					{
						auto nx(mTA.ease(tLocal.pos.x, mPos.x));
						auto ny(mTA.ease(tLocal.pos.y, mPos.y));
						tLocal.pos = Vec2f{nx, ny};
					};
				}

				inline void colorize(sf::Color& mColor, const sf::Color& mCStart, const sf::Color& mCEnd)
				{
					createTA(25.f) += [this, &mColor, mCStart, mCEnd](auto& mTA, FT mFT)
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
					tLocal.scale = Vec2f{0.f, 0.f};
					tLocal.rot = 0.0f;
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
						appear();
						jump();
						createTA(15.f) += [this, mStr](auto& mTA, FT mFT){ };

						return;
					}

					createTA(0.f) += [this, mStr](auto& mTA, FT mFT)
					{
						impl.setString(mStr);
					};

					jump();
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
					text = &create<Text>(Vec2f{0.f, 0.f});
					bg.setSize(Vec2f{65, 65});
					bg.setOutlineColor(sf::Color::White);
					bg.setOutlineThickness(3);

					appear();
					jump();
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

				inline void highlight()		{ colorize(bgColor, bgColorDef, bgColorHgl); }
				inline void unhighlight()	{ colorize(bgColor, bgColorHgl, bgColorDef); }
		};

		class Vector : public Base
		{
			private:
				std::vector<TextSquare*> tss;

				inline auto& makeTs(const std::string& mStr)
				{
					auto& ts(create<TextSquare>(Vec2f{0.f, 0.f}));
					ts = mStr;
					return ts;
				}

				inline void refreshPositions()
				{
					if(tss.empty()) return;

					tss[0]->tLocal.pos = Vec2f{0.f, 0.f};					
					for(auto i(1u); i < tss.size(); ++i) 
					{
						tss[i]->tLocal.pos = tss[i - 1]->tLocal.pos + Vec2f{65.f, 0.f};						
					}
				}

			public:
				inline Vector(Ctx& mCtx, const Vec2f& mPos) : Base{mCtx, mPos} { }

				inline void pushFront(const std::string& mStr)
				{
					createTA(5.f) += [this, mStr](auto& mTA, FT mFT)
					{
						auto& ts(makeTs(mStr));
						tss.emplace(std::begin(tss), &ts);
						refreshPositions();
					};
				}
				inline void pushBack(const std::string& mStr)
				{
					createTA(5.f) += [this, mStr](auto& mTA, FT mFT)
					{
						auto& ts(makeTs(mStr));
						tss.emplace_back(&ts);
						refreshPositions();
					};
				}

				inline void update(FT mFT) override
				{
					//tLocal.rot += 0.01f * mFT;
				}

				inline void swap(SizeT mA, SizeT mB)
				{
					createTA(5.f) += [this, mA, mB](auto& mTA, FT mFT)
					{
						auto& tsA(*tss[mA]);
						auto& tsB(*tss[mB]);

						auto pTSA(tsA.tLocal.pos);
						auto pTSB(tsB.tLocal.pos);

						tsA.highlight();
						tsB.highlight();

						tsA.translate(tsA.tLocal.pos + Vec2f{0, -100});
						tsB.translate(tsB.tLocal.pos + Vec2f{0, -100});

						tsA.translate(Vec2f{pTSB.x, tsA.tLocal.pos.y - 100});
						simultaneously();
						tsB.translate(Vec2f{pTSA.x, tsB.tLocal.pos.y - 100});

						tsA.translate(pTSB);
						tsB.translate(pTSA);

						tsA.unhighlight();
						simultaneously();
						tsB.unhighlight();

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
			std::vector<TAPtr> taQueue;
			std::vector<TAPtr> taInExec;
			ssvs::GameWindow* gameWindow;
			float speedFactor{1.f};

			template<typename T> inline void render(T& mX)
			{
				gameWindow->draw(mX);
			}
			template<typename... TArgs> inline auto& createTA(TArgs&&... mArgs)
			{
				return taRecycler.getCreateEmplace(taQueue, ssvu::fwd<TArgs>(mArgs)...);
			}

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
			inline Ctx(ssvs::GameWindow& mGameWindow)
				: gameWindow{&mGameWindow}, root{*this}
			{

			}

			inline void setSpeedFactor(float mX) noexcept { speedFactor = mX; }

			inline void skipAnim()
			{
				for(auto& ta : taInExec)
				{
					ta->current = ta->target - 0.005f;
				}
			}

			inline void update(FT mFT)
			{
				if(!taQueue.empty() && taInExec.empty())
				{
					execFirstTA();
				}

				for(auto& ta : taInExec)
				{
					ta->update(mFT * speedFactor);
				}

				taInExec.erase(std::remove_if(std::begin(taInExec), std::end(taInExec), [](const auto& mTA){ return mTA->isDone(); }), std::end(taInExec));

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
		auto wPtr(ctx->wRecycler.create<T>(*ctx, ssvu::fwd<TArgs>(mArgs)...));
		auto result(wPtr.get());
		children.emplace_back(std::move(wPtr));
		return ssvu::castUp<T>(*result);
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
			v.pushBack("C");
			v.pushBack("D");
			v.pushBack("E");
			v.pushFront("A");
			v.pushFront("B");

			v.swap(0, 3);
			v.swap(1, 2);
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
	// TODO: nested timed actions!

	Boilerplate::AppRunner<AlgoVizTestApp>{"AlgoVee tests", 1440, 900};
	return 0;
}