#!/bin/zsh

nohup python3 -m http.server 8080 &

/usr/lib/emscripten/em++ -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s USE_LIBPNG=1 -s DEMANGLE_SUPPORT=1 -s ALLOW_MEMORY_GROWTH=1 \
    -I/usr/local/include -I/usr/include/glm -Iinclude \
    -I/home/vittorioromeo/OHWorkspace/vrm_pp/include \
    -I/home/vittorioromeo/OHWorkspace/vrm_core/include \
    --shell-file /usr/lib/emscripten/src/shell_minimal.html \
    --preload-file ./files/ \
    --preload-file ./vrm/sdl/glsl/ \
    --profiling \
    -std=c++14 "${@:2}" \
    -o ./build/"$1".html ./src/"$1".cpp \
&& (vblank_mode=0 __GL_SYNC_TO_VBLANK=0 chromium --disable-gpu-vsync http://localhost:8080/build/"$1".html)
# && (vblank_mode=0 firefox http://localhost:8080/"$1".html)

# -O3 -ffast-math -fno-exceptions \
# -O3 --profiling -ffast-math -fno-exceptions \
