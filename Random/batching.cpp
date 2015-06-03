#include <SSVStart/SSVStart.hpp>

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
			ssvs::GameWindow& gameWindow;

		public:
			inline App(ssvs::GameWindow& mGW) : gameCamera{mGW, 1.f}, gameWindow{mGW} { }

			inline void stop() noexcept	{ return gameWindow.stop(); }

			template<typename... TArgs> inline void render(TArgs&&... mArgs)
			{
				gameWindow.draw(FWD(mArgs)...);
			}

			inline auto& getGameState() noexcept				{ return gameState; }
			inline const auto& getGameState() const noexcept	{ return gameState; }
			inline auto& getGameCamera() noexcept				{ return gameCamera; }
			inline const auto& getGameCamera() const noexcept	{ return gameCamera; }
			inline auto& getGameWindow() noexcept				{ return gameWindow; }
			inline const auto& getGameWindow() const noexcept	{ return gameWindow; }
	};

	template<typename T> class AppRunner
	{
		private:
			ssvs::GameWindow gameWindow;
			ssvu::Maybe<T> app;

		public:
			inline AppRunner(const std::string& mTitle, ssvu::SizeT mWidth, ssvu::SizeT mHeight)
			{
				gameWindow.setTitle(mTitle);
				gameWindow.setTimer<ssvs::TimerStatic>(0.5f, 0.5f);
				gameWindow.setSize(mWidth, mHeight);
				gameWindow.setFullscreen(false);
				gameWindow.setFPSLimited(true);
				gameWindow.setMaxFPS(200);
				gameWindow.setPixelMult(1);

				app.init(gameWindow);

				gameWindow.setGameState(app->getGameState());				
			}

			inline void run() { gameWindow.run(); }
			inline auto& getApp() noexcept { return app.get(); }

			inline ~AppRunner() { app.deinit(); }
	};

	struct TestApp : public App
	{
		private:
			ssvs::BitmapText txtInfo{ssvs::getDefaultAsset<ssvs::BitmapFont>()};

		public:
			ssvu::Delegate<void(ssvs::GameState&)> onInitInput;
			ssvu::Delegate<void(ssvu::FT)> onUpdate;
			ssvu::Delegate<void()> onDraw;

		private:
			inline void initInput()
			{			
				auto& gState(gameState);
				
				gState.addInput({{IK::Escape}}, [this](ssvu::FT){ stop(); });

				gState.addInput({{IK::A}}, [this](ssvu::FT){ gameCamera.pan(-4, 0); });
				gState.addInput({{IK::D}}, [this](ssvu::FT){ gameCamera.pan(4, 0); });
				gState.addInput({{IK::W}}, [this](ssvu::FT){ gameCamera.pan(0, -4); });
				gState.addInput({{IK::S}}, [this](ssvu::FT){ gameCamera.pan(0, 4); });
				gState.addInput({{IK::Q}}, [this](ssvu::FT){ gameCamera.zoomOut(1.1f); });
				gState.addInput({{IK::E}}, [this](ssvu::FT){ gameCamera.zoomIn(1.1f); });

				onInitInput(gState);		
			}

			inline void update(ssvu::FT mFT)
			{
				onUpdate(mFT);
				gameCamera.update<float>(mFT);				

				txtInfo.setString
				(
					std::string{"FPS: "} + ssvu::toStr(ssvu::toInt(getGameWindow().getFPS())) + "\n" 
					+ "U: " + ssvu::toStr(getGameWindow().getMsUpdate()) + "\n" 
					+ "D: " + ssvu::toStr(getGameWindow().getMsDraw()) + "\n"
				);
			}

			inline void draw()
			{
				gameCamera.apply();
				onDraw();
				gameCamera.unapply();

				render(txtInfo);
			}

		public:
			inline TestApp(ssvs::GameWindow& mGW) : App{mGW}
			{
				initInput();

				txtInfo.setPosition(ssvs::Vec2f(5.f, 5.f));
				txtInfo.setScale(ssvs::Vec2f(2.f, 2.f));
				txtInfo.setTracking(-3);
				
				gameState.onUpdate += [this](ssvu::FT mFT){ update(mFT); };
				gameState.onDraw += [this]{ draw(); };				
			}			
	};

	using TestAppRunner = AppRunner<TestApp>;
}

