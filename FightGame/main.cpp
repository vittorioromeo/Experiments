#include <SSVEntitySystem/SSVEntitySystem.hpp>
#include "../AlgoVee/Common.hpp"
#include "../AlgoVee/Boilerplate.hpp"

template<typename T> inline constexpr float toPixels(T mValue)					{ return mValue / 100.f; }
template<typename T> inline constexpr int toCoords(T mValue)					{ return mValue * 100; }
template<typename T> inline ssvs::Vec2f toPixels(const ssvs::Vec2<T>& mValue)	{ return {toPixels(mValue.x), toPixels(mValue.y)}; }
template<typename T> inline ssvs::Vec2i toCoords(const ssvs::Vec2<T>& mValue)	{ return {toCoords(mValue.x), toCoords(mValue.y)}; }

class FGGame;
class FGCPhys;

class FGCPhys : public sses::Component
{
	public:
		FGGame& game;		
		Vec2i pos, size;
		Vec2f vel, acc;

	private:
		inline void integrate(FT mFT)
		{
			vel += acc * mFT;
			pos += Vec2i(vel * mFT);
			ssvs::nullify(acc);
		}

	public:
		inline FGCPhys(FGGame& mGame, const Vec2i& mPos, const Vec2i& mSize) 
			: game(mGame), pos{mPos}, size{mSize} { }
		
		inline void init() { }

		inline void update(FT mFT) override
		{
			integrate(mFT);
		}

		inline Vec2f getPosPx() const noexcept			{ return toPixels(pos); }
		inline Vec2f getPosF() const noexcept			{ return Vec2f(pos); }

		inline Vec2f getSizePx() const noexcept			{ return toPixels(size); }

		inline Vec2i getHalfSize() const noexcept 		{ return size / 2; }
		inline Vec2f getHalfSizePx() const noexcept		{ return toPixels(getHalfSize()); }
		
		inline float getWidth() const noexcept			{ return size.x; }
		inline float getHeight() const noexcept			{ return size.y; }
		inline float getHalfWidth() const noexcept		{ return size.x / 2; }
		inline float getHalfHeight() const noexcept		{ return size.y / 2; }

		inline float getLeft() const noexcept			{ return pos.x - getHalfWidth(); }
		inline float getRight() const noexcept			{ return pos.x + getHalfWidth(); }
		inline float getTop() const noexcept			{ return pos.y - getHalfHeight(); }
		inline float getBottom() const noexcept			{ return pos.y + getHalfHeight(); }
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



class FGFactory
{
	private:
		FGGame& game:

	public:
		inline FGFactory(FGGame& mGame) : game{mGame} { }
};

class FGGame : public Boilerplate::App
{
	private:
		sses::Manager manager;

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
		}

		inline void initTest()
		{

		}

		inline void update(FT mFT)
		{
			gameCamera.update<float>(mFT);
			manager.update(mFT);
			
		}
		inline void draw()
		{
			gameCamera.apply();
			manager.draw();
			gameCamera.unapply();
		}

	public:
		inline FGGame(ssvs::GameWindow& mGameWindow) : Boilerplate::App{mGameWindow}
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
};



inline void FGCPhysRender::draw()
{
	game.render(shp);
}



int main()
{
	Boilerplate::AppRunner<FGGame>{"UNIMETUMPULATE", 1440, 900};
	return 0;
}

