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

namespace Trig
{
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
}

using namespace Trig;

namespace Batch
{
	class LSprite;
	class ZSprite;
	class Manager;

	namespace Impl
	{
		using TextureID = ssvu::SizeT;
		using LayerID = ssvu::SizeT;
		using ZOrder = ssvu::SizeT;
		using VectorQuads = ssvs::VertexVector<sf::PrimitiveType::Quads>;

		struct ZData
		{
			std::array<ssvs::Vec2f, 4> vPs;
			std::array<ssvs::Vec2f, 4> tPs;
			TextureID textureID;
			ZOrder zOrder;
		};

		struct TextureHandle
		{
			ssvs::Vec2f halfSize, nw, ne, sw, se;
			TextureID id;

			inline TextureHandle(ssvu::SizeT mID, float mSizeX, float mSizeY) noexcept
				: halfSize{mSizeX / 2.f, mSizeY / 2.f}, nw{0, 0}, ne{mSizeX, 0}, sw{0, mSizeY}, se{mSizeX, mSizeY}, id{mID}
			{

			}
		};

		struct LayerHandle
		{
			LayerID id;
		};

		struct ZLayerHandle
		{
			LayerID id;
		};

		class SpriteBase
		{
			friend class Manager;

			protected:
				Impl::TextureHandle hTexture;
				ssvs::Vec2f position, origin, scale;
				float radians, rSin, rCos;

			public:
				inline void setTexture(const Impl::TextureHandle& mX) noexcept 	{ hTexture = mX; }
				inline void setPosition(const ssvs::Vec2f& mX) noexcept 		{ position = mX; }
				inline void setOrigin(const ssvs::Vec2f& mX) noexcept 			{ origin = mX; }
				inline void setScale(const ssvs::Vec2f& mX) noexcept			{ scale = mX; }

				inline void setRadians(float mX) noexcept
				{
					radians = ssvu::getWrapRad(mX);
					rSin = getSin(radians);
					rCos = getCos(radians);
				}

				inline const auto& getTexture() const noexcept 					{ return hTexture; }
				inline const auto& getPosition() const noexcept 				{ return position; }
				inline const auto& getOrigin() const noexcept 					{ return origin; }
				inline const auto& getScale() const noexcept 					{ return scale; }
				inline const auto& getRadians() const noexcept 					{ return radians; }
				inline const auto& getTextureHalfSize() const noexcept 			{ return hTexture.halfSize; }

				inline SpriteBase(const Impl::TextureHandle& mTH) noexcept
					: hTexture(mTH)
				{

				}

			protected:
				inline auto getRotatedPoint(float mX, float mY, float mSSin, float mSCos, const ssvs::Vec2f& mTOrigin) const noexcept
				{
					return ssvs::Vec2f
					{
						(mTOrigin.x - mX) * mSCos - (mTOrigin.y - mY) * mSSin + position.x,
						(mTOrigin.y - mY) * mSCos + (mTOrigin.x - mX) * mSSin + position.y
					};
				}
		};
	}

	class LSprite : public Impl::SpriteBase
	{
		friend class Manager;

		private:
			Impl::LayerHandle hLayer;

		public:
			inline LSprite(const Impl::TextureHandle& mTH, const Impl::LayerHandle& mLH) noexcept
				: Impl::SpriteBase{mTH}, hLayer(mLH)
			{

			}

			inline void setLayer(const Impl::LayerHandle& mX) noexcept	{ hLayer = mX; }
			inline const auto& getLayer() const noexcept { return hLayer; }

		private:
			inline void emplaceRotatedVertex(Impl::VectorQuads& mV, float mX, float mY, float mSSin, float mSCos, const ssvs::Vec2f& mTOrigin, const ssvs::Vec2f& mTPos) const
			{
				mV.emplace_back(getRotatedPoint(mX, mY, mSSin, mSCos, mTOrigin), mTPos);
			}

			inline void emplaceVertices(Impl::VectorQuads& mV) const
			{
				const auto& hs(getTextureHalfSize());
				const auto& tOrigin(origin - hs);

				auto sSin(rSin * scale.y);
				auto sCos(rCos * scale.x);

				auto erv([this, &mV, &tOrigin, &sSin, &sCos](auto mX, auto mY, const auto& mTp)
				{
					emplaceRotatedVertex(mV, mX, mY, sSin, sCos, tOrigin, mTp);
				});

				erv(-hs.x, -hs.y, hTexture.nw);
				erv(+hs.x, -hs.y, hTexture.ne);
				erv(+hs.x, +hs.y, hTexture.se);
				erv(-hs.x, +hs.y, hTexture.sw);
			}
	};