static constexpr ssvu::SizeT tablePrecision{628};
static constexpr float tableRatio{tablePrecision / ssvu::tau};
inline constexpr auto getTableIdx(float mX) noexcept { return ssvu::toInt(mX * tableRatio) % tablePrecision; }

struct SinTable
{
	std::array<float, tablePrecision> arr;
	inline SinTable() noexcept
	{
		for(auto i(0u); i < tablePrecision; ++i) arr[i] = std::sin(i / tableRatio);
	}
};

struct CosTable
{
	std::array<float, tablePrecision> arr;
	inline CosTable() noexcept
	{
		for(auto i(0u); i < tablePrecision; ++i) arr[i] = std::cos(i / tableRatio);
	}
};

struct SinCosTable
{
	std::array<ssvu::Tpl<float, float>, tablePrecision> arr;
	inline SinCosTable() noexcept
	{
		for(auto i(0u); i < tablePrecision; ++i) arr[i] = ssvu::mkTpl(std::sin(i / tableRatio), std::cos(i / tableRatio));
	}
};

inline const auto& getSinTable() noexcept 		{ static SinTable result; return result.arr; }
inline const auto& getCosTable() noexcept 		{ static CosTable result; return result.arr; }
inline const auto& getSinCosTable() noexcept	{ static SinCosTable result; return result.arr; }

inline auto getSin(float mX) noexcept 		{ return getSinTable()[getTableIdx(mX)]; }
inline auto getCos(float mX) noexcept 		{ return getCosTable()[getTableIdx(mX)]; }
inline auto getSinCos(float mX) noexcept	{ return getSinCosTable()[getTableIdx(mX)]; }

namespace Batch
{	
	namespace Impl
	{
		using VVQuads = ssvs::VertexVector<sf::PrimitiveType::Quads>;

		template<typename T1, typename T2, typename T3, typename T4, typename T5> 
		inline auto getRotateVecAroundVec(const ssvs::Vec2<T1>& mCenter, const ssvs::Vec2<T2>& mPoint, const T3& mSin, const T4& mCos, const ssvs::Vec2<T5>& mScale) noexcept
		{	
			return ssvs::Vec2<ssvs::CT<T1, T2, T3, T4, T5>>
			{
				mCenter.x + ((mPoint.x - mCenter.x) * mCos - (mPoint.y - mCenter.y) * mSin) * mScale.x,
				mCenter.y + ((mPoint.x - mCenter.x) * mSin + (mPoint.y - mCenter.y) * mCos) * mScale.y
			};
		}

		struct TextureID 
		{ 
			const sf::Vector2f halfSize, nw, ne, sw, se;
			const ssvu::SizeT id; 

			inline TextureID(ssvu::SizeT mID, float mSizeX, float mSizeY) noexcept
				: halfSize{mSizeX / 2.f, mSizeY / 2.f}, nw{0, 0}, ne{mSizeX, 0}, sw{0, mSizeY}, se{mSizeX, mSizeY}, id{mID}
			{

			}
		};
		
		struct LayerID  
		{ 
			const ssvu::SizeT id; 
		};
	}

	class Manager;

	class BatchSprite
	{
		friend class Manager;

		private:
			Impl::TextureID textureID;
			Impl::LayerID layerID;		
			sf::Vector2f position, origin, scale;
			float radians, rSin, rCos;		

		public:
			template<typename T> inline void setTexture(T&& mX) noexcept 	{ textureID = FWD(mX); }
			template<typename T> inline void setLayer(T&& mX) noexcept		{ layerID = FWD(mX); }
			template<typename T> inline void setPosition(T&& mX) noexcept 	{ position = FWD(mX); }
			template<typename T> inline void setOrigin(T&& mX) noexcept 	{ origin = FWD(mX); }
			template<typename T> inline void setScale(T&& mX) noexcept		{ scale = FWD(mX); }
			
