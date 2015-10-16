#include <exception>
#include <functional>

#include <emscripten.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_opengles2.h>

const char* vShaderStr =
    "attribute vec4 vPosition;    \n"
    "void main()                  \n"
    "{                            \n"
    "   gl_Position = vec4(vPosition.xyz, 1.0);  \n"
    "}                            \n";

const char* fShaderStr =
    "precision mediump float;\n"
    "void main()                                  \n"
    "{                                            \n"
    "  gl_FragColor = vec4 (1.0, 1.0, 1.0, 1.0 );\n"
    "}                                            \n";

std::function<void()> loop;
void main_loop() { loop(); }

int main(int argc, char** argv)
{
    SDL_Init(SDL_INIT_VIDEO);

    auto wnd(
        SDL_CreateWindow("test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN));

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetSwapInterval(0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    auto glc = SDL_GL_CreateContext(wnd);

    auto rdr = SDL_CreateRenderer(
        wnd, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

    auto v_shader(glCreateShader(GL_VERTEX_SHADER));
    glShaderSource(v_shader, 1, &vShaderStr, nullptr);
    glCompileShader(v_shader);

    auto f_shader(glCreateShader(GL_FRAGMENT_SHADER));
    glShaderSource(f_shader, 1, &fShaderStr, nullptr);
    glCompileShader(f_shader);

    auto program(glCreateProgram());
    glAttachShader(program, v_shader);
    glAttachShader(program, f_shader);
    glLinkProgram(program);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    GLfloat vVertices[] = {
        0.0f, 0.5f, 0.0f, -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f};


    loop = [&]
    {
        SDL_Event e;
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_QUIT) std::terminate();
        }

        glUseProgram(program);


        glViewport(0, 0, 640, 480);
        glClear(
            GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // Load the vertex data
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
        glEnableVertexAttribArray(0);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        SDL_GL_SwapWindow(wnd);
    };

    emscripten_set_main_loop(main_loop, 0, true);

    return 0;
}