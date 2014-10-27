#include <SSVEntitySystem/SSVEntitySystem.hpp>
#include <SSVSCollision/SSVSCollision.hpp>
#include "../AlgoVee/Common.hpp"
#include "../AlgoVee/Boilerplate.hpp"

#include "../FightGame/Code/Typedefs.hpp"
#include "../FightGame/Code/Utility.hpp"
#include "../FightGame/Code/Enums.hpp"
#include "../FightGame/Code/FwdDecl.hpp"
#include "../FightGame/Code/FGAssets.hpp"
#include "../FightGame/Code/FGFactory.hpp"
#include "../FightGame/Code/FGGame.hpp"

#include "../FightGame/Code/Components/FGCPhys.hpp"
#include "../FightGame/Code/Components/FGCPhysRender.hpp"
#include "../FightGame/Code/Components/FGCRender.hpp"
#include "../FightGame/Code/Components/FGCFighter.hpp"
#include "../FightGame/Code/Components/FGCFighterAnim.hpp"
#include "../FightGame/Code/Components/FGCPlayerControl.hpp"
#include "../FightGame/Code/Components/FGCProj.hpp"

inline void FGCPhysRender::draw()
{
	game.render(shp);
}

inline void FGCFighter::damage(FGCProj& mCProj)
{
	auto force(mCProj.getBody().getVelocity());
	force.y -= toCr(10.f);

	cPhys->getBody().applyAccel(force * 0.3f);
}

inline Entity& FGFactory::createTestWall(const Vec2i& mMin, const Vec2i& mMax)
{
	auto& e(createEntity());
	auto& cPhys(e.createComponent<FGCPhys>(game, true, ssvs::getCenter(mMin, mMax), ssvs::getSize(mMin, mMax)));
	cPhys.getBody().addGroups(FGGroup::FGGSolid);
	auto& cPhysRender(e.createComponent<FGCPhysRender>(game));
	return e;
}

inline Entity& FGFactory::createTestEntity(const Vec2i& mPos, const Vec2i& mSize, const Vec2f& mVel, bool mPlayer)
{
	auto& e(createEntity());
	auto& cPhys(e.createComponent<FGCPhys>(game, false, mPos, mSize));
	auto& cPhysRender(e.createComponent<FGCPhysRender>(game));
	auto& cFighter(e.createComponent<FGCFighter>(game));
	if(mPlayer) e.createComponent<FGCPlayerControl>(game);
	auto& cRender(e.createComponent<FGCRender>(game));
	auto& cFighterAnim(e.createComponent<FGCFighterAnim>(game, FGAssets::get().tsFighter));
	
	cPhys.setVel(mVel);

	cRender.emplaceSprite(*(FGAssets::get().txFighterMarco));
	cRender.setScaleWithBody(false);

	cRender.setGlobalScale(0.5f);	
	cRender.setGlobalOffset(Vec2f(0.f, -90.f));
	cRender.setAnchor(FGCRender::Anchor::Bottom);

	e.setDrawPriority(-1000);

	return e;
}

inline Entity& FGFactory::createProjPunch(Body& mParent, const Vec2i& mPos, const Vec2i& mSize, const Vec2f& mVel, float mLife)
{
	auto& e(createEntity());
	auto& cPhys(e.createComponent<FGCPhys>(game, false, mPos, mSize));
	auto& cPhysRender(e.createComponent<FGCPhysRender>(game));
	auto& cProj(e.createComponent<FGCProj>(game, mParent, mLife));
	
	cPhys.setVel(mVel);
	cPhys.setAffectedByGravity(false);

	return e;
}

inline void FGCRender::draw() { for(const auto& s : sprites) game.render(s); }

int main()
{
	Boilerplate::AppRunner<FGGame>{"TUMPULATUNIme", 1440, 900};
	return 0;
}

