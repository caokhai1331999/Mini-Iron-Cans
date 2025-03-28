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
        // return false;
        return false;
    }

    if( topA >= bottomB )
    {
        // return false;
        return false;
    }

    if( rightA <= leftB )
    {
        // return false;
        return false;
    }

    if( leftA >= rightB )
    {
        // return false;
        return false;
    }
    //If none of the sides from A are outside B
    return true;
}

CollisionData checkCollisionS( SDL_Rect* a, SDL_Rect* b )
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;
    CollisionData* data = nullptr;
    data = new CollisionData();
    
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
        // return false;
        data->collided = false;
    } else {
        data->collided = true;
        data->TankAface = DOWN;
        data->TankBface = UP;
    }

    if( topA >= bottomB )
    {
        // return false;
        data->collided = false;
    } else {
        data->collided = true;
        data->TankAface = UP;
        data->TankBface = DOWN;
    }

    if( rightA <= leftB )
    {
        // return false;
        data->collided = false;
    } else {
        data->collided = true;
        data->TankAface = RIGHT;
        data->TankBface = LEFT;
    }

    if( leftA >= rightB )
    {
        // return false;
        data->collided = false;
    } else {
        data->collided = true;
        data->TankAface = LEFT;
        data->TankBface = RIGHT;
    }

    //If none of the sides from A are outside B
    // printf("Check face 3 inside:%d %d \n", (int)data->TankAface, 
           // (int)data->TankAface);
    data->collided = true;
    if(data->collided){
        data->TankAface = NOPE;
        data->TankBface = NOPE;
    };
    return *data;
    delete data;
    data = nullptr;
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
