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
#include "Tile.h"
#include "xTank.h"
// #include "Dot.h"

//Box collision detector
bool checkCollision( SDL_Rect* a, SDL_Rect* b );

//Mass Box collision detector
bool MassCheckCollision( TankInfo* UserTank, TankInfo* EnemyTanks);

//Checks collision box against set of tiles
bool touchesWall( SDL_Rect* box, Tile* tiles[] );

#define GAMEPHYSICS_H
#endif
