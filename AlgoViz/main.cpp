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

#define EASE ssvu::getMapEased<ssvu::Easing::Sine, ssvu::Easing::InOut>

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
				assetManager.load<sf::Font>("testFont", "/usr/share/fonts/TTF/Ubuntu-Regular.ttf");
				loaded = true;
			}

			return assetManager;
		}
	};

	class Ctx;

	namespace Internal
	{
		struct TimedAction
		{		
			FT current{0}, target;
			ssvu::Delegate<void(TimedAction&, FT)> action;
			bool simultaneous{false};

			inline TimedAction(FT mTarget) : target{mTarget} { }

			inline void update(FT mFT) { current += mFT; action(*this, mFT); }
			inline bool isDone() const noexcept { return current >= target; }
		};
	}

	using TARecycler = ssvu::MonoRecycler<Internal::TimedAction>;
	using TAPtr = TARecycler::PtrType;

	namespace Internal
	{
		class Widget
		{
			friend class Ctx;

			private:
				Ctx* ctx{nullptr};
				std::vector<ssvu::PolyRecycler<Internal::Widget>::PtrType> children;

			public:			
				template<typename T> void render(const T& mX);
				template<typename... TArgs> TimedAction& createTA(TArgs&&...);
				template<typename T, typename... TArgs> T& create(TArgs&&... mArgs);
				void simultaneously();

				inline Widget(Ctx& mCtx) : ctx{&mCtx} { } 

				inline virtual void update(FT) { }
				inline virtual void draw() { }

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
				Vec2f pos; 
				Vec2f scale{0.f, 0.f};

				inline void appear()
				{
					createTA(25.f).action += [this](auto& mTA, FT mFT)
					{
						auto s(EASE(mTA.current, 0.f, mTA.target, 0.f, 1.f));
						scale = Vec2f{s, s};
					};
				}

				inline void jump()
				{
					createTA(6.f).action += [this](auto& mTA, FT mFT)
					{
						auto s(EASE(mTA.current, 0.f, mTA.target, 1.f, 1.3f));
						scale = Vec2f{s, s};
					};

					createTA(6.f).action += [this](auto& mTA, FT mFT)
					{
						auto s(EASE(mTA.current, 0.f, mTA.target, 1.3f, 1.f));
						scale = Vec2f{s, s};
					};
				}

				inline void translate(const Vec2f& mPos)
				{
					createTA(25.f).action += [this, mPos](auto& mTA, FT mFT)
					{
						auto nx(EASE(mTA.current, 0.f, mTA.target, pos.x, mPos.x));
						auto ny(EASE(mTA.current, 0.f, mTA.target, pos.y, mPos.y));
						pos = Vec2f{nx, ny};
					};
				}	

				template<typename T> inline void apply(T& mX)
				{
					mX.setOrigin(ssvs::getLocalCenter(mX));
					mX.setPosition(pos);
					mX.setScale(scale);
				}

			public:
				inline Base(Ctx& mCtx, const Vec2f& mPos) : Internal::Widget{mCtx}, pos{mPos}
				{

				}
		};

		class Text : public Base
		{
			private:
				sf::Text impl;

			public:
				inline Text(Ctx& mCtx, const Vec2f& mPos) 
					: Base{mCtx, mPos}, impl{"", Assets::getAM().get<sf::Font>("testFont"), 30}
				{
					impl.setScale(0.f, 0.f);
					impl.setColor(sf::Color::White);												
				}

				inline void setStr(const std::string& mStr)
				{
					if(impl.getString() == "") 
					{
						impl.setString(mStr);
						appear();
						jump();
						createTA(15.f).action += [this, mStr](auto& mTA, FT mFT){ };

						return;
					}

					createTA(0.f).action += [this, mStr](auto& mTA, FT mFT)
					{
						impl.setString(mStr);
					};

					jump();
				}

				inline void update(FT) override { apply(impl); }
				inline void draw() override { render(impl); }
		};

		class TextSquare : public Base
		{
			private:
				sf::RectangleShape bg;
				Text* text;

			public:
				inline TextSquare(Ctx& mCtx, const Vec2f& mPos) 
					: Base{mCtx, mPos}
				{
					text = &create<Text>(mPos);
					bg.setSize(Vec2f{65, 65});
					bg.setOutlineColor(sf::Color::White);
					bg.setOutlineThickness(3);
					bg.setFillColor(sf::Color{95, 95, 95, 255});

					appear();
					jump();
				}

				inline void update(FT) override 
				{ 
					apply(bg); 
					text->pos = pos; 
				}
				inline void draw() override { render(bg); }

				inline auto& getText() noexcept { return *text; }
		};

		class Vector : public Base
		{	
			private:
				std::vector<TextSquare*> tss;

			public:
				inline Vector(Ctx& mCtx, const Vec2f& mPos) 
					: Base{mCtx, mPos}
				{

				}

				inline void add(const std::string& mStr)
				{
					auto& ts(create<TextSquare>(pos));
					tss.emplace_back(&ts);
					ts.getText().setStr(mStr);

					ts.pos = pos + Vec2f{(tss.size() - 1) * 65.f, 0.f};
				}

				inline void update(FT) override
				{

				}

				inline void swap(SizeT mA, SizeT mB)
				{
					auto& tsA(*tss[mA]);
					auto& tsB(*tss[mB]);

					auto pTSA(tsA.pos);
					auto pTSB(tsB.pos);

					tsA.translate(tsA.pos + Vec2f{0, -100});
					tsB.translate(tsB.pos + Vec2f{0, -100});

					tsA.translate(Vec2f{pTSB.x, tsA.pos.y - 100});
					simultaneously();
					tsB.translate(Vec2f{pTSA.x, tsB.pos.y - 100});

					tsA.translate(pTSB);
					tsB.translate(pTSA);

					std::swap(tss[mA], tss[mB]);
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

class AlgoVizTestApp
{
	private:
		ssvs::GameState gameState;
		ssvs::Camera gameCamera; 
		ssvs::GameWindow* gameWindow;
		avz::Ctx ctx;

		inline void initInput()
		{
			using k = ssvs::KKey;
			using b = ssvs::MBtn;
			using t = ssvs::Input::Type;
			using m = ssvs::Input::Mode;

			auto& gs(gameState);

			gs.addInput({{k::Escape}}, [this](FT){ gameWindow->stop(); });
			gs.addInput({{k::Space}}, [this](FT){ ctx.skipAnim(); }, t::Once);
			gs.addInput({{k::X}}, [this](FT){ ctx.setSpeedFactor(2.5f); }, [this](FT){ ctx.setSpeedFactor(1.f); });

			gs.addInput({{k::A}}, [this](FT){ gameCamera.pan(-4, 0); });
			gs.addInput({{k::D}}, [this](FT){ gameCamera.pan(4, 0); });
			gs.addInput({{k::W}}, [this](FT){ gameCamera.pan(0, -4); });
			gs.addInput({{k::S}}, [this](FT){ gameCamera.pan(0, 4); });
			gs.addInput({{k::Q}}, [this](FT){ gameCamera.zoomOut(1.1f); });
			gs.addInput({{k::E}}, [this](FT){ gameCamera.zoomIn(1.1f); });
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
			v.add("A");
			v.add("B");
			v.add("C");
			v.add("D");
			v.add("E");

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
		inline AlgoVizTestApp(ssvs::GameWindow& mGameWindow) 
			: gameWindow{&mGameWindow}, gameCamera{mGameWindow, 1.f}, ctx{*gameWindow}
		{
			gameState.onUpdate += [this](FT mFT){ update(mFT); };
			gameState.onDraw += [this]{ draw(); };

			initInput();
			initTest();
		}

		inline auto& getGameState() noexcept { return gameState; }
};

int main()
{
	auto width(1440);
	auto height(900);

	ssvs::GameWindow gameWindow;
	gameWindow.setTitle("AlgoViz tests");
	gameWindow.setTimer<ssvs::TimerStatic>(0.5f, 0.5f);
	gameWindow.setSize(width, height);
	gameWindow.setFullscreen(false);
	gameWindow.setFPSLimited(true);
	gameWindow.setMaxFPS(200);
	gameWindow.setPixelMult(1);

	AlgoVizTestApp testApp{gameWindow};

	gameWindow.setGameState(testApp.getGameState());
	gameWindow.run();

	return 0;
}