#pragma once

class FGAssets
{
private:
    ssvs::AssetManager assetManager;

public:
    ssvs::SoundPlayer soundPlayer;
    ssvs::MusicPlayer musicPlayer;

    sf::Texture* txFighterMarco{nullptr};

    ssvs::Tileset tsFighter{ssvj::Val::fromFile("Data/Tilesets/tsFighter.json")
                                .as<ssvs::Tileset>()};

    ssvs::Animation anFgStandStop, anFgStandWalk, anFgStandAttack,
        anFgCrouchStop, anFgCrouchWalk, anFgCrouchAttack, anFgJump, anFgFall;

    inline FGAssets()
    {
        txFighterMarco = &assetManager.load<sf::Texture>(
            "txFighterMarco", "Data/Images/txFighterMarco.png");

        auto animFile(ssvj::Val::fromFile("Data/Animations/anFighter.json"));

        anFgStandStop =
            ssvs::getAnimationFromJson(tsFighter, animFile["stand"]);
        anFgStandWalk = ssvs::getAnimationFromJson(tsFighter, animFile["walk"]);
        anFgStandAttack =
            ssvs::getAnimationFromJson(tsFighter, animFile["standAttack"]);
        anFgCrouchStop =
            ssvs::getAnimationFromJson(tsFighter, animFile["crouch"]);
        anFgCrouchWalk =
            ssvs::getAnimationFromJson(tsFighter, animFile["crouchWalk"]);
        anFgCrouchAttack =
            ssvs::getAnimationFromJson(tsFighter, animFile["crouchAttack"]);
        anFgJump = ssvs::getAnimationFromJson(tsFighter, animFile["jump"]);
        anFgFall = ssvs::getAnimationFromJson(tsFighter, animFile["fall"]);
    }

    inline static auto& get()
    {
        static FGAssets result;
        return result;
    }
};