			inline void setRadians(float mX) noexcept						
			{ 
				radians = mX; 				
				std::tie(rSin, rCos) = getSinCos(radians);
				
				// rSin = getSin(radians);
				// rCos = getCos(radians);
			}

			inline const auto& getTexture() const noexcept 					{ return textureID; }
			inline const auto& getLayer() const noexcept 					{ return layerID; }
			inline const auto& getPosition() const noexcept 				{ return position; }
			inline const auto& getOrigin() const noexcept 					{ return origin; }
			inline const auto& getScale() const noexcept 					{ return scale; }
			inline const auto& getRadians() const noexcept 					{ return radians; }			
			inline const auto& getHalfTextureSize() const noexcept 			{ return textureID.halfSize; }

			inline auto getCenter() const noexcept
			{
				return ssvs::Vec2f{position.x + origin.x, position.y + origin.y};
			}

			inline BatchSprite(const Impl::TextureID& mTextureID, const Impl::LayerID& mLayerID) noexcept 
				: textureID(mTextureID), layerID(mLayerID)
			{

			}	
		
		private:
			inline void emplaceVertices(Impl::VVQuads& mV) const
			{						
				const auto& hs(getHalfTextureSize());
				const auto& center(getCenter());

				auto l(center.x - hs.x);
				auto r(center.x + hs.x);
				auto t(center.y - hs.y);
				auto b(center.y + hs.y);

				sf::Vector2f nw{l, t};
				sf::Vector2f ne{r, t};
				sf::Vector2f se{r, b};
				sf::Vector2f sw{l, b};
				
				mV.emplace_back(Impl::getRotateVecAroundVec(center, nw, rSin, rCos, scale), textureID.nw);
				mV.emplace_back(Impl::getRotateVecAroundVec(center, ne, rSin, rCos, scale), textureID.ne);
				mV.emplace_back(Impl::getRotateVecAroundVec(center, se, rSin, rCos, scale), textureID.se);
				mV.emplace_back(Impl::getRotateVecAroundVec(center, sw, rSin, rCos, scale), textureID.sw);
			}		
	};

	namespace Impl
	{
		class Layer
		{	
			friend class Batch::Manager;

			private:
				std::vector<VVQuads> vVectors;

			public:
				inline void resize(ssvu::SizeT mX) { vVectors.resize(mX); }
				inline void clear() noexcept { for(auto& v : vVectors) v.clear(); }
				void drawOn(Manager& mManager, sf::RenderTarget& mX) noexcept;
		};
	}

	class Manager
	{
		friend class Impl::Layer;

		private:
			std::vector<const sf::Texture*> boundTextures;
			std::vector<Impl::Layer> layers;

		private:
			inline void clearLayers() noexcept { for(auto& l : layers) l.clear(); }

		public:
			inline auto bind(const sf::Texture& mTexture)
			{
				boundTextures.emplace_back(&mTexture);
				auto next(boundTextures.size());

				for(auto& l : layers) l.resize(next);

				const auto& ts(mTexture.getSize());
				return Impl::TextureID{next - 1, ssvu::toFloat(ts.x), ssvu::toFloat(ts.y)};
			}

			inline auto mkLayer()
			{
				Impl::Layer l{};
				l.resize(boundTextures.size());
				
				layers.emplace_back(l);
				return Impl::LayerID{layers.size() - 1};
			}

			inline void drawOn(sf::RenderTarget& mX) noexcept
			{
				for(auto& l : layers) l.drawOn(*this, mX); 
				clearLayers();
			}

			inline void enqueue(const BatchSprite& mX)
			{
				auto& l(layers[mX.layerID.id]);
				auto& v(l.vVectors[mX.textureID.id]);
				
				mX.emplaceVertices(v);
			}	

			inline void directDraw(sf::RenderTarget& mRT, const BatchSprite& mX)
			{
				static Impl::VVQuads x;
				x.clear();
				mX.emplaceVertices(x);

				mRT.draw(x, sf::RenderStates{boundTextures[mX.getTexture().id]});
			}
	};

