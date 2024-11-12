/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */

#include "BasicPlatform.h"



BasicPlatform::BasicPlatform(){

//The window we'll be rendering to
    gWindow = NULL;

//The window renderer
    gRenderer = NULL;

    gTileTexture = new LTexture;
    gDotTexture = new LTexture;
    gTextTexture = new LTexture;    
}

BasicPlatform::~BasicPlatform(){
    
}


bool BasicPlatform::setTiles( Tile* tiles[])
{
	//Success flag
	bool tilesLoaded = true;

    //The tile offsets
    int x = 0, y = 0;

    //Open the map
    std::ifstream map( "lazy.map" );

    //If the map couldn't be loaded
    if( map.fail() )
    {
		printf( "Unable to load map file!\n" );
		tilesLoaded = false;
    }
	else
	{
		//Initialize the tiles
		for( int i = 0; i < TOTAL_TILES; ++i )
		{
			//Determines what kind of tile will be made
			int tileType = -1;

			//Read tile from map file
			map >> tileType;

			//If the was a problem in reading the map
			if( map.fail() )
			{
				//Stop loading map
				printf( "Error loading map: Unexpected end of file!\n" );
				tilesLoaded = false;
				break;
			}

			//If the number is a valid tile number
			if( ( tileType >= 0 ) && ( tileType < TOTAL_TILE_SPRITES ) )
			{
				tiles[ i ] = new Tile( x, y, tileType );
			}
			//If we don't recognize the tile type
			else
			{
				//Stop loading map
				printf( "Error loading map: Invalid tile type at %d!\n", i );
				tilesLoaded = false;
				break;
			}

			//Move to next tile spot
			x += TILE_WIDTH;

			//If we've gone too far
			if( x >= LEVEL_WIDTH )
			{
				//Move back
				x = 0;

				//Move to the next row
				y += TILE_HEIGHT;
			}
		}
		
		//Clip the sprite sheet
		if( tilesLoaded )
		{
			gTileClips[ TILE_RED ].x = 0;
			gTileClips[ TILE_RED ].y = 0;
			gTileClips[ TILE_RED ].w = TILE_WIDTH;
			gTileClips[ TILE_RED ].h = TILE_HEIGHT;

			gTileClips[ TILE_GREEN ].x = 0;
			gTileClips[ TILE_GREEN ].y = 80;
			gTileClips[ TILE_GREEN ].w = TILE_WIDTH;
			gTileClips[ TILE_GREEN ].h = TILE_HEIGHT;

			gTileClips[ TILE_BLUE ].x = 0;
			gTileClips[ TILE_BLUE ].y = 160;
			gTileClips[ TILE_BLUE ].w = TILE_WIDTH;
			gTileClips[ TILE_BLUE ].h = TILE_HEIGHT;

			gTileClips[ TILE_TOPLEFT ].x = 80;
			gTileClips[ TILE_TOPLEFT ].y = 0;
			gTileClips[ TILE_TOPLEFT ].w = TILE_WIDTH;
			gTileClips[ TILE_TOPLEFT ].h = TILE_HEIGHT;

			gTileClips[ TILE_LEFT ].x = 80;
			gTileClips[ TILE_LEFT ].y = 80;
			gTileClips[ TILE_LEFT ].w = TILE_WIDTH;
			gTileClips[ TILE_LEFT ].h = TILE_HEIGHT;

			gTileClips[ TILE_BOTTOMLEFT ].x = 80;
			gTileClips[ TILE_BOTTOMLEFT ].y = 160;
			gTileClips[ TILE_BOTTOMLEFT ].w = TILE_WIDTH;
			gTileClips[ TILE_BOTTOMLEFT ].h = TILE_HEIGHT;

			gTileClips[ TILE_TOP ].x = 160;
			gTileClips[ TILE_TOP ].y = 0;
			gTileClips[ TILE_TOP ].w = TILE_WIDTH;
			gTileClips[ TILE_TOP ].h = TILE_HEIGHT;

			gTileClips[ TILE_CENTER ].x = 160;
			gTileClips[ TILE_CENTER ].y = 80;
			gTileClips[ TILE_CENTER ].w = TILE_WIDTH;
			gTileClips[ TILE_CENTER ].h = TILE_HEIGHT;

			gTileClips[ TILE_BOTTOM ].x = 160;
			gTileClips[ TILE_BOTTOM ].y = 160;
			gTileClips[ TILE_BOTTOM ].w = TILE_WIDTH;
			gTileClips[ TILE_BOTTOM ].h = TILE_HEIGHT;

			gTileClips[ TILE_TOPRIGHT ].x = 240;
			gTileClips[ TILE_TOPRIGHT ].y = 0;
			gTileClips[ TILE_TOPRIGHT ].w = TILE_WIDTH;
			gTileClips[ TILE_TOPRIGHT ].h = TILE_HEIGHT;

			gTileClips[ TILE_RIGHT ].x = 240;
			gTileClips[ TILE_RIGHT ].y = 80;
			gTileClips[ TILE_RIGHT ].w = TILE_WIDTH;
			gTileClips[ TILE_RIGHT ].h = TILE_HEIGHT;

			gTileClips[ TILE_BOTTOMRIGHT ].x = 240;
			gTileClips[ TILE_BOTTOMRIGHT ].y = 160;
			gTileClips[ TILE_BOTTOMRIGHT ].w = TILE_WIDTH;
			gTileClips[ TILE_BOTTOMRIGHT ].h = TILE_HEIGHT;
		}
	}

    //Close the file
    map.close();

    //If the map was loaded fine
    return tilesLoaded;
}

