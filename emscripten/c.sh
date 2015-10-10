#!/bin/zsh

nohup python3 -m http.server 8080 &
sudo cp -R ./vrm/ /usr/local/include/

/usr/lib/emscripten/em++ -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s USE_LIBPNG=1 -s DEMANGLE_SUPPORT=1 \
    -I/usr/local/include \
    --shell-file /usr/lib/emscripten/src/shell_minimal.html \
    --preload-file ./files/ \
    -std=c++14 "${@:2}" \
    -o "$1".html ./"$1".cpp \
&& (vblank_mode=0 chromium --disable-gpu-vsync http://localhost:8080/"$1".html)
# -O3 -ffast-math -fno-exceptions \
# -O3 --profiling -ffast-math -fno-exceptions \
