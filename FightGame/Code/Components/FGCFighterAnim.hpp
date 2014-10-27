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
		inline FGCFighterAnim(FGGame& mGame, ssvs::Tileset& mTileset) : game(mGame), tileset(mTileset) { }

		inline void init()
		{
			cRender = &getEntity().getComponent<FGCRender>();
			cFighter = &getEntity().getComponent<FGCFighter>();
		}

		inline void update(FT mFT) override
		{
			using Action = FGCFighter::Action;

			cRender->setFlippedX(cFighter->isFacingLeft());			

			switch(cFighter->getAction())
			{
				case Action::Standing: 			currentAnim = &FGAssets::get().anFgStanding; 			break;
				case Action::StandWalking: 		currentAnim = &FGAssets::get().anFgStandWalking; 		break;
				case Action::StandAttacking: 	currentAnim = &FGAssets::get().anFgStandAttacking; 		break;
				case Action::Crouching: 		currentAnim = &FGAssets::get().anFgCrouching; 			break;
				case Action::CrouchWalking: 	currentAnim = &FGAssets::get().anFgCrouchWalking; 		break;
				case Action::CrouchAttacking: 	currentAnim = &FGAssets::get().anFgCrouchAttacking; 	break;
				case Action::Jumping: 			currentAnim = &FGAssets::get().anFgJumping; 			break;
				case Action::Falling: 			currentAnim = &FGAssets::get().anFgFalling; 			break;
			}

			currentAnim->update(mFT);
			(*cRender)[0].setTextureRect(tileset(currentAnim->getTileIndex()));
		}
};