// NOTE: Somehow SDL block passing Window and Renderer pointer through function
// Got to delve more into this
void BasicPlatform::close( Tile* tiles[])
{
	//Deallocate tiles
	for( int i = 0; i < TOTAL_TILES; ++i )
	{
		 if( tiles[ i ] != NULL )
		 {
			delete tiles[ i ];
			tiles[ i ] = NULL;
		 }
	}

	//Free loaded images
	gDotTexture->free();
	gTileTexture->free();
	gTextTexture->free();

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
    TTF_CloseFont(gFont);
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

SDL_Renderer* BasicPlatform::GetRenderer(){
    return gRenderer;
}


SDL_Window* BasicPlatform::GetWindow(){
    return gWindow;
}

LTexture* BasicPlatform::GetgTileTexture(){
    return gTileTexture;   
}

LTexture* BasicPlatform::GetgDotTexture(){
    return gDotTexture;
}

LTexture* BasicPlatform::GetgTextTexture(){
    return gTextTexture;
}

SDL_Rect* BasicPlatform::GetgTileClips(){
    return gTileClips;
}
SDL_Rect* BasicPlatform::GetgSpriteClips(){
    return gSpriteClips;
}


bool BasicPlatform::LoadMedia(Tile* tiles[])
{
	//Loading success flag
	bool success = true;
    
	//Load dot texture
	if( !gDotTexture->loadFromFile( "move-to-left-256px-175px-per-frame.png", gRenderer) )
	{
		printf( "Failed to load rabit texture!\n" );
		success = false;
	} else {

      gSpriteClips[ 0 ].x = 0;
      gSpriteClips[ 0 ].y = 0;
      gSpriteClips[ 0 ].w = 256;
      gSpriteClips[ 0 ].h = 170;

      gSpriteClips[ 1 ].x = 256;
      gSpriteClips[ 1 ].y = 0;
      gSpriteClips[ 1 ].w = 256;
      gSpriteClips[ 1 ].h = 170;

      gSpriteClips[ 2 ].x = 512;
      gSpriteClips[ 2 ].y = 0;
      gSpriteClips[ 2 ].w = 256;
      gSpriteClips[ 2 ].h = 170;

      gSpriteClips[ 3 ].x = 768;
      gSpriteClips[ 3 ].y = 0;
      gSpriteClips[ 3 ].w = 256;
      gSpriteClips[ 3 ].h = 170;

//      gSpriteClips[ 4 ].x = 1000;
//      gSpriteClips[ 4 ].y = 0;
//      gSpriteClips[ 4 ].w = 250;
//      gSpriteClips[ 4 ].h = 170;
     
    }

	//Load tile texture
	if( !gTileTexture->loadFromFile( "39_tiling/tiles.png", gRenderer) )
	{
		printf( "Failed to load tile set texture!\n" );
		success = false;
	}

	//Load tile map
	if( !setTiles( tiles ) )
	{
		printf( "Failed to load tile set!\n" );
		success = false;
	}

	return success;
}

bool BasicPlatform::init()
{
	//Initialization flag
	bool success = true;
	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
            
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}

                // Init SDL_ttf
// #if defined(SDL_TTF_MAJOR_VERSION)
                if(TTF_Init() == -1){
                    printf( "Unable to Init TTF subsystem  ! SDL_ttf Error: %s\n", TTF_GetError());
                    success = false;
                };
// #endif                
			}
		}
	}

	return success;
}

