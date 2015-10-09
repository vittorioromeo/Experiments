#define GLEW_STATIC
#include <SSVUtils/SSVUtils.hpp>
#include <SSVStart/SSVStart.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
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

    inline auto& getRenderWindow() noexcept
    {
        return getGameWindow().getRenderWindow();
    }
    inline const auto& getRenderWindow() const noexcept
    {
        return getGameWindow().getRenderWindow();
    }

    inline void pushGLStates() noexcept { getRenderWindow().pushGLStates(); }
    inline void popGLStates() noexcept { getRenderWindow().popGLStates(); }
    inline void resetGLStates() noexcept { getRenderWindow().resetGLStates(); }
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
            glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind all the buffers
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            glBindVertexArray(0);

            pushGLStates();
            drawTxtInfo();
            popGLStates();
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

// Shader sources
const GLchar* vertexSource =
R"(
#version 150 core

in vec2 position;
in vec3 color;
out vec3 Color;

uniform mat4 model;
uniform mat4 projection;

void main() {
   Color = color;
   gl_Position = projection * model * vec4(position, 0.0, 1.0);
}
)";

const GLchar* fragmentSource =
R"(
#version 150 core

in vec3 Color;
out vec4 outColor;
void main() {
   outColor = vec4(Color, 1.0);
}
)";

namespace vrm
{
namespace gl
{
    namespace Impl
    {
        template <typename T, typename TDeleter>
        class unique_resource
        {
        private:
            T id;
            TDeleter deleter;
            bool has_res{true};

        public:
            unique_resource(const T& mX) : id{mX} {}

            unique_resource(const unique_resource&) = delete;
            unique_resource& operator=(const unique_resource&) = delete;

            unique_resource(unique_resource&& mX)
                : id{mX.id}, deleter{mX.deleter}
            {
                mX.has_res = false;
            }

            unique_resource& operator=(unique_resource&& mX)
            {
                id = mX.id;
                deleter = mX.deleter;

                has_res = true;
                mX.has_res = false;

                return *this;
            }

            ~unique_resource()
            {
                if(has_res) {
                    deleter(id);
                }
            }

            operator T() const noexcept
            {
                assert(has_res);
                return id;
            }
        };

        struct ShaderDeleter
        {
            void operator()(GLint id) { glDeleteShader(id); }
        };

        struct ProgramDeleter
        {
            void operator()(GLint id) { glDeleteProgram(id); }
        };
    }

    using unique_shader = Impl::unique_resource<GLint, Impl::ShaderDeleter>;
    using unique_program = Impl::unique_resource<GLint, Impl::ProgramDeleter>;

    auto make_shader(GLenum type, const GLchar** src) noexcept
    {
        constexpr std::size_t shaderLogBufferSize{512};
        thread_local char logBuffer[shaderLogBufferSize];
        thread_local std::string logString;

        auto id(glCreateShader(type));
        glShaderSource(id, 1, src, nullptr);
        glCompileShader(id);

        GLint status;
        glGetShaderiv(id, GL_COMPILE_STATUS, &status);

        glGetShaderInfoLog(id, shaderLogBufferSize, nullptr, logBuffer);
        logString = logBuffer;

        if(!logString.empty())
            ssvu::lo("Shader compilation log") << logString << std::endl;

        return unique_shader{id};
    }

    class program
    {
    private:
        unique_program id;

    public:
        program(unique_program&& mResource) : id{std::move(mResource)} {}

        operator GLint() const noexcept { return id; }

        template <typename... TShaders>
        inline void attach_and_link(TShaders&&... mShaders) noexcept
        {
            ssvu::forArgs(
            [this](auto&& s)
            {
                glAttachShader(id, FWD(s));
            },
            FWD(mShaders)...);

            glLinkProgram(id);

            ssvu::forArgs(
            [this](auto&& s)
            {
                glDetachShader(id, FWD(s));
            },
            FWD(mShaders)...);
        }

        inline void use() noexcept { glUseProgram(id); }

        inline auto attribute(const std::string& mName) const noexcept
        {
            return glGetAttribLocation(id, mName.c_str());
        }
    };

