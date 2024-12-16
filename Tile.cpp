#include "Tile.h"

Tile::Tile( int x, int y, int tileType )
{
    //Get the offsets
    mBox.x = x;
    mBox.y = y;

    //Set the collision box
    mBox.w = TILE_WIDTH;
    mBox.h = TILE_HEIGHT;

    //Get the tile type
    mType = tileType;
}

void Tile::render( SDL_Rect& camera, SDL_Renderer* gRenderer, LTexture* gTileTexture, SDL_Rect* gTileClips, bool checkCollision)
{
    // May be I pass the wrong type of gTileCLips
    //If the tile is on screen
    // if(checkCollision )
    // {
        //Show the tile
        gTileTexture->render(gRenderer, mBox.x - camera.x, mBox.y - camera.y, &gTileClips[ mType ] );
        // if (((mBox.x >= camera.x) && (mBox.x <= camera.x + camera.w)) &&((mBox.y >= camera.y) && (mBox.y <= camera.y + camera.w)) ){            
        // }
   // }
}

int Tile::getType()
{
    return mType;
}

SDL_Rect* Tile::getBox()
{
    return &mBox;
}