	class ZSprite : public Impl::SpriteBase
	{
		friend class Manager;

		private:
			Impl::ZLayerHandle hZLayer;		
			Impl::ZOrder zOrder;

		public:
			inline ZSprite(const Impl::TextureHandle& mTH, const Impl::ZLayerHandle& mZLH, Impl::ZOrder mZOrder = 0) noexcept
				: Impl::SpriteBase{mTH}, hZLayer(mZLH), zOrder{mZOrder}
			{

			}

			inline void setZLayer(const Impl::ZLayerHandle& mX) noexcept	{ hZLayer = mX; }
			inline void setZOrder(Impl::ZOrder mX) noexcept					{ zOrder = mX; }
			
			inline const auto& getZLayer() const noexcept { return hZLayer; }
			inline const auto& getZOrder() const noexcept { return zOrder; }

		private:
			inline void emplaceVertices(Impl::ZData& mZD) const
			{
				const auto& hs(getTextureHalfSize());
				const auto& tOrigin(origin - hs);

				auto sSin(rSin * scale.y);
				auto sCos(rCos * scale.x);

				mZD.zOrder = zOrder;
				mZD.textureID = hTexture.id;

				auto erv([this, &mZD, &tOrigin, &sSin, &sCos](auto mI, auto mX, auto mY, const auto& mTp)
				{
					mZD.vPs[mI] = getRotatedPoint(mX, mY, sSin, sCos, tOrigin);
					mZD.tPs[mI] = mTp;
				});

				erv(0, -hs.x, -hs.y, hTexture.nw);
				erv(1, +hs.x, -hs.y, hTexture.ne);
				erv(2, +hs.x, +hs.y, hTexture.se);
				erv(3, -hs.x, +hs.y, hTexture.sw);
			}
	};

	namespace Impl
	{
		class Layer
		{
			friend class Batch::Manager;

			private:
				std::vector<VectorQuads> vVectors;

			public:
				inline void resize(ssvu::SizeT mX) { vVectors.resize(mX); }
				inline void clear() noexcept { for(auto& v : vVectors) v.clear(); }
				void drawOn(Manager& mManager, sf::RenderTarget& mX) noexcept;
		};

		class ZLayer
		{
			friend class Batch::Manager;

			private:
				std::vector<Impl::ZData> zvs;				
				ssvu::GrowableArray<sf::Vertex> vs;
				ssvu::SizeT vsCap{0u};

				void drawOn(Manager& mMgr, sf::RenderTarget& mX) noexcept;
		};
	}

	class Manager
	{
		friend class Impl::Layer;
		friend class Impl::ZLayer;

		private:
			struct DrawOrderItem
			{
				enum class Type{Layer, ZLayer};
				Type type;
				ssvu::SizeT idx;

				inline DrawOrderItem(Type mType, ssvu::SizeT mIdx) noexcept
					: type{mType}, idx{mIdx}
				{

				}
			};

			std::vector<const sf::Texture*> boundTextures;
			std::vector<Impl::Layer> layers;
			std::vector<Impl::ZLayer> zLayers;
			std::vector<DrawOrderItem> drawOrder;

		private:
			inline void clearLayers() noexcept { for(auto& l : layers) l.clear(); }

		public:
			inline auto bind(const sf::Texture& mX)
			{
				boundTextures.emplace_back(&mX);
				auto next(boundTextures.size());

				for(auto& l : layers) l.resize(next);

				const auto& ts(mX.getSize());
				return Impl::TextureHandle{next - 1, ssvu::toFloat(ts.x), ssvu::toFloat(ts.y)};
			}

			inline auto mkLayer()
			{
				Impl::Layer l{};
				l.resize(boundTextures.size());

				layers.emplace_back(l);
				auto lIdx(layers.size() - 1);

				drawOrder.emplace_back(DrawOrderItem::Type::Layer, lIdx);
				return Impl::LayerHandle{lIdx};
			}

			inline auto mkZLayer()
			{
				zLayers.emplace_back();

				auto lIdx(layers.size() - 1);

				drawOrder.emplace_back(DrawOrderItem::Type::ZLayer, lIdx);
				return Impl::ZLayerHandle{lIdx};
			}

