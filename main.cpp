#include "raylib.h"
#include <iostream>
#include "tries.h"
#include <string>

int main() {
    const int HEIGHT = 720;
    const int WIDTH = 480;
    InitWindow(WIDTH, HEIGHT, "Typing Game");
    SetTargetFPS(60);
    while(!WindowShouldClose()){
        BeginDrawing();
        DrawText("Game", 100, 100, 100, WHITE);
        EndDrawing();
    }
    CloseWindow();
    return 0;
    
}
