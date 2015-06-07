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

			ssvs::GameState gState;
			ssvs::Camera gCamera;
			ssvs::GameWindow& gWindow;

		public:
			inline App(ssvs::GameWindow& mGW) : gCamera{mGW, 1.f}, gWindow{mGW} { }

			inline void run() noexcept { gWindow.run(); }
			inline void stop() noexcept	{ gWindow.stop(); }

			template<typename... TArgs> inline void render(TArgs&&... mArgs)
			{
				gWindow.draw(FWD(mArgs)...);
			}

			inline auto& getGState() noexcept				{ return gState; }
			inline const auto& getGState() const noexcept	{ return gState; }
			inline auto& getGCamera() noexcept				{ return gCamera; }
			inline const auto& getGCamera() const noexcept	{ return gCamera; }
			inline auto& getGWindow() noexcept				{ return gWindow; }
			inline const auto& getGWindow() const noexcept	{ return gWindow; }
	};

	template<typename T> class AppRunner
	{
		private:
			ssvs::GameWindow gWindow;
			T app;

		public:
			inline AppRunner(const std::string& mTitle, ssvu::SizeT mWidth, ssvu::SizeT mHeight) 
				: app{gWindow}
			{
				gWindow.setTitle(mTitle);
				gWindow.setTimer<ssvs::TimerStatic>(0.5f, 0.5f);
				gWindow.setSize(mWidth, mHeight);
				gWindow.setFullscreen(false);
				gWindow.setFPSLimited(true);
				gWindow.setMaxFPS(200);
				gWindow.setPixelMult(1);

				gWindow.setGameState(app.getGState());
			}

			inline auto& getApp() noexcept { return app; }
	};

	struct TestApp : public App
	{
		private:
			ssvs::BitmapText txtInfo{ssvs::getDefaultAsset<ssvs::BitmapFont>()};

		public:
			ssvu::Delegate<void(ssvu::FT)> onUpdate;
			ssvu::Delegate<void()> onDraw;

		private:
			inline void initInput()
			{
				auto& gs(gState);

				gs.addInput({{IK::Escape}}, [this](ssvu::FT){ stop(); });

				gs.addInput({{IK::A}}, [this](ssvu::FT){ gCamera.pan(-4, 0); });
				gs.addInput({{IK::D}}, [this](ssvu::FT){ gCamera.pan(4, 0); });
				gs.addInput({{IK::W}}, [this](ssvu::FT){ gCamera.pan(0, -4); });
				gs.addInput({{IK::S}}, [this](ssvu::FT){ gCamera.pan(0, 4); });
				gs.addInput({{IK::Q}}, [this](ssvu::FT){ gCamera.zoomOut(1.1f); });
				gs.addInput({{IK::E}}, [this](ssvu::FT){ gCamera.zoomIn(1.1f); });
			}

			inline void update(ssvu::FT mFT)
			{
				onUpdate(mFT);
				gCamera.update<float>(mFT);

				const auto& gw(getGWindow());

				std::ostringstream osInfo;
				osInfo 	<< "FPS: " << ssvu::toInt(gw.getFPS())
						<< "\nU: " << gw.getMsUpdate() 
						<< "\nD: " << gw.getMsDraw() 
						<< "\n";

				txtInfo.setString(osInfo.str());
			}

			inline void draw()
			{
				gCamera.apply();
				onDraw();
				gCamera.unapply();

				render(txtInfo);
			}

		public:
			inline TestApp(ssvs::GameWindow& mGW) : App{mGW}
			{
				initInput();

				txtInfo.setPosition({5.f, 5.f});
				txtInfo.setScale({2.f, 2.f});
				txtInfo.setTracking(-3);

				gState.onUpdate += [this](ssvu::FT mFT){ update(mFT); };
				gState.onDraw += [this]{ draw(); };
			}
	};

	using TestAppRunner = AppRunner<TestApp>;
}

template<ssvu::SizeT TPrecision> struct TrigTable
{
	private:
		static constexpr float ratio{TPrecision / ssvu::tau};
		std::array<float, TPrecision> arr;

	public:
		template<typename TF> inline TrigTable(TF&& mFn) noexcept
		{
			for(auto i(0u); i < TPrecision; ++i) arr[i] = mFn(i / ratio);	
		}

		inline auto get(float mX) const noexcept 
		{ 
			SSVU_ASSERT(mX >= 0.f && mX <= ssvu::tau);
			return arr[ssvu::toInt(mX * ratio)]; 
		}
}; 

static constexpr ssvu::SizeT tablePrecision{628};

