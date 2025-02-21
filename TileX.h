#if !defined(TILEX_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "LTexture.h"

struct Tile{
    //The attributes of the tile
    SDL_Rect mBox;

    //The tile type
    int mType;
    Tile( int x = 0, int y = 0, int tileType = 0 ){
        //Get the offsets
        mBox.x = x;
        mBox.y = y;

        //Set the collision box
        mBox.w = SMALL_TILE_WIDTH;
        mBox.h = SMALL_TILE_HEIGHT;

        //Get the tile type
        mType = tileType;        
    };
};

void renderTile( SDL_Rect& camera, SDL_Renderer* gRenderer, Tile tile, LTexture* gTileTexture, SDL_Rect* gTileClips = NULL, bool checkCollision = false);

#define TILEX_H
#endif
