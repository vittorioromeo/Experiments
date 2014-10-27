#pragma once

class FGAssets 
{ 
	private:
		ssvs::AssetManager assetManager;

	public:
		ssvs::SoundPlayer soundPlayer;
		ssvs::MusicPlayer musicPlayer;

		sf::Texture* txFighterMarco{nullptr};
		
		ssvs::Tileset tsFighter{ssvj::Val::fromFile("Data/Tilesets/tsFighter.json").as<ssvs::Tileset>()};

		ssvs::Animation anFgStanding, anFgStandWalking, anFgStandAttacking, anFgCrouching, anFgCrouchWalking, anFgCrouchAttacking, anFgJumping, anFgFalling;

		inline FGAssets()
		{
			txFighterMarco = &assetManager.load<sf::Texture>("txFighterMarco", "Data/Images/txFighterMarco.png");			

			auto animFile(ssvj::Val::fromFile("Data/Animations/anFighter.json"));

			anFgStanding = 			ssvs::getAnimationFromJson(tsFighter, animFile["stand"]);
			anFgStandWalking = 		ssvs::getAnimationFromJson(tsFighter, animFile["walk"]);
			anFgStandAttacking = 	ssvs::getAnimationFromJson(tsFighter, animFile["standAttack"]);
			anFgCrouching = 		ssvs::getAnimationFromJson(tsFighter, animFile["crouch"]);
			anFgCrouchWalking = 	ssvs::getAnimationFromJson(tsFighter, animFile["crouchWalk"]);
			anFgCrouchAttacking = 	ssvs::getAnimationFromJson(tsFighter, animFile["crouchAttack"]);
			anFgJumping = 			ssvs::getAnimationFromJson(tsFighter, animFile["jump"]);
			anFgFalling = 			ssvs::getAnimationFromJson(tsFighter, animFile["fall"]);
		}

		inline static auto& get()
		{
			static FGAssets result;
			return result;
		}
};