			inline void drawOn(sf::RenderTarget& mX) noexcept
			{
				for(const auto& doi : drawOrder) 
				{
					if(doi.type == DrawOrderItem::Type::Layer)
					{
						layers[doi.idx].drawOn(*this, mX);
					}
					else if (doi.type == DrawOrderItem::Type::ZLayer)
					{
						zLayers[doi.idx].drawOn(*this, mX);
					}
				}

				clearLayers();
			}

			inline void enqueue(const LSprite& mX)
			{
				auto& l(layers[mX.hLayer.id]);
				auto& v(l.vVectors[mX.hTexture.id]);

				mX.emplaceVertices(v);
			}

			inline void enqueue(const ZSprite& mX)
			{
				auto& l(zLayers[mX.hZLayer.id]);

				l.zvs.emplace_back();
				auto& zd(l.zvs.back());

				mX.emplaceVertices(zd);
			}

			inline void directDraw(sf::RenderTarget& mRT, const LSprite& mX)
			{
				static Impl::VectorQuads x;
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
			}
	};

	inline void Impl::Layer::drawOn(Manager& mManager, sf::RenderTarget& mX) noexcept
	{
		SSVU_ASSERT(mManager.boundTextures.size() == vVectors.size());

		for(auto i(0u); i < vVectors.size(); ++i)
		{
			auto& v(vVectors[i]);
			mX.draw(v.data(), v.size(), sf::PrimitiveType::Quads, sf::RenderStates{mManager.boundTextures[i]});
		}
	}

	inline void Impl::ZLayer::drawOn(Batch::Manager& mMgr, sf::RenderTarget& mX) noexcept
	{				
		ssvu::sort(zvs, [](const auto& mA, const auto& mB)
		{ 
			if(mA.zOrder < mB.zOrder) return true;
			if(mA.zOrder > mB.zOrder) return false;

			return mA.textureID < mB.textureID; 
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
			auto cv(vs.getDataPtr());
			auto tx(curr->textureID);

			do
			{
				for(auto i(0u); i < 4; ++i, ++cv) 
				{							
					cv->position = curr->vPs[i];
					cv->texCoords = curr->tPs[i];
				}
				
				++curr;
			}
			while(curr->textureID == tx);

			mX.draw(vs.getDataPtr(), cv - vs.getDataPtr(), sf::PrimitiveType::Quads, sf::RenderStates{mMgr.boundTextures[tx]});
		}

		zvs.clear();
	}
}

struct MovingThing
{
	Batch::ZSprite spr;
	ssvs::Vec2f velocity;

	inline MovingThing(const decltype(spr)& mSpr) : spr{mSpr}
	{
		const auto& hts(spr.getTextureHalfSize());
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
	ssvu::lo("sizeof Batch::LSprite") << sizeof(Batch::LSprite) << "\n";
	ssvu::lo("sizeof float") << sizeof(float) << "\n";
	ssvu::lo("sizeof Vec2f") << sizeof(ssvs::Vec2f) << "\n";
	ssvu::lo("sizeof SizeT") << sizeof(ssvu::SizeT) << "\n";

	Batch::Manager bm;
	auto btxL0(bm.bind(atxL0));
	auto btxL1(bm.bind(atxL1));
	auto btxL2(bm.bind(atxL2));
	auto btxL3(bm.bind(atxL3));
	auto btlForeground(bm.mkLayer());
	auto btzlTest(bm.mkZLayer());

	auto btxs(ssvu::mkVector(btxL0, btxL1, btxL2, btxL3));

	Boilerplate::TestAppRunner ar{"", 800, 600};
	auto& app(ar.getApp());




	std::vector<MovingThing> lasers;

	for(auto i(0u); i < 150000; ++i)
	{
		MovingThing l{Batch::ZSprite{btxs[ssvu::getRndI(0, btxs.size())], btzlTest, i % 100}};
		l.spr.setPosition(ssvs::Vec2f{ssvu::getRndR(0.f, 800.f), ssvu::getRndR(0.f, 600.f)});
		auto r(ssvu::getRndR(0.f, ssvu::tau));
		l.velocity = ssvs::getVecFromRad(ssvu::getRndR(0.f, ssvu::tau), ssvu::getRndR(0.1f, 20.5f));
		// l.velocity = ssvs::zeroVec2f;
		l.spr.setRadians(r);
		auto s(ssvu::getRndR(0.1f, 1.1f));
		l.spr.setScale(ssvs::Vec2f{s, s});
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