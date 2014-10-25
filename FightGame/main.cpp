#include <SSVEntitySystem/SSVEntitySystem.hpp>
#include <SSVSCollision/SSVSCollision.hpp>
#include "../AlgoVee/Common.hpp"
#include "../AlgoVee/Boilerplate.hpp"

using Entity = sses::Entity;
using Component = sses::Component;

using World = ssvsc::World<ssvsc::HashGrid, ssvsc::Impulse>;
using Body = World::BodyType;
using Sensor = World::SensorType;
using DetectionInfo = World::DetectionInfoType;
using ResolutionInfo = World::ResolutionInfoType;

template<typename T> inline constexpr float toPixels(T mValue)					{ return mValue / 100.f; }
template<typename T> inline constexpr int toCoords(T mValue)					{ return mValue * 100; }
template<typename T> inline ssvs::Vec2f toPixels(const ssvs::Vec2<T>& mValue)	{ return {toPixels(mValue.x), toPixels(mValue.y)}; }
template<typename T> inline ssvs::Vec2i toCoords(const ssvs::Vec2<T>& mValue)	{ return {toCoords(mValue.x), toCoords(mValue.y)}; }
template<typename T> inline ssvs::Vec2i toCoords(T mX, T mY)					{ return {toCoords(mX), toCoords(mY)}; }

enum FGGroup : unsigned int
{
	FGGSolid,
	FGGFighter
};

class FGGame;
class FGFactory;

class FGCFighter;
class FGCPhys;
class FGCPhysRender;

class FGAssets 
{ 
	inline static auto& get()
	{
		static FGAssets result;
		return result;
	}
};

class FGFactory
{
	private:
		FGGame& game;
		sses::Manager& mgr;

		inline auto& createEntity(int mDrawPriority = 0) 
		{
			auto& result(mgr.createEntity()); 
			result.setDrawPriority(mDrawPriority); 
			return result; 
		}

	public:
		inline FGFactory(FGGame& mGame, sses::Manager& mMgr) : game{mGame}, mgr{mMgr} { }

		Entity& createTestWall(const Vec2i& mMin, const Vec2i& mMax);
		Entity& createTestEntity(const Vec2i& mPos, const Vec2i& mSize, const Vec2f& mVel);
};

class FGGame : public Boilerplate::App
{
	private:
		sses::Manager mgr;
		World world{1000, 1000, 3000, 500};
		FGFactory factory;

		int inputX;
		bool inputCrouching;
		bool inputJumping;

		inline void initInput()
		{
			auto& gs(gameState);

			gs.addInput({{IK::Escape}}, [this](FT){ gameWindow->stop(); });	

			gs.addInput({{IK::A}}, [this](FT){ gameCamera.pan(-4, 0); });
			gs.addInput({{IK::D}}, [this](FT){ gameCamera.pan(4, 0); });
			gs.addInput({{IK::W}}, [this](FT){ gameCamera.pan(0, -4); });
			gs.addInput({{IK::S}}, [this](FT){ gameCamera.pan(0, 4); });
			gs.addInput({{IK::Q}}, [this](FT){ gameCamera.zoomOut(1.1f); });
			gs.addInput({{IK::E}}, [this](FT){ gameCamera.zoomIn(1.1f); });

			add2StateInput(gameState, {{IK::Up}}, inputJumping);
			add2StateInput(gameState, {{IK::Down}}, inputCrouching);
			add3StateInput(gameState, {{IK::Left}}, {{IK::Right}}, inputX);
		}

		inline void initTest()
		{
			factory.createTestWall(toCoords(-200, 1200), toCoords(1000, 1400));
			factory.createTestEntity(toCoords(400, 0), toCoords(60, 140), Vec2f{100.f, 100.f});
		}

		inline void update(FT mFT)
		{
			gameCamera.update<float>(mFT);
			mgr.update(mFT);
			world.update(mFT);
			
		}
		inline void draw()
		{
			gameCamera.apply();
			mgr.draw();
			gameCamera.unapply();
		}

	public:
		inline FGGame(ssvs::GameWindow& mGameWindow) : Boilerplate::App{mGameWindow}, factory{*this, mgr}
		{
			gameState.onUpdate += [this](FT mFT){ update(mFT); };
			gameState.onDraw += [this]{ draw(); };

			initInput();
			initTest();
		}

		template<typename T> inline void render(const T& mX)
		{
			gameWindow->draw(mX);
		}

		inline auto& getWorld() noexcept { return world; }

		inline const auto& getInputX() const noexcept { return inputX; }
		inline const auto& getInputCrouching() const noexcept { return inputCrouching; }
		inline const auto& getInputJumping() const noexcept { return inputJumping; }
};

