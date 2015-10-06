#pragma once

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
    inline FGFactory(FGGame& mGame, sses::Manager& mMgr)
        : game{mGame}, mgr{mMgr}
    {
    }

    Entity& createTestWall(const Vec2i& mMin, const Vec2i& mMax);
    Entity& createTestEntity(
        const Vec2i& mPos, const Vec2i& mSize, const Vec2f& mVel, bool mPlayer);

    Entity& createProjPunch(Body& mParent, const Vec2i& mPos,
        const Vec2i& mSize, const Vec2f& mVel, float mLife);
};