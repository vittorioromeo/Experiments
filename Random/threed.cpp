#include <SSVUtils/SSVUtils.hpp>
#include <SSVStart/SSVStart.hpp>
#include <glm/glm.hpp>
#include <GL/glut.h>

namespace Boilerplate
{
class App
{
protected:
    using IK = ssvs::KKey;
    using IB = ssvs::MBtn;
    using IT = ssvs::Input::Type;
    using IM = ssvs::Input::Mode;

    ssvs::GameState gameState;
    ssvs::Camera gameCamera;
    ssvs::GameWindow& gameWindow;

public:
    inline App(ssvs::GameWindow& mGameWindow)
        : gameCamera{mGameWindow, 1.f}, gameWindow{mGameWindow}
    {
    }

    inline void stop() noexcept { return gameWindow.stop(); }

    template <typename... TArgs>
    inline void render(TArgs&&... mArgs)
    {
        gameWindow.draw(FWD(mArgs)...);
    }

    inline auto& getGameState() noexcept { return gameState; }
    inline const auto& getGameState() const noexcept { return gameState; }
    inline auto& getGameCamera() noexcept { return gameCamera; }
    inline const auto& getGameCamera() const noexcept { return gameCamera; }
    inline auto& getGameWindow() noexcept { return gameWindow; }
    inline const auto& getGameWindow() const noexcept { return gameWindow; }
};

auto makeBitmapText()
{
    ssvs::BitmapText result{ssvs::getDefaultAsset<ssvs::BitmapFont>()};
    result.setTracking(-3);
    return result;
}

class TestApp : public Boilerplate::App
{
protected:
    using FT = ssvs::FT;

public:
    ssvu::Delegate<void(std::ostringstream&, FT)> onTxtInfoUpdate;

private:
    FT txtInfoUpdateRate, txtInfoUpdateFT{0.f};
    std::ostringstream ossInfo;
    ssvs::BitmapText txtInfo{makeBitmapText()};

    void initTxtInfo()
    {
        txtInfo.setPosition(5.f, 5.f);
        txtInfo.setColor(sf::Color::White);
    }

    void initInput()
    {
        gameState.addInput({{IK::Escape}},
        [this](FT)
        {
            stop();
        },
        IT::Once);
        gameState.addInput({{IK::A}}, [this](FT)
        {
            gameCamera.pan(-4, 0);
        });
        gameState.addInput({{IK::D}}, [this](FT)
        {
            gameCamera.pan(4, 0);
        });
        gameState.addInput({{IK::W}}, [this](FT)
        {
            gameCamera.pan(0, -4);
        });
        gameState.addInput({{IK::S}}, [this](FT)
        {
            gameCamera.pan(0, 4);
        });
        gameState.addInput({{IK::Q}}, [this](FT)
        {
            gameCamera.zoomOut(1.1f);
        });
        gameState.addInput({{IK::E}}, [this](FT)
        {
            gameCamera.zoomIn(1.1f);
        });
    }

    void updateTxtInfo(FT mFT)
    {
        txtInfoUpdateFT += mFT;
        if(txtInfoUpdateFT < txtInfoUpdateRate) return;

        txtInfoUpdateFT = 0;

        txtInfo.setString(ossInfo.str());
        ossInfo.str("");

        ossInfo << "FPS: " << gameWindow.getFPS() << "\n"
                << "U: " << gameWindow.getMsUpdate() << "\t"
                << "D: " << gameWindow.getMsDraw() << "\n";

        onTxtInfoUpdate(ossInfo, mFT);

        ossInfo << "\n";
    }

    void drawTxtInfo() { render(txtInfo); }

public:
    TestApp(ssvs::GameWindow& mGameWindow) : Boilerplate::App{mGameWindow}
    {
        initTxtInfo();
        initInput();

        gameState.onUpdate += [this](FT mFT)
        {
            gameCamera.update(mFT);
            update(mFT);
        };

        gameState.onDraw += [this]
        {
            gameCamera.apply();
            draw();
            gameCamera.unapply();
        };

        enableTxtInfo(10.f);
    }

    virtual ~TestApp() {}