class FGCPhys : public sses::Component
{
	public:
		FGGame& game;		
		World& world;
		Body& body;

		Vec2f gravityAcc{0, 35};

		ssvu::Delegate<void(Entity&)> onDetection;
		ssvu::Delegate<void(const Vec2i&)> onResolution;

	private:

	public:
		inline FGCPhys(FGGame& mGame, bool mIsStatic, const Vec2i& mPos, const Vec2i& mSize) 
			: game(mGame), world{mGame.getWorld()}, body{world.create(mPos, mSize, mIsStatic)} { }
		
		inline void init() { }

		inline void update(FT mFT) override
		{
			body.applyAccel(gravityAcc);			
		}

		inline void setPos(const Vec2i& mPos) noexcept	{ body.setPosition(mPos); }
		inline void setVel(const Vec2f& mVel) noexcept	{ body.setVelocity(mVel); }

		inline void setMass(float mMass) noexcept		{ body.setMass(mMass); }	
		inline const Vec2i& getPosI() const noexcept	{ return body.getPosition(); }
		inline Vec2f getPosPx() const noexcept			{ return toPixels(body.getPosition()); }
		inline Vec2f getPosF() const noexcept			{ return Vec2f(body.getPosition()); }
		inline const Vec2f& getVel() const noexcept		{ return body.getVelocity(); }
		inline const Vec2f& getOldVel() const noexcept	{ return body.getOldVelocity(); }
		inline float getLeft() const noexcept			{ return body.getShape().getLeft(); }
		inline float getRight() const noexcept			{ return body.getShape().getRight(); }
		inline float getTop() const noexcept			{ return body.getShape().getTop(); }
		inline float getBottom() const noexcept			{ return body.getShape().getBottom(); }
	
		inline Vec2f getSizePx() const noexcept			{ return toPixels(body.getSize()); }
		inline Vec2i getHalfSize() const noexcept 		{ return body.getSize() / 2; }
		inline Vec2f getHalfSizePx() const noexcept		{ return toPixels(getHalfSize()); }
		
		inline float getWidth() const noexcept			{ return body.getSize().x; }
		inline float getHeight() const noexcept			{ return body.getSize().y; }
		inline float getHalfWidth() const noexcept		{ return body.getSize().x / 2; }
		inline float getHalfHeight() const noexcept		{ return body.getSize().y / 2; }
};

class FGCPhysRender : public sses::Component
{
	public:	
		FGGame& game;
		const FGCPhys* cPhys{nullptr};

	private:
		sf::RectangleShape shp;

	public:
		inline FGCPhysRender(FGGame& mGame) : game(mGame) { }

		inline void init()
		{
			cPhys = &getEntity().getComponent<FGCPhys>();
		}

		inline void update(FT mFT) override
		{
			shp.setPosition(cPhys->getPosPx());
			shp.setSize(cPhys->getSizePx());
			shp.setOrigin(cPhys->getHalfSizePx());
		}

		void draw() override;
};

struct FighterClass
{
	int hp;
	
	int speedStanding;
	int speedCrouching;
	
	int jumpStrength;

	int width;

	int heightStanding;
	int heightCrouching;
};

inline auto getTestFC()
{
	FighterClass result;

	result.hp = 100;
	
	result.speedStanding = 10;
	result.speedCrouching = 6;
	
	result.jumpStrength = 25;

	result.width = 60;

	result.heightStanding = 150;
	result.heightCrouching = 65;

	return result;
}

class FGCFighter : public sses::Component
{
	public:	
		enum class Dir{Left, Right};

		FGGame& game;
		FGCPhys* cPhys{nullptr};
		Body* body{nullptr};
		
		Sensor* groundSensor{nullptr};
		bool inAir{false};

		FighterClass fc;
		int hp;

	private:
		Dir dir;
		bool crouching{true};

	public:
		inline void unCrouch()
		{
			if(crouching)
			{
				body->setPosition(body->getPosition() - Vec2i{0, (toCoords(fc.heightStanding) - toCoords(fc.heightCrouching)) / 2});
				body->setHeight(toCoords(fc.heightStanding));
			}
			crouching = false;
		}
		inline void crouch()
		{
			if(isInAir()) return;
			if(!crouching)
			{
				body->setPosition(body->getPosition() + Vec2i{0, (toCoords(fc.heightStanding) - toCoords(fc.heightCrouching)) / 2});
				body->setHeight(toCoords(fc.heightCrouching));
			}
			crouching = true;
		}

		inline bool isInAir() const noexcept { return inAir; }

	public:
		inline FGCFighter(FGGame& mGame) : game(mGame) { }

