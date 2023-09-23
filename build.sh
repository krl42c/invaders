clang++ \
    src/main.cc src/game_scene.cc src/config.cc \
    -L/opt/homebrew/Cellar/raylib/4.5.0/lib -I/opt/homebrew/Cellar/raylib/4.5.0/include -lraylib \
    -std=c++17 \
    -o spacething\
    -Wall -Wextra -Wpedantic\
