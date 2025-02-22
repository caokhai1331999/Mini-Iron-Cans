/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include "TileX.h"

void renderTile( SDL_Rect& camera, SDL_Renderer* gRenderer, Tile tile, XTexture* gTileTexture, SDL_Rect* gTileClips, bool checkCollision)
{
    // May be I pass the wrong type of gTileCLips
    //If the tile is on screen
    // if(checkCollision )
    // {
        //Show the tile
    render(gRenderer, tile.mBox.x - camera.x, tile.mBox.y - camera.y, gTileTexture, &gTileClips[ tile.mType ]);
        // if (((mBox.x >= camera.x) && (mBox.x <= camera.x + camera.w)) &&((mBox.y >= camera.y) && (mBox.y <= camera.y + camera.w)) ){            
        // }
   // }
}
