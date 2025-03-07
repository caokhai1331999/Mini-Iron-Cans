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

global_variable uint8_t* frame;
global_variable uint8_t Uframe = 0;
global_variable SDL_Rect camera;

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


struct Game{    
    STATE state = MENU_INIT;
    float startTime = 0.0f;
    int k = 0;    
    uint8_t pointed_option = 0;
    MENUCHOICE chosen_option = NONE;
    // NOTE: Manually set Init game stats here!!!

    Tile tileSet[TOTAL_TILES];   
    Position* TankPos = nullptr;
    TankInfo* userTank = nullptr;
    TankInfo* enemyTank = nullptr;
    
    // Game(){
    //     state = MENU_INIT;
    //     pointed_option = 0;
    //     chosen_option = NONE;

    //     tileSet = (Tile*)malloc(sizeof(*tileSet)*TOTAL_TILES);
       
    //     userTank = (TankInfo*)malloc(sizeof(*userTank));

    //     TankPos = (Position*)malloc(sizeof(*TankPos)*TOTAL_ENEMY_TANK);

    //     enemyTank = (TankInfo*)malloc(sizeof(*enemyTank)*TOTAL_ENEMY_TANK);
    // };
};

void displayMenu(PlatformP* p, Game* g);
void get_Menu_choice(Game* g, KeyState* key);
void changeState(Game* g);
bool Start(PlatformP* p, Game* g);
void ProcessInput(Game* g, bool* done);
void Update(Game* g);
void resetGame(Game* g);

// TODO: Complete this part
void runMainScene(Game* g);
void RenderMainScene(PlatformP* p, Game* g);
//===========================
void Render(PlatformP* p, Game* g);
void Close(PlatformP* p, Game* g);
#define GAME_H
#endif
