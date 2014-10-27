#pragma once

class FGCPhys : public sses::Component
{
	private:
		static Vec2f gravityAcc;

		FGGame& game;		
		World& world;
		Body& body;
		
	public:
		ssvu::Delegate<void(Entity&)> onDetection;
		ssvu::Delegate<void(const Vec2i&)> onResolution;

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
		inline void setWidth(float mWidth) noexcept		{ body.setWidth(mWidth); }
		inline void setHeight(float mHeight) noexcept	{ body.setHeight(mHeight); }

		inline const Vec2i& getPosI() const noexcept	{ return body.getPosition(); }
		inline Vec2f getPosPx() const noexcept			{ return toPx(body.getPosition()); }
		inline Vec2f getPosF() const noexcept			{ return Vec2f(body.getPosition()); }
		inline const Vec2f& getVel() const noexcept		{ return body.getVelocity(); }
		inline const Vec2f& getOldVel() const noexcept	{ return body.getOldVelocity(); }
		inline float getLeft() const noexcept			{ return body.getShape().getLeft(); }
		inline float getRight() const noexcept			{ return body.getShape().getRight(); }
		inline float getTop() const noexcept			{ return body.getShape().getTop(); }
		inline float getBottom() const noexcept			{ return body.getShape().getBottom(); }
	
		inline Vec2f getSizePx() const noexcept			{ return toPx(body.getSize()); }
		inline Vec2i getHalfSize() const noexcept 		{ return body.getSize() / 2; }
		inline Vec2f getHalfSizePx() const noexcept		{ return toPx(getHalfSize()); }
		
		inline float getWidth() const noexcept			{ return body.getSize().x; }
		inline float getHeight() const noexcept			{ return body.getSize().y; }
		inline float getHalfWidth() const noexcept		{ return body.getSize().x / 2; }
		inline float getHalfHeight() const noexcept		{ return body.getSize().y / 2; }

		inline auto& getBody() noexcept { return body; }
		inline const auto& getBody() const noexcept { return body; }

		inline auto& getWorld() noexcept { return world; }
		inline const auto& getWorld() const noexcept { return world; }
};

Vec2f FGCPhys::gravityAcc{0.f, 35.f};