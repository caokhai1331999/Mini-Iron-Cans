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
#include "Tile.h"
#include "xTank.h"
#include "GamePhysics.h"
#include "PlatformP.h"
#include "KState.h"

    global_variable real32 StartTime;
    global_variable real32 EndTime;
    global_variable real32 TimeElapsed = 0;

    global_variable real32 respawnStartTime = 0;
    global_variable real32 respawnEndTime = 0;
    global_variable real32 respawnTime = 0.0f;

    global_variable uint8_t FPS = 0;

    global_variable int frame[4] = {};
    global_variable uint8_t Uframe = 0;
    const global_variable real32 StandardFPS = 60.0f;
    const global_variable uint32 FRAME_DELAY = 1000/60;

    global_variable bool Ucollided = false;
    global_variable bool Ecollided = false;

struct Game{    
    const char* Menu[4] = {"NEW GAME", "RESUME", "OPTIONS", "EXIT"};
    STATE state;
    float startTime = 0.0f;
    int k = 0;    
    MENUCHOICE pointed_option;
    MENUCHOICE chosen_option;
    Tile* tileSet[ TOTAL_TILES ];
    SDL_Rect camera;
    
    Position* TankPos = nullptr;    
    PlatformP* Platform = nullptr;
    TankInfo* userTank = nullptr;
    TankInfo* enemyTank = nullptr;
    Game(){
        Platform = new PlatformP;
        userTank = new TankInfo(true);
        enemyTank = new TankInfo[TOTAL_ENEMY_TANK];
        state = MENU_INIT;
        pointed_option = NEW_GAME;
        chosen_option = NONE;
    };
};

void displayMenu(Game* g);
void get_Menu_choice(Game* g, KeyState* key);
void changeState(Game* g);
bool Start(Game* g);
void ProcessInput(Game* g, bool done);
void Update(Game* g);
void resetGame(Game* g);

// TODO: Complete this part
void runMainScene(Game* g);
void RenderMainScene(Game* g);
//===========================
void Render(Game* g);
void Close(Game* g);

#define GAME_H
#endif
