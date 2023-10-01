cl /MD /DEBUG /Z7 /std:c++17 /I include src\main.cc src\game_scene.cc src\config.cc src\sprites.cc /LINK .\lib\raylib.lib user32.lib gdi32.lib opengl32.lib kernel32.lib winmm.lib shell32.lib
