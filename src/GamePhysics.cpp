/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */

#include <GamePhysics.h>

bool checkCollision( SDL_Rect* a, SDL_Rect* b )
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a->x;
    rightA = a->x + a->w;
    topA = a->y;
    bottomA = a->y + a->h;

    //Calculate the sides of rect B
    leftB = b->x;
    rightB = b->x + b->w;
    topB = b->y;
    bottomB = b->y + b->h;

    //If any of the sides from A are outside of B
    if( bottomA <= topB )
    {
        return false;
    }

    if( topA >= bottomB )
    {
        return false;
    }

    if( rightA <= leftB )
    {
        return false;
    }

    if( leftA >= rightB )
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}

// TODO: 
// bool MassCheckCollision(SDL_Rect* BulletBox, TankInfo* EnemyTanks){
//     bool collided = false;
//     return collided;
// }

bool touchesWall( SDL_Rect* box, Tile* tiles)
{
    //Go through the tiles
    for( int i = 0; i < TOTAL_TILES; ++i )
    {
        //If the tile is a wall type tile
        if( ( tiles[ i ].mType >= SMALL_STUMP_TILE_1 ) && ( tiles[ i ].mType <= SMALL_STUMP_TILE_4 ) )
        {
            //If the collision box touches the wall tile
            if( checkCollision( box, &tiles[ i ].mBox) )
            {
                return true;
            }
        }
    }

    //If no wall tiles were touched
    return false;
}
