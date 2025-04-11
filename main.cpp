#include "raylib.h"
#include <iostream>
#include "tries.h"
#include "words.h"
#include <string>
#include <ctime>
#include <cstdlib>

using namespace std;
struct Asteroid{
    string word;
    Vector2 pos;
    float speed;
    bool isTargeted = false;
    Asteroid(string w, Vector p, float sp): word(w), pos(p), speed(sp) {}
};
typedef enum GameScreen {MENU = 0, GAMEPLAY, ENDING} GameScreen;
int main() {
    Trie trie;
    const int HEIGHT = 720;
    const int WIDTH = 480;
    InitWindow(WIDTH, HEIGHT, "Typing Game");
    const Texture2D shooter = LoadTexture("shooter.png");
    string currentInput = "";
    float rotation = 0;
    int score = 0;
    int index = 0;
    int wave;
    int asteroidCount = 0;
    int generateTimer = 0;
    int currentTime = 0;
    vector<Asteroid> asteroids;
    SetTargetFPS(60);
    GameScreen currentScreen = MENU;
    srand(time());
    while(!WindowShouldClose()){
        switch(currentScreen){
            case MENU:
            if(IsKeyPressed(KEY_ENTER)){
                currentScreen = GAMEPLAY;
            }
            break;
            case GAMEPLAY:

            break;
            case ENDING:

            break;
            default: break;
        }
        index = rand() % 100;
        trie.insert(words[index]);
        asteroids.push_back(new Asteroid(words[index], {GetRandomValue(30, 400), 100}, GetRandomValue(3, 10)));
        BeginDrawing();
        switch(currentScreen){
            case MENU:
            DrawRectangle(0, 0, WIDTH, HEIGHT, GRAY);
            DrawText( "Press Enter To Play", 25, 50, 20, BLACK);
            break;
            case GAMEPLAY:
            DrawRectangle(0, 0, WIDTH, HEIGHT, RAYWHITE);

            DrawTextureEx(shooter, {175, 500}, rotation, 0.5f, WHITE);
            break;
            case ENDING:
            break;         
            default: break;
        }
        EndDrawing();
    }
    UnloadTexture(shooter);
    CloseWindow();
    return 0;
    
}
