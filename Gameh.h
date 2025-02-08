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
    global_variable real32 FPS = 0;

    const global_variable real32 StandardFPS = 60.0f;
    const global_variable uint32 FRAME_DELAY = 1000/60;

struct Game{    
    const char* Menu[] = {"NEW GAME", "RESUME", "OPTIONS", "EXIT"};
    KSTATE state;
    MENUCHOICE pointed_option;
    MENUCHOICE chosen_option;
    Tile* tileSet[ TOTAL_TILES ];
    Position* TankPos = nullptr;    
    PlatformP* Platform = nullptr;
    TankInfo* userTank = nullptr;
    TankInfo* enemyTank = nullptr;
    Game(){
        Platform = new PlatformP;
        userTank = new TankInfo(true);
        enemyTank = new TankInfo[TOTAL_ENEMY_TANK];
        state = MENU_IDLE;
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
void runMainScene(Game* g);
void Render(Game* g);
void Close(Game* g);

#define GAME_H
#endif