    template <typename... TShaders>
    auto make_program(TShaders&&... mShaders) noexcept
    {
        auto id(glCreateProgram());

        unique_program res{id};
        program p{std::move(res)};

        p.attach_and_link(FWD(mShaders)...);

        return p;
    }
}
}

struct Game : Boilerplate::TestApp
{
    Game(ssvs::GameWindow& mX) : Boilerplate::TestApp{mX}
    {
        onTxtInfoUpdate += [this](auto& oss, FT)
        {
            oss << "Hello world!\n";
        };

        glewExperimental = GL_TRUE;
        glewInit();

        gameState.addInput({{IK::A}}, [this](FT ft)
        {
            x -= 1.f * ft;
        });
        gameState.addInput({{IK::D}}, [this](FT ft)
        {
            x += 1.f * ft;
        });
        gameState.addInput({{IK::W}}, [this](FT ft)
        {
            z -= 1.f * ft;
        });
        gameState.addInput({{IK::S}}, [this](FT ft)
        {
            z += 1.f * ft;
        });
        gameState.addInput({{IK::Q}}, [this](FT ft)
        {
            y -= 1.f * ft;
        });
        gameState.addInput({{IK::E}}, [this](FT ft)
        {
            y += 1.f * ft;
        });
    }

    ~Game() {}

    float x = 0, y = 0, z = 0;

    void update(FT mFT) override {}
    void draw() override
    {


        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // Create a Vertex Buffer Object and copy the vertex data to it
        GLuint vbo;
        glGenBuffers(1, &vbo);

        GLfloat vertices[] = {
        -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, // Top-left
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f,  // Top-right
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right
        -0.5f, -0.5f, 1.0f, 1.0f, 1.0f // Bottom-left
        };

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(
        GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Create an element array
        GLuint ebo;
        glGenBuffers(1, &ebo);

        GLuint elements[] = {0, 1, 2, 2, 3, 0};

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(
        GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

        // Create and compile the vertex shader
        auto vertexShader =
        vrm::gl::make_shader(GL_VERTEX_SHADER, &vertexSource);



        // Create and compile the fragment shader
        auto fragmentShader =
        vrm::gl::make_shader(GL_FRAGMENT_SHADER, &fragmentSource);

        // Link the vertex and fragment shader into a shader program
        auto shaderProgram =
        vrm::gl::make_program(vertexShader, fragmentShader);

        glBindFragDataLocation(shaderProgram, 0, "outColor");
        glUseProgram(shaderProgram);


        // Specify the layout of the vertex data
        auto posAttrib = shaderProgram.attribute("position");
        glEnableVertexAttribArray(posAttrib);
        glVertexAttribPointer(
        posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);

        auto colAttrib = shaderProgram.attribute("color");
        glEnableVertexAttribArray(colAttrib);
        glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE,
        5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));


        auto projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);
        //auto projection = glm::mat4(1.0f) * glm::perspective(90.f, x + 1.0f, y + 0.1f, z+  100.0f);

        glm::vec2 position(200.f, 200.f);
        glm::vec2 size(300.f, 300.f);
        float rotate(x * 0.2);

        glm::mat4 model;
        model = glm::translate(model,
        glm::vec3(position, 0.0f)); // First translate (transformations are:
                                    // scale happens first, then rotation and
                                    // then finall translation happens; reversed
                                    // order)

        model = glm::translate(
        model, glm::vec3(0.5f * size.x, 0.5f * size.y,
               0.0f)); // Move origin of rotation to center of quad
        model =
        glm::rotate(model, rotate, glm::vec3(-0.01f, 0.01f, 1.0f)); // Then rotate
        model = glm::translate(model,
        glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // Move origin back

        model = glm::scale(model, glm::vec3(size, 1.0f)); // Last scale

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1,
        GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1,
        GL_FALSE, glm::value_ptr(projection));


        // Clear the screen to black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw a rectangle from the 2 triangles using 6 indices
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
};

int main()
{


    Boilerplate::AppRunner<Game>{"3d", 800, 600};
    return 0;
}