    void enableTxtInfo(FT mUpdateRate)
    {
        txtInfoUpdateRate = mUpdateRate;
        gameState.onUpdate += [this](FT mFT)
        {
            updateTxtInfo(mFT);
        };
        gameState.onDraw += [this]
        {
            drawTxtInfo();
        };
    }

    virtual void update(FT) {}
    virtual void draw() {}
};

template <typename T>
class AppRunner
{
private:
    ssvs::GameWindow gameWindow;
    ssvu::UPtr<T> app;

public:
    inline AppRunner(
    const std::string& mTitle, ssvu::SizeT mWidth, ssvu::SizeT mHeight)
    {
        gameWindow.setTitle(mTitle);
        gameWindow.setTimer<ssvs::TimerStatic>(0.5f, 0.5f);
        gameWindow.setSize(mWidth, mHeight);
        gameWindow.setFullscreen(false);
        gameWindow.setFPSLimited(true);
        gameWindow.setMaxFPS(200);
        gameWindow.setPixelMult(1);

        app = ssvu::mkUPtr<T>(gameWindow);

        gameWindow.setGameState(app->getGameState());
        gameWindow.recreate();
        gameWindow.run();
    }
};
}

using ssvs::FT;
using ssvs::Vec2f;
using ssvs::Vec2i;
using ssvs::SizeT;

struct Game : Boilerplate::TestApp
{
    float angle = 0.f;
    float xdf = 50.f;

    void initGLStuff()
    {
        glClearDepth(1.f);
        glClearColor(0.3f, 0.3f, 0.3f, 0.f);
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(90.f, 1.f, 1.f, 300.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void deinitGLStuff() { glDepthMask(GL_FALSE); }

    Game(ssvs::GameWindow& mX) : Boilerplate::TestApp{mX}
    {
        // We can print debug text with this "delegate".
        onTxtInfoUpdate += [this](auto& oss, FT)
        {
            oss << "Hello world!\n";
        };
    }

    // Logic can be defined by overriding the following methods.
    void update(FT mFT) override
    {
        xdf += mFT * 0.1f;
        angle += mFT * 0.1f;
    }
    void draw() override
    {
        initGLStuff();
        getGameWindow().getRenderWindow().pushGLStates();


        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0.f, 0.f, -0.f);

        glRotatef(angle * 50, 1.f, 0.f, 0.f);
        glRotatef(angle * 30, 0.f, 1.f, 0.f);
        glRotatef(angle * 90, 0.f, 0.f, 1.f);



        // Draw a cube
        glBegin(GL_QUADS); // draw some squares
        glColor3i(0, 1, 1);
        glVertex3f(-xdf, -xdf, -xdf);
        glVertex3f(-xdf, xdf, -xdf);
        glVertex3f(xdf, xdf, -xdf);
        glVertex3f(xdf, -xdf, -xdf);

        glColor3f(0, 0, 1);
        glVertex3f(-xdf, -xdf, xdf);
        glVertex3f(-xdf, xdf, xdf);
        glVertex3f(xdf, xdf, xdf);
        glVertex3f(xdf, -xdf, xdf);

        glColor3f(1, 0, 1);
        glVertex3f(-xdf, -xdf, -xdf);
        glVertex3f(-xdf, xdf, -xdf);
        glVertex3f(-xdf, xdf, xdf);
        glVertex3f(-xdf, -xdf, xdf);

        glColor3f(0, 1, 0);
        glVertex3f(xdf, -xdf, -xdf);
        glVertex3f(xdf, xdf, -xdf);
        glVertex3f(xdf, xdf, xdf);
        glVertex3f(xdf, -xdf, xdf);

        glColor3f(1, 1, 0);
        glVertex3f(-xdf, -xdf, xdf);
        glVertex3f(-xdf, -xdf, -xdf);
        glVertex3f(xdf, -xdf, -xdf);
        glVertex3f(xdf, -xdf, xdf);

        glColor3f(1, 0, 0);
        glVertex3f(-xdf, xdf, xdf);
        glVertex3f(-xdf, xdf, -xdf);
        glVertex3f(xdf, xdf, -xdf);
        glVertex3f(xdf, xdf, xdf);

        glEnd();

        getGameWindow().getRenderWindow().popGLStates();

        deinitGLStuff();
    }
};

int main()
{



    Boilerplate::AppRunner<Game>{"3d", 640, 480};


    return 0;
}