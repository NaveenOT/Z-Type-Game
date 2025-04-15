    #include "raylib.h"
    #include <iostream>
    #include <math.h>
    #include "tries.h"
    #include "words.h"
    #include <string>
    #include <ctime>
    #include <cstdlib>
    #include <fstream>
    #define PI 3.14
    using namespace std;
    struct Asteroid{
        string word;
        Vector2 pos;
        float speed;
        vector<int> score;
        bool isTargeted = false;
        Asteroid(string w, Vector2 p, float sp): word(w), pos(p), speed(sp) {}
    };
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


    int main() {
        Trie trie;
        const int HEIGHT = 720;
        const int WIDTH = 480;
        InitWindow(WIDTH, HEIGHT, "Typing Game");
        const Texture2D shooter = LoadTexture("shooter.png");
        const Texture2D asteroidImg = LoadTexture("asteroid.png");
        bool addScore = false;
        Rectangle source = { 0, 0, (float)shooter.width, (float)shooter.height };
        Rectangle dest = { 240, 550, shooter.width * 0.5f, shooter.height * 0.5f };
        Vector2 origin = { dest.width / 2, dest.height / 2 };
        Color fadeColor;
        float rotation = 0;
        int score = 0;
        int key;
        int index = 0;
        float transparency = 0.0f;
        int wave = 1;
        string disp = "Score: "; 
        Vector2 dir = {0, 0};
        string gun = "";
        int asteroidCount = 0;
        float spawnTimer = 0;
        float spawnInterval = 1.75f;
        vector<Asteroid> asteroids;
        vector<int> scores;
        int y_axis;
        float maxSpeed = 1.2;
        float collision_timer = 0.0f;
        Vector2 mousePos = GetMousePosition();
        int waveLimit = 30;
        SetTargetFPS(60);
        GameScreen currentScreen = MENU;
        Rectangle textBox = {20, 620, 440, 30};
        Rectangle btnStart = { 150, 140, 200, 40 };
        Rectangle btnScore = { 150, 200, 200, 40 };
        Rectangle btnExit  = { 150, 280, 200, 40 };
        Rectangle btnBackToMenu = { 150, 400, 200, 40  };
        srand(time(0));
        while(!WindowShouldClose()){
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
                            explosions.push_back(Explosion(it->pos));
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
            if(score >= waveLimit){
                wave++;
                maxSpeed+= 0.3f;
                if(spawnInterval > 0.5f) spawnInterval =  spawnInterval - 0.1f;
                waveLimit = waveLimit + score;
                currentScreen = WAVE;

            }
                case WAVE:
                if(spawnTimer >= 2.05f){
                    currentScreen = GAMEPLAY;
                }
                break;
                break;
                case ENDING:

                break;
                default: break;
            }
            
            BeginDrawing();
            switch(currentScreen){
                case MENU:
                spawnTimer = 0;
                DrawRectangle(0, 0, WIDTH, HEIGHT, GRAY);
                DrawText("Main Menu", 320, 40, 30, RED);
                DrawRectangleRec(btnStart, LIGHTGRAY);
                DrawText("Start Game", 160, 145,25, BLACK);
                DrawRectangleRec(btnScore, LIGHTGRAY);
                DrawText("Score", 160, 205,25, BLACK);
                DrawRectangleRec(btnExit, LIGHTGRAY);
                DrawText("Exit", 160, 285,25, RED);
                if(CheckCollisionPointRec(mousePos, btnStart) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                    currentScreen = GAMEPLAY;
                }
                
                if(CheckCollisionPointRec(mousePos, btnScore) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                    currentScreen = SCORE;
                }

                if(CheckCollisionPointRec(mousePos, btnExit) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                    currentScreen = ENDING;
                }
                //DrawText( "Press Enter To Play", 25, 50, 20, BLACK);
                break;

                case GAMEPLAY:
                DrawRectangle(0, 0, WIDTH, HEIGHT, RAYWHITE);
                
                DrawText(disp.c_str(),5, 20, 15, BLACK);
                DrawText(to_string(score).c_str(), 55, 20, 15, BLACK);
                DrawText("Wave: ", 400, 20, 15, BLACK);
                DrawText(to_string(wave).c_str(), 455, 20, 15, BLACK);
                //DrawTextureEx(shooter, {175, 500}, rotation, 0.5f, WHITE);
                DrawTexturePro(shooter,source, dest, origin, rotation, WHITE);
                //DrawRectangle(20, 620, 440, 30, BLACK);
                DrawRectangleRec(textBox, LIGHTGRAY);
                DrawText(gun.c_str(), textBox.x, textBox.y, 25, RAYWHITE);
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
                        explosions.push_back(asteroid.pos);
                        collision_timer += GetFrameTime();
                        DrawText("Collision Detected! ", 50, 120,  50, RED);
                        SetTargetFPS(20);  // Slow motion effect
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
                        currentScreen = ENDING;
                    }
                }
                
                break;

                case SCORE:
                    scores = readFromFile();
                    DrawRectangle(0, 0, WIDTH, HEIGHT, BLACK);
                    DrawText("Score List", 120, 30, 45, WHITE);
                    y_axis = 100;
                    for(int i = 0; i < scores.size(); i++){
                        DrawRectangle(20, y_axis, 440, 60, RAYWHITE);
                        DrawText(to_string(scores[i]).c_str(), 200, y_axis + 15, 38, BLACK);
                        y_axis += 75;
                    }
                break;
                case WAVE:
                DrawRectangle(0, 0, WIDTH, HEIGHT, LIGHTGRAY);
                DrawText("Wave: ", 200 ,300, 25, BLACK);
                DrawText(to_string(wave).c_str(), 265 ,300, 25, BLACK);
                break;
                case ENDING:
                    SetTargetFPS(60);
                    DrawRectangle(0, 0, WIDTH, HEIGHT, BLACK);
                    DrawText("Thanks For Playing", 20, 300, 40, WHITE);
                    DrawText("Your Score Is ", 80, 350, 35, WHITE);
                    DrawText(to_string(score).c_str(), 350, 350, 35, WHITE);
                    DrawRectangleRec(btnBackToMenu, LIGHTGRAY);
                    if(!addScore){
                        appendToFile(score);
                        addScore = true;
                    }
                    if(CheckCollisionPointRec(mousePos, btnBackToMenu) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                        currentScreen = MENU;
                    }

                break;         
                default: 
                
                break;
            }
            EndDrawing(); 
            spawnTimer += GetFrameTime();
        }
        UnloadTexture(shooter);
        CloseWindow();
        return 0;
        
    }