inline const auto& getSinTable() noexcept 		
{ 
	static TrigTable<tablePrecision> result{[](auto mX){ return std::sin(mX); }}; 
	return result; 
}
inline const auto& getCosTable() noexcept 		
{ 
	static TrigTable<tablePrecision> result{[](auto mX){ return std::cos(mX); }}; 
	return result; 
}

inline auto getSin(float mX) noexcept { return getSinTable().get(mX); }
inline auto getCos(float mX) noexcept { return getCosTable().get(mX); }


namespace Batch
{
	namespace Impl
	{
		using VVQuads = ssvs::VertexVector<sf::PrimitiveType::Quads>;	

		struct DrawData
		{
			std::array<ssvs::Vec2f, 4> points;
			std::array<ssvs::Vec2f, 4> tp;
			const sf::Texture* texture;
			ssvu::SizeT zOrder;
		};

		struct TextureData
		{
			sf::Vector2f halfSize, nw, ne, sw, se;
			
			inline void set(float mSizeX, float mSizeY) noexcept
			{
				halfSize = ssvs::Vec2f{mSizeX / 2.f, mSizeY / 2.f};				
				nw = ssvs::Vec2f{0, 0};
				ne = ssvs::Vec2f{mSizeX, 0};
				sw = ssvs::Vec2f{0, mSizeY};
				se = ssvs::Vec2f{mSizeX, mSizeY};
			}

			inline void set(const sf::Texture& mTexture) noexcept 				
			{ 
				set(ssvu::toFloat(mTexture.getSize().x), ssvu::toFloat(mTexture.getSize().y));
			}
		};
	}

	class Manager;

	class BatchSprite
	{
		friend class Manager;

		private:
			const sf::Texture* texture;
			Impl::TextureData textureData;

			sf::Vector2f position, origin, scale;
			float radians, rSin, rCos;
			std::size_t zOrder;

		public:
			inline void setTexture(const sf::Texture& mX) noexcept 	
			{ 
				texture = &mX; 
				textureData.set(mX);
			}


			inline void setZOrder(std::size_t mX) noexcept					{ zOrder = mX; }
			template<typename T> inline void setPosition(T&& mX) noexcept 	{ position = FWD(mX); }
			template<typename T> inline void setOrigin(T&& mX) noexcept 	{ origin = FWD(mX); }
			template<typename T> inline void setScale(T&& mX) noexcept		{ scale = FWD(mX); }

			inline void setRadians(float mX) noexcept
			{
				radians = ssvu::getWrapRad(mX);			
				rSin = getSin(radians);	
				rCos = getCos(radians);
			}

			inline const auto& getTexture() const noexcept 					{ return *texture; }
			inline const auto& getZOrder() const noexcept 					{ return zOrder; }
			inline const auto& getPosition() const noexcept 				{ return position; }
			inline const auto& getOrigin() const noexcept 					{ return origin; }
			inline const auto& getScale() const noexcept 					{ return scale; }
			inline const auto& getRadians() const noexcept 					{ return radians; }
			inline const auto& getHalfTextureSize() const noexcept 			{ return textureData.halfSize; }

			inline BatchSprite(const sf::Texture& mTexture) noexcept 
			{
				setTexture(mTexture);
			}

		private:
			inline auto getRotatedPoint(float mX, float mY, float mSSin, float mSCos, const sf::Vector2f& mTOrigin) const noexcept
			{				
				return ssvs::Vec2f
				{
					(mTOrigin.x - mX) * mSCos - (mTOrigin.y - mY) * mSSin + position.x, 
					(mTOrigin.y - mY) * mSCos + (mTOrigin.x - mX) * mSSin + position.y
				};
			}

			template<typename T> inline void emplaceVertices(T& mV) const
			{
				const auto& hs(getHalfTextureSize());
				const auto& tOrigin(origin - getHalfTextureSize());

				auto sSin(rSin * scale.y);
				auto sCos(rCos * scale.x);

				mV.emplace_back();
				auto& d(mV.back());
			
				d.zOrder = zOrder;
				d.texture = texture;

				auto erv([this, &d, &tOrigin, &sSin, &sCos](auto mI, auto mX, auto mY, const auto& mTp)
				{
					d.points[mI] = getRotatedPoint(mX, mY, sSin, sCos, tOrigin);					
					d.tp[mI] = mTp;					
				});

				erv(0, -hs.x, -hs.y, textureData.nw);
				erv(1, +hs.x, -hs.y, textureData.ne);
				erv(2, +hs.x, +hs.y, textureData.se);
				erv(3, -hs.x, +hs.y, textureData.sw);
			}
	};

