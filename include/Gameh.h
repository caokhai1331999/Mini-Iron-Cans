#if !defined(GAMEH_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */

//Using SDL, SDL_image, standard IO, strings, and file streams
#include <string>
#include <cstring>
#include "SDL2/SDL_timer.h"
#include "GamePhysics.h"
#include "PlatformP.h"
#include "xTank.h"

global_variable real32 StartTime = 0.0f;
global_variable real32 EndTime = 0.0f;
global_variable real32 TimeElapsed = 0.0f;

global_variable uint8_t FPS = 0;

global_variable uint8_t ExplosionFrame[5] = {};
global_variable uint8_t MovingFrame[5] = {};
// NOTE: May be before I set the camera to {} with reset w,h to
// 0 everytime I reload code
global_variable char* Menu[4] = {"NEW GAME", "RESUME", "OPTIONS", "EXIT"};
const global_variable real32 StandardFPS = 60.0f;
const global_variable uint32 FRAME_DELAY = 1000/60;

global_variable bool Ucollided = false;
global_variable bool Ecollided = false;


enum STATE{
    // NOTE: How to tell the new game state apart from the current one
    EMPTY, MENU_INIT, GAME_NEW, GAME_RELOADED, PAUSE
};

enum MENUCHOICE{
    NONE = static_cast<uint8_t>(0), NEW_GAME, RESUME, OPTIONS, EXIT
};

#define CHANGED true
#define NOT_YET false


struct Game{
    bool stateChange = NOT_YET;
    STATE state = MENU_INIT;
    int k = 0;    
    uint8_t pointed_option = 0;
    MENUCHOICE chosen_option = NONE;
    // NOTE: Manually set Init game stats here!!!
    SDL_Rect Camera = {0, 0, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT};
    PlatformP* platform;
    
    Tile* tileSet;   
    Position* TankPos;
    TankInfo* userTank;
    TankInfo* enemyTank;

    Particle* particles;
    
    Game(){
        printf("Camera w and h in struct is %d  %d\n", Camera.w, Camera.h);
        state = MENU_INIT;
        pointed_option = 0;
        chosen_option = NONE;
        stateChange = NOT_YET;

        platform = nullptr;
        platform = new PlatformP;
        
        tileSet = nullptr;
        tileSet = new Tile[TOTAL_TILES]();
       
        TankPos = nullptr;
        TankPos = new Position[TOTAL_ENEMY_TANK]();

        userTank = nullptr;
        userTank = new TankInfo(0 , 0, true);

        enemyTank = nullptr;
        enemyTank = new TankInfo[TOTAL_ENEMY_TANK]();

        particles = nullptr;
        particles = new Particle[TOTAL_PARTICLES]();
    };
};

// void resize(int* w, int* h, PlatformP* p);
void displayMenu(Game* g);
void get_Menu_choice(Game* g, KeyState* key);
void changeState(Game* g);
bool Start(Game* g);
void ProcessInput(Game* g, bool* done);
void Update(Game* g);
void resetGame(Game* g);

void runMainScene(Game* g);
void RenderMainScene(Game* g);
//===========================
void Render(Game* g);
void Close(Game* g);
#define GAME_H
#endif
