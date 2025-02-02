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

struct Game{
    global_variable real32 StartTime;
    global_variable real32 EndTime;
    global_variable real32 TimeElapsed = 0;
    global_variable real32 respawnStartTime = 0;
    global_variable real32 respawnEndTime = 0;
    global_variable real32 FPS = 0;

    const global_variable real32 StandardFPS = 60.0f;
    const global_variable uint32 FRAME_DELAY = 1000/60;
    PlatformP* Platform = nullptr;

    userTank = new TankInfo(true);
    enemyTank = new TankInfo[TOTAL_ENEMY_TANK];            
            
    Position* TankPos = nullptr;    
    
    Game(){
        Platform = new PlatformP;
    };
};

bool Start(Game* g);
void ProcessInput(Game* g);
void Update(Game* g);
void Render(Game* g);

#define GAME_H
#endif
