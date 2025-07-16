#include "raylib.h"
#include <iostream>
#include <math.h>
#include "tries.h"
#include "words.h"
#include "skew.h"
#include <string>
#include <ctime>
#include <cstdlib>
#include <fstream>

#define PI 3.14
#define SPACE (Color){ 10, 10, 30, 255 } 
using namespace std;

struct Asteroid{
        string word;
        Vector2 pos;
        float speed;
        vector<int> score;
        bool isTargeted = false;
        Asteroid(string w, Vector2 p, float sp): word(w), pos(p), speed(sp) {}
    };
typedef struct {
        Vector2 position;
        float radius;
        Color color;
    } Star;
    typedef enum GameScreen {MENU = 0, GAMEPLAY,SCORE,WAVE, ENDING, } GameScreen;
    void appendToFile(int score){
        std::ofstream outFile("scores.txt", ios::app);
        if(outFile.is_open()){
            outFile << score << "\n";
            outFile.close();
        }
    }
    vector<int> readFromFile(){
        vector<int> scores;
        ifstream inFile("scores.txt");
        int score;
        if(inFile.is_open()){
            while(inFile >> score){
                scores.push_back(score);
            }
            inFile.close();
        }
        return scores;

    }
    struct Explosion {
        Vector2 pos;
        float timer;
        Explosion(Vector2 p) : pos(p), timer(0.0f) {}
    };
    vector<Explosion> explosions;
    struct Shoot{
        Vector2 pos;
        Vector2 dir;
        Vector2 astPos;
        bool destroy;
        Shoot(Vector2 p, Vector2 d, Vector2 ap): pos(p), dir(d), destroy(true), astPos(ap){}; 
    };
    vector<Shoot> bullets;
    
    int main() {
        Trie trie;
        InitAudioDevice();
        Sound explosionSound = LoadSound("soundfx.wav");
        Music bgMusic = LoadMusicStream("bg.mp3");
        PlayMusicStream(bgMusic);
        const int HEIGHT = 720;
        const int WIDTH = 480;
        InitWindow(WIDTH, HEIGHT, "Typing Game");
        const Texture2D shooter = LoadTexture("shooter.png");
        const Texture2D asteroidImg = LoadTexture("asteroid.png");
        bool addScore = false;
        Rectangle source = { 0, 0, (float)shooter.width, (float)shooter.height };
        Rectangle dest = { 240, 550, shooter.width * 0.5f, shooter.height * 0.5f };
        Vector2 origin = { dest.width / 2, dest.height / 2 };
        float rotation = 0;
        int score = 0;
        int key;
        float waveTimer = 0.0f;
        int index = 0;
        float transparency = 1.0f;
        int wave = 1;
        string disp = "Score: "; 
        Vector2 dir = {0, 0};
        string gun = "";
        float spawnTimer = 0;
        float spawnInterval = 1.75f;
        vector<Asteroid> asteroids;
        vector<int> scores;
        int y_axis;
        Skew heap;
        int size;
        float maxSpeed = 1.2;
        float collision_timer = 0.0f;
        Vector2 mousePos = GetMousePosition();
        int waveLimit = 30;
        bool dispWave = false;
        SetTargetFPS(60);
        GameScreen currentScreen = MENU;
        Rectangle textBox = {20, 620, 440, 30};
        Rectangle btnStart = { 95, 160, 300, 60 };
        Rectangle btnScore = { 95, 240, 300, 60 };
        Rectangle btnExit  = { 95, 320, 300, 60 };
        Rectangle btnBackToMenu = { 145, 420, 210, 45  };
        Star stars[60];
        for (int i = 0; i < 60; i++) {
            stars[i].position = { (float)GetRandomValue(0, WIDTH), (float)GetRandomValue(0, HEIGHT) };
            stars[i].radius = (float)GetRandomValue(1, 3); 
            stars[i].color = (Color){ 200 + GetRandomValue(-30, 30), 200 + GetRandomValue(-30, 30), 200 + GetRandomValue(-30, 30), 255 };
        }
        srand(time(0));
        while(!WindowShouldClose()){
            UpdateMusicStream(bgMusic);
            mousePos = GetMousePosition();
            switch(currentScreen){
                case MENU:

                break;
                case GAMEPLAY:
                if(spawnTimer >= spawnInterval)
                {  

                spawnTimer = 0;
                index = rand() % 100;
                while(trie.search(words[index])){
                    index = rand()%100;
                }
                trie.insert(words[index]);
                asteroids.push_back(Asteroid(words[index], {GetRandomValue(30, 400), 100},(float)GetRandomValue(1, maxSpeed)));
            }
            key = GetCharPressed();
            //check if more keys have been pressed in same frame
            if(IsKeyPressed(KEY_BACKSPACE) && !gun.empty()){
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
                    score += 10;
                    for (auto it = asteroids.begin(); it != asteroids.end(); ++it) {
                        if (it->word == gun) {
                            PlaySound(explosionSound);
                            explosions.push_back(Explosion(it->pos));
                            asteroids.erase(it);
                            dir = {it->pos.x - 240, it->pos.y - 550};
                            float len = sqrt(pow(dir.x, 2) +  pow(dir.y, 2));
                            dir.x = dir.x / len;
                            dir.y = dir.y / len;
                            dir.x *= 10;
                            dir.y *= 10;
                            bullets.push_back(Shoot({240, 550},dir, it->pos));
                            break;
                        }
                    }
                    //gun = "";
                }    
                gun = "";
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
            if(score >= waveLimit){
                wave++;
                maxSpeed+= 0.3f;
                if(spawnInterval > 0.5f) spawnInterval =  spawnInterval - 0.1f;
                waveLimit = waveLimit + score;
                dispWave = true;
            }
            if(dispWave){
                transparency -= 0.02f;
            }
            if(waveTimer <= 2.5f){
                waveTimer += GetFrameTime();
            }
            if(waveTimer > 2.5f){
                dispWave = false;
                waveTimer = 0;
                transparency = 1.0f;
            }
            break;
                case WAVE:
                
                break;
                case SCORE:
                if(CheckCollisionPointRec(mousePos, {20, y_axis, 440, 60}) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                    currentScreen = MENU;
                }
                break;
                case ENDING:

                break;
                default: break;
            }
            
            BeginDrawing();
            switch(currentScreen){
                case MENU:
                spawnTimer = 0;
                DrawRectangleGradientV(0, 0, WIDTH, HEIGHT, DARKPURPLE, PURPLE);
                for(int i = 0; i < 60; i++){
                    DrawCircleV(stars[i].position, stars[i].radius, stars[i].color);
                }
                DrawText("Main Menu", 115, 45, 50, BLACK);
                DrawRectangleRoundedLinesEx(btnStart,15,10,5, DARKPURPLE);
                DrawText("Start Game", 150, 175,35, WHITE);
                DrawRectangleRoundedLinesEx(btnScore,15,10,5, DARKPURPLE);
                DrawText("Score", 190, 260,35, WHITE);
                DrawRectangleRoundedLinesEx(btnExit,15,10,5, DARKPURPLE);
                DrawText("Exit", 200, 335,35, RED);
                if(CheckCollisionPointRec(mousePos, btnStart) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                    currentScreen = GAMEPLAY;
                }
                
                if(CheckCollisionPointRec(mousePos, btnScore) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                    currentScreen = SCORE;
                }

                if(CheckCollisionPointRec(mousePos, btnExit) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                    CloseWindow();
                }
                //DrawText( "Press Enter To Play", 25, 50, 20, BLACK);
                break;

                case GAMEPLAY:
                //DrawRectangle(0, 0, WIDTH, HEIGHT, BLACK);
                ClearBackground(SPACE);
                for(int i = 0; i < 60; i++){
                    DrawCircleV(stars[i].position, stars[i].radius, stars[i].color);
                }
                DrawText(disp.c_str(),5, 20, 15, WHITE);
                DrawText(to_string(score).c_str(), 55, 20, 20, WHITE);
                DrawText("Wave: ", 400, 20, 15, WHITE);
                DrawText(to_string(wave).c_str(), 455, 20, 20, WHITE);
                //DrawTextureEx(shooter, {175, 500}, rotation, 0.5f, WHITE);
                DrawTexturePro(shooter,source, dest, origin, rotation, WHITE);
                //DrawRectangle(20, 620, 440, 30, BLACK);
                DrawRectangleRec(textBox, LIGHTGRAY);
                DrawText(gun.c_str(), textBox.x + 10, textBox.y, 25, BLACK);
                DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, BLACK);
                //DrawText();
                for(auto &asteroid: asteroids){
                    dir = {240 - asteroid.pos.x, 550 - asteroid.pos.y};
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
                    Rectangle astPos = {asteroid.pos.x, asteroid.pos.y, asteroidImg.width * 0.2f, asteroidImg.height * 0.2f}; 
                  
                    bool collision = CheckCollisionRecs(dest, astPos);
                    if (collision) {
                        addScore = true;
                        explosions.push_back(asteroid.pos);
                        collision_timer += GetFrameTime();
                        DrawText("Asteroid Collided! ", 10, 120,  50, RED);
                        SetTargetFPS(20);  // Slow motion effect
                    }                 
                }
                for(auto it = bullets.begin(); it != bullets.end(); ++it){
                    if(it->destroy){
                        it->pos.x += it->dir.x;
                        it->pos.y += it->dir.y;
                        DrawCircle(it->pos.x,it->pos.y, 5, RED);
                        if(CheckCollisionPointCircle({it->astPos.x, it->astPos.y}, it->pos, 20)){
                            it->destroy = false;
                        }
                    }
                }
                for (int i = 0; i < explosions.size();i++) {
                    Explosion &exp = explosions[i];
                    for(int j = 1; j <= 5; j++){
                        float alpha = 1.0f - (exp.timer / 1.0f);
                        if(collision_timer != 0){
                            DrawCircleV(exp.pos, j * 15, Fade(RED, alpha));
                        }else DrawCircleV(exp.pos, j * 10, Fade(ORANGE, alpha));
                    }
                    exp.timer += GetFrameTime();
                
                    if(collision_timer >= 0.5f){
                        asteroids.clear();
                        wave = 0;
                        spawnTimer = 0;
                        wave = 1;
                        collision_timer = 0;
                        maxSpeed = 1.2;
                        currentScreen = ENDING;

                    }
                }
                if(dispWave){
                    DrawText("Wave: ", WIDTH/2 - 90, HEIGHT/2 - 80, 50, Fade(WHITE, transparency));
                    DrawText(to_string(wave).c_str(), WIDTH/2 + 40, HEIGHT/2 - 80, 50, Fade(WHITE, transparency));
                }
                break;

                case SCORE:
                    DrawRectangleGradientV(0, 0, WIDTH, HEIGHT, DARKBLUE, SKYBLUE);
                    scores = readFromFile();
                    size = scores.size();
                    for(int i = 0; i < size; i++){
                        heap.insert(scores[i]);
                    }
                    DrawText("Score List(5)", 110, 30, 45, WHITE);
                    y_axis = 100;
                    for(int i = 0; i < size; i++){
                        if(i >= 5) break;
                        DrawRectangleRounded({20, y_axis, 440, 60},15, 10, RAYWHITE);
                        DrawText(to_string(heap.findMin()).c_str(), 200, y_axis + 15, 38, BLACK);
                        heap.deleteMin();
                        y_axis += 75;
                    }
                    DrawRectangleRoundedLinesEx({20, y_axis, 440, 60},15, 10, 5, RAYWHITE);
                    DrawText("Back To Menu <-", 120, y_axis + 15, 35, RAYWHITE);
                break;
                case WAVE:
                DrawRectangle(0, 0, WIDTH, HEIGHT, LIGHTGRAY);
                DrawText("Wave: ", 200 ,300, 25, BLACK);
                DrawText(to_string(wave).c_str(), 265 ,300, 25, BLACK);
                break;
                case ENDING:
                    SetTargetFPS(60);
                    if(addScore){
                        appendToFile(score);
                       addScore = false;
                   }
                    DrawRectangle(0, 0, WIDTH, HEIGHT, DARKPURPLE);
                    DrawText("Thanks For Playing", 25, 275, 40, WHITE);
                    DrawText("Your Score Is ", 80, 350, 35, WHITE);
                    DrawText(to_string(score).c_str(), 350, 350, 35, WHITE);
                    DrawRectangleRounded(btnBackToMenu,15, 5, WHITE);
                    DrawText("Back to Menu", btnBackToMenu.x + 20, btnBackToMenu.y + 10, 25, BLACK);
                    if(CheckCollisionPointRec(mousePos, btnBackToMenu) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                        score = 0;
                        currentScreen = MENU;
                    }

                break;         
                default: 
                
                break;
            }
            EndDrawing();
            spawnTimer += GetFrameTime();
        }
        UnloadSound(explosionSound);
        UnloadMusicStream(bgMusic);
        CloseAudioDevice();
        UnloadTexture(shooter);
        CloseWindow();
        return 0;
        
    }