	inline void Impl::Layer::drawOn(Manager& mManager, sf::RenderTarget& mX) noexcept
	{
		SSVU_ASSERT(mManager.boundTextures.size() == vVectors.size());

		for(auto i(0u); i < vVectors.size(); ++i)
		{
			auto& v(vVectors[i]);
			mX.draw(&v[0], v.size(), sf::PrimitiveType::Quads, sf::RenderStates{mManager.boundTextures[i]});
		}
	}
}

struct MovingThing
{
	Batch::BatchSprite spr;
	sf::Vector2f velocity;		
	
	inline MovingThing(const Batch::BatchSprite& mSpr) : spr{mSpr}
	{			
		const auto& hts(spr.getHalfTextureSize());	
		spr.setOrigin(ssvs::Vec2f{hts.x, hts.y});
	}

	inline void update(ssvu::FT mFT) noexcept
	{
		spr.setPosition(spr.getPosition() + velocity * mFT);
		spr.setRadians(spr.getRadians() + mFT * 0.01f);
	}

	inline void draw(Batch::Manager& mMgr)
	{		
		mMgr.enqueue(spr);
	}
};

int main()
{
	SSVUT_RUN();

	ssvs::AssetManager<> am;
	am.load<sf::Texture>("l0", "./laser0.png");
	am.load<sf::Texture>("l1", "./laser1.png");
	am.load<sf::Texture>("l2", "./laser2.png");
	am.load<sf::Texture>("l3", "./laser3.png");
	auto& atxL0(am.get<sf::Texture>("l0"));
	auto& atxL1(am.get<sf::Texture>("l1"));
	auto& atxL2(am.get<sf::Texture>("l2"));
	auto& atxL3(am.get<sf::Texture>("l3"));

	ssvu::lo("sizeof sf::Sprite") << sizeof(sf::Sprite) << "\n";
	ssvu::lo("sizeof Batch::Sprite") << sizeof(Batch::BatchSprite) << "\n";
	ssvu::lo("sizeof float") << sizeof(float) << "\n";
	ssvu::lo("sizeof Vec2f") << sizeof(sf::Vector2f) << "\n";
	ssvu::lo("sizeof SizeT") << sizeof(ssvu::SizeT) << "\n";

	Batch::Manager bm;
	auto btxL0(bm.bind(atxL0));
	auto btxL1(bm.bind(atxL1));
	auto btxL2(bm.bind(atxL2));
	auto btxL3(bm.bind(atxL3));
	auto btlForeground(bm.mkLayer());

	auto btxs(ssvu::mkVector(btxL0, btxL1, btxL2, btxL3));

	Boilerplate::TestAppRunner appR{"", 800, 600};
	auto& app(appR.getApp());

	

	std::vector<MovingThing> lasers;

	for(auto i(0u); i < 150000; ++i)
	{	
		MovingThing l{Batch::BatchSprite{btxs[ssvu::getRndI(0, btxs.size())], btlForeground}};
		l.spr.setPosition(sf::Vector2f{ssvu::getRndR(0.f, 800.f), ssvu::getRndR(0.f, 600.f)});
		auto r(ssvu::getRndR(0.f, ssvu::tau));
		l.velocity = ssvs::getVecFromRad(r, ssvu::getRndR(0.1f, 10.5f));
		l.spr.setRadians(r);
		auto s(ssvu::getRndR(0.1f, 1.1f));
		l.spr.setScale(sf::Vector2f{s, s});
		lasers.emplace_back(l);	
	}

	app.onUpdate += [&](ssvu::FT mFT)
	{
		for(auto& l : lasers) l.update(mFT);
	};

	app.onDraw += [&]()
	{
		// for(auto& l : lasers) bm.directDraw(app.getGameWindow(), l.spr);

		for(auto& l : lasers) l.draw(bm);	
		bm.drawOn(app.getGameWindow());		
	};

	appR.run();
	ssvu::lo() << "end\n";

	return 0;
}