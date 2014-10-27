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

inline void FGCPhysRender::draw()
{
	game.render(shp);
}

inline Entity& FGFactory::createTestWall(const Vec2i& mMin, const Vec2i& mMax)
{
	auto& e(createEntity());
	auto& cPhys(e.createComponent<FGCPhys>(game, true, ssvs::getCenter(mMin, mMax), ssvs::getSize(mMin, mMax)));
	cPhys.getBody().addGroups(FGGroup::FGGSolid);
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
	auto& cRender(e.createComponent<FGCRender>(game));
	auto& cFighterAnim(e.createComponent<FGCFighterAnim>(game, FGAssets::get().tsFighter));

	cRender.emplaceSprite(*(FGAssets::get().txFighterMarco));
	cRender.setScaleWithBody(false);

	cRender.globalScale = 0.5f;	
	cRender.setGlobalOffset(Vec2f(0.f, -90.f));
	cRender.anchor = FGCRender::Anchor::Bottom;

	e.setDrawPriority(-1000);

	return e;
}

inline void FGCRender::draw() { for(const auto& s : sprites) game.render(s); }

int main()
{
	Boilerplate::AppRunner<FGGame>{"UNIMETUMPULATE", 1440, 900};
	return 0;
}

