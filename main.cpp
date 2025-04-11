#include "raylib.h"
#include <iostream>
#include <math.h>
#include "tries.h"
#include "words.h"
#include <string>
#include <ctime>
#include <cstdlib>
#define PI 3.14
using namespace std;
struct Asteroid{
    string word;
    Vector2 pos;
    float speed;
    bool isTargeted = false;
    Asteroid(string w, Vector2 p, float sp): word(w), pos(p), speed(sp) {}
};
typedef enum GameScreen {MENU = 0, GAMEPLAY, ENDING} GameScreen;

int main() {
    Trie trie;
    const int HEIGHT = 720;
    const int WIDTH = 480;
    InitWindow(WIDTH, HEIGHT, "Typing Game");
    const Texture2D shooter = LoadTexture("shooter.png");
    const Texture2D asteroidImg = LoadTexture("asteroid.png");
    Rectangle source = { 0, 0, (float)shooter.width, (float)shooter.height };
    Rectangle dest = { 240, 550, shooter.width * 0.5f, shooter.height * 0.5f };
    Vector2 origin = { dest.width / 2, dest.height / 2 };
    float rotation = 0;
    int score = 0;
    int index = 0;
    int wave;
    Vector2 dir = {0, 0};
    string gun = "";
    int asteroidCount = 0;
    float spawnTimer = 0;
    const float spawnInterval = 1.5;
    vector<Asteroid> asteroids;
    SetTargetFPS(60);
    GameScreen currentScreen = MENU;
    Rectangle textBox = {20, 620, 440, 30};
    srand(time(0));
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
        if(spawnTimer >= spawnInterval)
        {   
            spawnTimer = 0;
            index = rand() % 100;
            trie.insert(words[index]);
            asteroids.push_back(Asteroid(words[index], {GetRandomValue(30, 400), 100},(float)GetRandomValue(1, 2)));
        }
        int key = GetCharPressed();
        //check if more keys have been pressed in same frame
        if(IsKeyDown(KEY_BACKSPACE) && !gun.empty()){
            gun.pop_back();       
        }
        while(key > 0){
            if((key >= 32) && (key <= 125)){
                gun.append(1, (char)key);
            }
            key = GetCharPressed();
        }
        if(IsKeyPressed(KEY_ENTER)){
            if(trie.search(gun)){
                rotation = atan2f(dir.x, dir.y) * (180/PI);
                trie.remove(gun);
                for (auto it = asteroids.begin(); it != asteroids.end(); ++it) {
                    if (it->word == gun) {
                        asteroids.erase(it);
                        break;
                    }
                }
                gun = "";
            }    
        }
        if(trie.startsWith(gun)){
            for (auto it = asteroids.begin(); it != asteroids.end(); ++it) {
                if (it->word.rfind(gun) == 0) {
                    it->isTargeted = true;
                    break;
                }else{
                    it->isTargeted = false;
                }
            }
        }
        
        BeginDrawing();
        switch(currentScreen){
            case MENU:
            DrawRectangle(0, 0, WIDTH, HEIGHT, GRAY);
            DrawText( "Press Enter To Play", 25, 50, 20, BLACK);
            break;
            case GAMEPLAY:
            DrawRectangle(0, 0, WIDTH, HEIGHT, RAYWHITE);
            //DrawTextureEx(shooter, {175, 500}, rotation, 0.5f, WHITE);
            DrawTexturePro(shooter,source, dest, origin, rotation, WHITE);
            //DrawRectangle(20, 620, 440, 30, BLACK);
            DrawRectangleRec(textBox, LIGHTGRAY);
            DrawText(gun.c_str(), textBox.x, textBox.y, 25, RAYWHITE);
            DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, BLACK);
            //DrawText();
            for(auto &asteroid: asteroids){
                dir = {175 - asteroid.pos.x, 600 - asteroid.pos.y};
                float length = sqrt(dir.x * dir.x + dir.y * dir.y);
                if (length != 0) {
                    dir.x /= length;
                    dir.y /= length;
                }
                DrawTextureEx(asteroidImg, {asteroid.pos.x - 3, asteroid.pos.y + 5}, 0, 0.2, WHITE);
                if(asteroid.isTargeted == true){
                    DrawText(asteroid.word.c_str(), asteroid.pos.x, asteroid.pos.y, 25, YELLOW);
                }else{
                    DrawText(asteroid.word.c_str(), asteroid.pos.x, asteroid.pos.y, 25, RED);
                }
                asteroid.pos.x += dir.x * asteroid.speed;
                asteroid.pos.y += dir.y * asteroid.speed;
            }
            break;
            case ENDING:
            break;         
            default: break;
        }
        EndDrawing();
        spawnTimer += GetFrameTime();
    }
    UnloadTexture(shooter);
    CloseWindow();
    return 0;
    
}
