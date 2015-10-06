#pragma once

class FGCProj : public sses::Component
{
private:
    FGGame& game;
    Body* parent;
    FGCPhys* cPhys{nullptr};
    Body* body{nullptr};
    float life;

public:
    inline FGCProj(Entity& mE, FGGame& mGame, Body& mParent, float mLife)
        : Component{mE}, game(mGame), parent{&mParent}, life{mLife}
    {
        cPhys = &getEntity().getComponent<FGCPhys>();
        body = &cPhys->getBody();

        body->addGroups(FGGroup::FGGProj);
        body->addGroupsToCheck(FGGroup::FGGSolid, FGGroup::FGGFighter);
        body->addGroupsNoResolve(FGGroup::FGGFighter);

        body->onDetection += [this](const DetectionInfo& mDI)
        {
            if(&mDI.body == parent) return;

            if(mDI.body.hasGroup(FGGroup::FGGFighter))
            {
                auto e(static_cast<Entity*>(mDI.userData));
                e->getComponent<FGCFighter>().damage(*this);
            }

            if(mDI.body.hasGroup(FGGroup::FGGSolid)) getEntity().destroy();
        };
    }

    inline void update(float mFT) override
    {
        if(life >= 0.f)
        {
            life -= mFT;
        }
        else
        {
            getEntity().destroy();
        }
    }

    inline const auto& getBody() const noexcept { return *body; }
};