Dependencies:
- MinGW-w64
- raylib 6.0 (https://github.com/raysan5/raylib/releases)

Build:
gcc -o game.exe main.c -I path/to/raylib/include -L path/to/raylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm