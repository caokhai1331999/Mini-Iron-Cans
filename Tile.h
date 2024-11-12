#ifndef TILE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "LTexture.h"

//The tile
class Tile
{
    public:
		//Initializes position and type
		Tile( int x, int y, int tileType );

		//Shows the tile
    void render( SDL_Rect& camera, SDL_Renderer* gRenderer, LTexture* gTileTexture, SDL_Rect* gTileClips = NULL, bool checkCollision = false);

    
		//Get the tile type
		int getType();

		//Get the collision box
		SDL_Rect* getBox();

    private:
		//The attributes of the tile
		SDL_Rect mBox;

		//The tile type
		int mType;
};

#define TILE_H
#endif
