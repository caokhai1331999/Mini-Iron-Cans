#if !defined(GAMEPHYSICS_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include <fstream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "TileX.h"
// #include "xTank.h"

#define FACE double
#define UP 0.0
#define DOWN 180.0
#define RIGHT 90.0
#define LEFT 270.0
#define NOPE -1.0

struct CollisionData{
    FACE TankAface = NOPE;
    FACE TankBface = NOPE;
    bool collided = false;
    CollisionData(){
        TankAface = NOPE;
        TankBface = NOPE;
        collided = false;
    };
};

bool checkCollision( SDL_Rect* a, SDL_Rect* b );

//Box collision detector
CollisionData checkCollisionS( SDL_Rect* a, SDL_Rect* b );

//Mass Box collision detector
// bool MassCheckCollision( TankInfo* UserTank, TankInfo* EnemyTanks);

//checks collision box against set of tiles
bool touchesWall( SDL_Rect* box, Tile* tiles );

#define GAMEPHYSICS_H
#endif
