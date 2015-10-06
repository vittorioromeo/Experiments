#pragma once

class FGGameInput
{
public:
    using IK = ssvs::KKey;
    using IB = ssvs::MBtn;
    using IT = ssvs::Input::Type;
    using IM = ssvs::Input::Mode;

private:
    FGGame& game;
    int iX;
    bool iCrouch, iJump, iAttack;

    void initInput();

public:
    inline FGGameInput(FGGame& mGame) noexcept : game{mGame} { initInput(); }

    inline int getX() const noexcept { return iX; }
    inline bool getCrouch() const noexcept { return iCrouch; }
    inline bool getJump() const noexcept { return iJump; }
    inline bool getAttack() const noexcept { return iAttack; }
};

class FGGame : public Boilerplate::App
{
private:
    World world{1000, 1000, 3000, 500};
    sses::Manager mgr;
    FGFactory factory;

    FGGameInput input{*this};


    inline void initTest()
    {
        factory.createTestWall(toCr(-200, 1200), toCr(1000, 1400));
        factory.createTestEntity(
            toCr(400, 100), toCr(60, 140), Vec2f{100.f, 100.f}, true);
        factory.createTestEntity(
            toCr(600, 100), toCr(60, 140), Vec2f{100.f, 100.f}, false);
    }

    inline void update(FT mFT)
    {
        gameCamera.update<float>(mFT);
        mgr.update(mFT);
        world.update(mFT);
    }
    inline void draw()
    {
        gameCamera.apply();
        mgr.draw();
        gameCamera.unapply();
    }

public:
    inline FGGame(ssvs::GameWindow& mGameWindow)
        : Boilerplate::App{mGameWindow}, factory{*this, mgr}
    {
        gameState.onUpdate += [this](FT mFT)
        {
            update(mFT);
        };
        gameState.onDraw += [this]
        {
            draw();
        };

        initTest();
    }

    template <typename T>
    inline void render(const T& mX)
    {
        gameWindow->draw(mX);
    }

    inline auto& getFactory() noexcept { return factory; }
    inline auto& getWorld() noexcept { return world; }
    inline const auto& getInput() const noexcept { return input; }
};

inline void FGGameInput::initInput()
{
    auto& gs(game.getGameState());

    gs.addInput({{IK::Escape}}, [this](FT)
        {
            game.getGameWindow().stop();
        });

    gs.addInput({{IK::A}}, [this](FT)
        {
            game.getGameCamera().pan(-4, 0);
        });
    gs.addInput({{IK::D}}, [this](FT)
        {
            game.getGameCamera().pan(4, 0);
        });
    gs.addInput({{IK::W}}, [this](FT)
        {
            game.getGameCamera().pan(0, -4);
        });
    gs.addInput({{IK::S}}, [this](FT)
        {
            game.getGameCamera().pan(0, 4);
        });
    gs.addInput({{IK::Q}}, [this](FT)
        {
            game.getGameCamera().zoomOut(1.1f);
        });
    gs.addInput({{IK::E}}, [this](FT)
        {
            game.getGameCamera().zoomIn(1.1f);
        });

    add2StateInput(gs, {{IK::Up}}, iJump);
    add2StateInput(gs, {{IK::Down}}, iCrouch);
    add3StateInput(gs, {{IK::Left}}, {{IK::Right}}, iX);
    add2StateInput(gs, {{IK::Z}}, iAttack);
}