		inline void init()
		{
			cPhys = &getEntity().getComponent<FGCPhys>();
			body = &cPhys->body;

			groundSensor = &cPhys->world.createSensor(body->getPosition(), Vec2i(100, 100));
			groundSensor->addGroupsToCheck(FGGroup::FGGSolid);

			fc = getTestFC();
			hp = fc.hp;

			getEntity().addGroups(FGGroup::FGGSolid, FGGroup::FGGFighter);
			body->addGroups(FGGroup::FGGSolid, FGGroup::FGGFighter);
			body->addGroupsToCheck(FGGroup::FGGSolid, FGGroup::FGGFighter);
			body->setRestitutionX(0.3f);
			body->setRestitutionY(0.3f);
			body->setMass(1.f);
			body->setVelTransferMultX(0.6f);			
			body->setWidth(toCoords(fc.width));

			body->onPreUpdate += [this]
			{
				
			};

			groundSensor->onPreUpdate += [this]
			{ 
				groundSensor->setPosition(body->getPosition() + Vec2i{0, body->getHeight() / 2}); 
				inAir = true; 
			};
			groundSensor->onDetection += [this](const DetectionInfo& mDI)
			{
				if(&mDI.body == this->body) return;
				inAir = false;				
			};
		}

		inline void update(FT) override
		{
		}

		inline void draw() override
		{
			if(groundSensor == nullptr) return; 
			
			sf::RectangleShape xshp;
			xshp.setOrigin(Vec2f(0.5f, 0.5f));
			xshp.setFillColor(sf::Color::Red);
			xshp.setPosition(toPixels(body->getPosition() + Vec2i{0, body->getHeight() / 2}));
			xshp.setSize(Vec2f(1, 1));
			game.render(xshp);
		}

		inline void moveLeft()
		{
			auto speed(crouching ? fc.speedCrouching : fc.speedStanding);
			auto crSpd(toCoords(speed));

			auto diff((-crSpd) - body->getVelocity().x);
			ssvu::clamp(diff, -crSpd, crSpd);

			body->applyAccel(Vec2f(diff, 0.f));
		}

		inline void moveRight()
		{
			auto speed(crouching ? fc.speedCrouching : fc.speedStanding);
			auto crSpd(toCoords(speed));

			auto diff(crSpd - body->getVelocity().x);
			ssvu::clamp(diff, -crSpd, crSpd);

			body->applyAccel(Vec2f(diff, 0.f));
		}

		inline void stop()
		{
			auto diff(0 - body->getVelocity().x);

			body->applyAccel(Vec2f(diff * 0.9f, 0.f));
		}
		
		inline void jump()
		{
			if(isInAir() || crouching) return;
			body->applyAccel(Vec2f(0.f, -toCoords(fc.jumpStrength)));
		}
};

class FGCPlayerControl : public sses::Component
{
	public:	
		FGGame& game;
		FGCFighter* cFighter{nullptr};

	public:
		inline FGCPlayerControl(FGGame& mGame) : game(mGame) { }

		inline void init()
		{
			cFighter = &getEntity().getComponent<FGCFighter>();
		}

		inline void update(FT)
		{
			if(game.getInputX() == 1)
			{
				cFighter->moveRight();
			}
			else if(game.getInputX() == -1)
			{
				cFighter->moveLeft();
			}
			else
			{
				cFighter->stop();
			}

			if(game.getInputCrouching())
			{
				cFighter->crouch();
			}	
			else
			{
				cFighter->unCrouch();
			}

			if(game.getInputJumping()) cFighter->jump();
		}
};





inline void FGCPhysRender::draw()
{
	game.render(shp);
}

inline Entity& FGFactory::createTestWall(const Vec2i& mMin, const Vec2i& mMax)
{
	auto& e(createEntity());
	auto& cPhys(e.createComponent<FGCPhys>(game, true, ssvs::getCenter(mMin, mMax), ssvs::getSize(mMin, mMax)));
	cPhys.body.addGroups(FGGroup::FGGSolid);
	auto& cPhysRender(e.createComponent<FGCPhysRender>(game));
	return e;
}

inline Entity& FGFactory::createTestEntity(const Vec2i& mPos, const Vec2i& mSize, const Vec2f& mVel)
{
	auto& e(createEntity());
	auto& cPhys(e.createComponent<FGCPhys>(game, false, mPos, mSize));
	cPhys.setVel(mVel);
	auto& cPhysRender(e.createComponent<FGCPhysRender>(game));
	auto& cFighter(e.createComponent<FGCFighter>(game));
	auto& cPlayerControl(e.createComponent<FGCPlayerControl>(game));
	return e;
}


int main()
{
	Boilerplate::AppRunner<FGGame>{"UNIMETUMPULATE", 1440, 900};
	return 0;
}

