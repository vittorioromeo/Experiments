#pragma once

class FGCFighterAnim : public sses::Component
{
	private:
		FGGame& game;
		FGCRender* cRender{nullptr};
		FGCFighter* cFighter{nullptr};

		ssvs::Tileset& tileset;
		ssvs::Animation* currentAnim{nullptr};

	public:
		inline FGCFighterAnim(Entity& mE, FGGame& mGame, ssvs::Tileset& mTileset) : Component{mE}, game(mGame), tileset(mTileset) 
		{
			cRender = &getEntity().getComponent<FGCRender>();
			cFighter = &getEntity().getComponent<FGCFighter>();
		}

		inline void update(FT mFT) override
		{
			using FAC = FGCFighter::Action;
			using FMS = FGCFighter::MovStatus;
			using FAS = FGCFighter::AtkStatus;

			auto ac(cFighter->getAction());
			auto ms(cFighter->getMovStatus());
			auto as(cFighter->getAtkStatus());

			cRender->setFlippedX(cFighter->isFacingLeft());			

			if(ac == FAC::Stand)
			{
				if(ms == FMS::Move)
				{
					if(as == FAS::Attack)
					{
						currentAnim = &FGAssets::get().anFgStandAttack;
					}
					else if(as == FAS::Idle)
					{
						currentAnim = &FGAssets::get().anFgStandWalk;
					}
				}
				else if(ms == FMS::Stop)
				{
					if(as == FAS::Attack)
					{	
						currentAnim = &FGAssets::get().anFgStandAttack;
					}
					else if(as == FAS::Idle)
					{	
						currentAnim = &FGAssets::get().anFgStandStop;
					}
				}
			}
			else if(ac == FAC::Crouch)
			{
				if(ms == FMS::Move)
				{
					if(as == FAS::Attack)
					{
						currentAnim = &FGAssets::get().anFgCrouchAttack;
					}
					else if(as == FAS::Idle)
					{
						currentAnim = &FGAssets::get().anFgCrouchWalk;
					}
				}
				else if(ms == FMS::Stop)
				{
					if(as == FAS::Attack)
					{
						currentAnim = &FGAssets::get().anFgCrouchAttack;
					}
					else if(as == FAS::Idle)
					{
						currentAnim = &FGAssets::get().anFgCrouchStop;
					}
				}
			}
			else if(ac == FAC::Jump)
			{	
				currentAnim = &FGAssets::get().anFgJump;
			}
			else if(ac == FAC::Fall)
			{
				currentAnim = &FGAssets::get().anFgFall;
			}

			currentAnim->update(mFT);
			(*cRender)[0].setTextureRect(tileset(currentAnim->getTileIndex()));
		}
};
