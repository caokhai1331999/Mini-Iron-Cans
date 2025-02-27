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

    STATE state;
    float startTime = 0.0f;
    int k = 0;    
    uint8_t pointed_option;
    MENUCHOICE chosen_option;
    Tile* tileSet = NULL;
    
    Position* TankPos = NULL;    
    PlatformP* Platform = NULL;
    TankInfo* userTank = NULL;
    TankInfo* enemyTank = NULL;

    Game(){
        Platform = new PlatformP;

        tileSet = new Tile[TOTAL_TILES];
        
        userTank = new TankInfo(true);

        TankPos = new Position[TOTAL_ENEMY_TANK];

        enemyTank = new TankInfo[TOTAL_ENEMY_TANK];
        
        state = MENU_INIT;
        pointed_option = 0;
        chosen_option = NONE;
    };
};

void displayMenu(Game* g);
void get_Menu_choice(Game* g, KeyState* key);
void changeState(Game* g);
bool Start(Game* g);
void ProcessInput(Game* g, bool* done);
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