	class Manager
	{	
		private:
			std::vector<Impl::DrawData> zvs;
			// std::vector<sf::Vertex> vs;			
			
			ssvu::GrowableArray<sf::Vertex> vs;
			ssvu::SizeT vsCap{0u};


		public:		
			inline void drawOn(sf::RenderTarget& mX) noexcept
			{				
				ssvu::sort(zvs, [](const auto& mA, const auto& mB)
				{ 
					if(mA.zOrder < mB.zOrder) return true;
					if(mA.zOrder > mB.zOrder) return false;

					return mA.texture < mB.texture; 
				});

				auto newCap(zvs.size() * 4);
				if(newCap != vsCap)
				{
					vs.grow(vsCap, newCap);
					vsCap = newCap;
				}
				
				
				auto curr(zvs.data());
				auto end(curr + zvs.size());
				
				while(curr != end)
				{					
					// vs.clear();
					auto cv(vs.getDataPtr());
					auto tx(curr->texture);

					do
					{
						for(auto i(0u); i < 4; ++i, ++cv) 
						{							
							cv->position = curr->points[i];
							cv->texCoords = curr->tp[i];
						}
						
						++curr;
					}
					while(curr->texture == tx);

					mX.draw(vs.getDataPtr(), cv - vs.getDataPtr(), sf::PrimitiveType::Quads, sf::RenderStates{tx});
				}

				zvs.clear();
			}

			inline void enqueue(const BatchSprite& mX)
			{				
				mX.emplaceVertices(zvs);
			}

			/*inline void directDraw(sf::RenderTarget& mRT, const BatchSprite& mX)
			{
				static Impl::VVQuads x;
				x.clear();
				mX.emplaceVertices(x);

				mRT.draw(x, sf::RenderStates{boundTextures[mX.getTexture().id]});

				sf::Sprite s;
				s.setTexture(*boundTextures[mX.getTexture().id]);
				s.setPosition(mX.getPosition());
				s.setRotation(180 + ssvu::toDeg(mX.getRadians()));
				s.setOrigin(mX.getOrigin());
				s.setScale(mX.getScale());

				mRT.draw(s);
			}*/
	};

	/*inline void Impl::Layer::drawOn(Manager& mManager, sf::RenderTarget& mX) noexcept
	{
		SSVU_ASSERT(mManager.boundTextures.size() == vVectors.size());

		for(auto i(0u); i < vVectors.size(); ++i)
		{
			auto& v(vVectors[i]);
			mX.draw(v.data(), v.size(), sf::PrimitiveType::Quads, sf::RenderStates{mManager.boundTextures[i]});
		}
	}*/
}

struct MovingThing
{
	Batch::BatchSprite spr;
	sf::Vector2f velocity;

	inline MovingThing(const Batch::BatchSprite& mSpr) : spr{mSpr}
	{
		const auto& hts(spr.getHalfTextureSize());
		spr.setOrigin(ssvs::Vec2f{ssvu::getRndR(0, hts.x), ssvu::getRndR(0, hts.y)});
		// spr.setOrigin(ssvs::Vec2f{0, 0});
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
	/*auto btxL0(bm.bind(atxL0));
	auto btxL1(bm.bind(atxL1));
	auto btxL2(bm.bind(atxL2));
	auto btxL3(bm.bind(atxL3));
	auto btlForeground(bm.mkLayer());*/

	auto btxs(ssvu::mkVector(atxL0, atxL1, atxL2, atxL3));

	Boilerplate::TestAppRunner ar{"", 800, 600};
	auto& app(ar.getApp());
	



	std::vector<MovingThing> lasers;

	for(auto i(0u); i < 150000; ++i)
	{
		MovingThing l{Batch::BatchSprite{btxs[ssvu::getRndI(0, btxs.size())]}};
		// MovingThing l{Batch::BatchSprite{btxs[0]}};
		l.spr.setPosition(sf::Vector2f{ssvu::getRndR(0.f, 800.f), ssvu::getRndR(0.f, 600.f)});
		auto r(ssvu::getRndR(0.f, ssvu::tau));
		l.velocity = ssvs::getVecFromRad(ssvu::getRndR(0.f, ssvu::tau), ssvu::getRndR(0.1f, 20.5f));
		// l.velocity = ssvs::zeroVec2f;
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
		// for(auto& l : lasers) bm.directDraw(app.getGWindow(), l.spr);

		for(auto& l : lasers) l.draw(bm);
		bm.drawOn(app.getGWindow());
	};

	app.run();
	ssvu::lo() << "end\n";

	return 0;
}