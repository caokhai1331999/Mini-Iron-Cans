/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include "PlatformP.h"
// NOTE: One the way to make Platform in SIMD and Procedure styles.


bool IsArrow(SDL_Scancode KeyCode){
    if (KeyCode > (SDL_Scancode)78 && KeyCode < (SDL_Scancode)83){
        return true;
    }
    return false;
};

//Frees media and shuts down SDL
void close( Tile* tiles[]){
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
	// Platform.gDotTexture->free();
	Platform.gTileTexture->free();
    Platform.gTextTexture->free();
    Platform.gUserTankTexture->free();
    Platform.gEnemyTankTexture->free();
    Platform.gUserBulletTexture->free();
    Platform.gEnemyBulletTexture->free();

    
	//Destroy window	
	SDL_DestroyRenderer( Platform.gRenderer );
	SDL_DestroyWindow( Platform.gWindow );
	Platform.gWindow = NULL;
	Platform.gRenderer = NULL;

	//Quit SDL subsystems
    TTF_Quit();
	IMG_Quit();
	SDL_Quit();    
}

bool LoadMedia(Tile* tiles[]){
	//Loading success flag
	bool success = true;

	//Load dot texture
    // NOTE: The Bugs lied here
	if( !Platform.gUserTankTexture->loadFromFile( "media/myTank.png", Platform.gRenderer, (int)100, (int)100) )
	{
		printf( "Failed to load User tank texture!\n" );
		success = false;
	} // else {

      // Platform.gSpriteClips[ 0 ].x = 0;
      // Platform.gSpriteClips[ 0 ].y = 0;
      // Platform.gSpriteClips[ 0 ].w = 256;
      // Platform.gSpriteClips[ 0 ].h = 170;

      // Platform.gSpriteClips[ 1 ].x = 256;
      // Platform.gSpriteClips[ 1 ].y = 0;
      // Platform.gSpriteClips[ 1 ].w = 256;
      // Platform.gSpriteClips[ 1 ].h = 170;

      // Platform.gSpriteClips[ 2 ].x = 512;
      // Platform.gSpriteClips[ 2 ].y = 0;
      // Platform.gSpriteClips[ 2 ].w = 256;
      // Platform.gSpriteClips[ 2 ].h = 170;

      // Platform.gSpriteClips[ 3 ].x = 768;
      // Platform.gSpriteClips[ 3 ].y = 0;
      // Platform.gSpriteClips[ 3 ].w = 256;
      // Platform.gSpriteClips[ 3 ].h = 170;

//      gSpriteClips[ 4 ].x = 1000;
//      gSpriteClips[ 4 ].y = 0;
//      gSpriteClips[ 4 ].w = 250;
//      gSpriteClips[ 4 ].h = 170;
     
    // }

	if( !Platform.gEnemyTankTexture->loadFromFile( "media/enemyTank.png", Platform.gRenderer, (int)100, (int)100))
	{
		printf( "Failed to load Enemy tank texture!\n" );
		success = false;
	} // else {


	//Load tile texture
        if( !Platform.gTileTexture->loadFromFile( "tiles.png", Platform.gRenderer, (int)100, (int)100) )
	{
		printf( "Failed to load tile set texture!\n" );
		success = false;
	}

	//Load tile texture
	if( !Platform.gUserBulletTexture->loadFromFile( "media/myBullet.png", Platform.gRenderer, (int)20, (int)20) )
	{
		printf( "Failed to load user bullet set texture!\n" );
		success = false;
	}

	//Load tile texture
	if( !Platform.gEnemyBulletTexture->loadFromFile( "media/EnemyBullet.png", Platform.gRenderer, (int)20, (int)20) )
	{
		printf( "Failed to load enemy bullet set texture!\n" );
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

bool init()
{
	//Initialization flag
	bool success = true;

    Platform.gWindow = NULL;
    Platform.gRenderer = NULL;
    srand(time(nullptr));    
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
		Platform.gWindow = SDL_CreateWindow( "SDL Tank Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE );
		if( Platform.gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			Platform.gRenderer = SDL_CreateRenderer( Platform.gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( Platform.gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( Platform.gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}

                // Init SDL_ttf
#if defined(SDL_TTF_MAJOR_VERSION)
                if(TTF_Init() == -1 ){
                    printf( "Unable to Init TTF subsystem  ! SDL_ttf Error: %s\n", TTF_GetError());
                    success = false;
                };
#endif                                
			}
		}
	}
    
	return success;    
}


//Sets tiles from tile map
bool setTiles( Tile *tiles[]){

    // NOTE: Still don't understand this map creating methodology. Spare time to delve more.
    
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
			if( x >= LEVEL_WIDTH *2 )
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
            // NOTE: TILE_RED or TILE_GREEN is TileTypes
			Platform.gTileClips[ TILE_RED ].x = 0;
			Platform.gTileClips[ TILE_RED ].y = 0;
			Platform.gTileClips[ TILE_RED ].w = TILE_WIDTH;
			Platform.gTileClips[ TILE_RED ].h = TILE_HEIGHT;

			Platform.gTileClips[ TILE_GREEN ].x = 0;
			Platform.gTileClips[ TILE_GREEN ].y = 80;
			Platform.gTileClips[ TILE_GREEN ].w = TILE_WIDTH;
			Platform.gTileClips[ TILE_GREEN ].h = TILE_HEIGHT;

			Platform.gTileClips[ TILE_BLUE ].x = 0;
			Platform.gTileClips[ TILE_BLUE ].y = 160;
			Platform.gTileClips[ TILE_BLUE ].w = TILE_WIDTH;
			Platform.gTileClips[ TILE_BLUE ].h = TILE_HEIGHT;

			Platform.gTileClips[ TILE_TOPLEFT ].x = 80;
			Platform.gTileClips[ TILE_TOPLEFT ].y = 0;
			Platform.gTileClips[ TILE_TOPLEFT ].w = TILE_WIDTH;
			Platform.gTileClips[ TILE_TOPLEFT ].h = TILE_HEIGHT;

			Platform.gTileClips[ TILE_LEFT ].x = 80;
			Platform.gTileClips[ TILE_LEFT ].y = 80;
			Platform.gTileClips[ TILE_LEFT ].w = TILE_WIDTH;
			Platform.gTileClips[ TILE_LEFT ].h = TILE_HEIGHT;

			Platform.gTileClips[ TILE_BOTTOMLEFT ].x = 80;
			Platform.gTileClips[ TILE_BOTTOMLEFT ].y = 160;
			Platform.gTileClips[ TILE_BOTTOMLEFT ].w = TILE_WIDTH;
			Platform.gTileClips[ TILE_BOTTOMLEFT ].h = TILE_HEIGHT;

			Platform.gTileClips[ TILE_TOP ].x = 160;
			Platform.gTileClips[ TILE_TOP ].y = 0;
			Platform.gTileClips[ TILE_TOP ].w = TILE_WIDTH;
			Platform.gTileClips[ TILE_TOP ].h = TILE_HEIGHT;

			Platform.gTileClips[ TILE_CENTER ].x = 160;
			Platform.gTileClips[ TILE_CENTER ].y = 80;
			Platform.gTileClips[ TILE_CENTER ].w = TILE_WIDTH;
			Platform.gTileClips[ TILE_CENTER ].h = TILE_HEIGHT;

			Platform.gTileClips[ TILE_BOTTOM ].x = 160;
			Platform.gTileClips[ TILE_BOTTOM ].y = 160;
			Platform.gTileClips[ TILE_BOTTOM ].w = TILE_WIDTH;
			Platform.gTileClips[ TILE_BOTTOM ].h = TILE_HEIGHT;

			Platform.gTileClips[ TILE_TOPRIGHT ].x = 240;
			Platform.gTileClips[ TILE_TOPRIGHT ].y = 0;
			Platform.gTileClips[ TILE_TOPRIGHT ].w = TILE_WIDTH;
			Platform.gTileClips[ TILE_TOPRIGHT ].h = TILE_HEIGHT;

			Platform.gTileClips[ TILE_RIGHT ].x = 240;
			Platform.gTileClips[ TILE_RIGHT ].y = 80;
			Platform.gTileClips[ TILE_RIGHT ].w = TILE_WIDTH;
			Platform.gTileClips[ TILE_RIGHT ].h = TILE_HEIGHT;

			Platform.gTileClips[ TILE_BOTTOMRIGHT ].x = 240;
			Platform.gTileClips[ TILE_BOTTOMRIGHT ].y = 160;
			Platform.gTileClips[ TILE_BOTTOMRIGHT ].w = TILE_WIDTH;
			Platform.gTileClips[ TILE_BOTTOMRIGHT ].h = TILE_HEIGHT;
		}
	}

    //Close the file
    map.close();

    //If the map was loaded fine
    return tilesLoaded;    
}

// // NOTE: This function is wrong in somepart, But I don't have time to
// // figure it out. Let's it aside and move on, comeback when possible
// bool valid(SDL_Event e, KeyState* previousKey, KeyState* currentKey){
//     if (!IsArrow(e.key.keysym.scancode)||currentKey->init==0){
//         return true;
//     } else {
//         if (!IsArrow(previousKey->key)){
//             if (!IsArrow(currentKey->key)){
//                 return true;
//             } else {
//                 if (e.key.keysym.scancode == currentKey->key) {
//                     return true;
//                 } else {
//                     if (e.key.state == SDL_PRESSED && !currentKey->pressed){
//                         return true;
//                     } else {
//                         return false;
//                     }
//                 }
//             }
//         } else {
//             if (!IsArrow(currentKey->key)){
//                 if (e.key.keysym.scancode == previousKey->key) {
//                     return true;
//                 } else {
//                     if (e.key.state == SDL_PRESSED && !previousKey->pressed){
//                         return true;
//                     } else {
//                         return false;
//                     }
//                 }               
//             } else {
//                 if (previousKey->key == currentKey->key) {
//                     return true;
//                 } else {
//                     if (currentKey->pressed != (e.key.state == SDL_PRESSED) && currentKey->pressed != previousKey->pressed){
//                         return true;
//                     } else {
//                         return false;
//                     }
//                 }
//             }
//         }
//     }
// }

//Shows the Tank on the screen
void render(TankInfo* Tank, SDL_Rect& camera) {

    //NOTE: Show the tank and bullet here
    if (Tank->userBelong && Platform.gUserTankTexture!=NULL){
        Platform.gUserTankTexture->render( Platform.gRenderer ,(Tank->mBox.x - camera.x), (Tank->mBox.y - camera.y), NULL,Tank->face);

        // NOTE: Now the bullets
        // TODO: The bullet not show, Time to check this one
        if(Platform.gUserBulletTexture!=NULL) {             
        for (int i = 0; i < TOTAL_BULLET_PER_TANK; i++){
            if (Tank->Bullets[i].Launched){                
                Platform.gUserBulletTexture->render(Platform.gRenderer, (Tank->Bullets[i].blBox.x - camera.x), (Tank->Bullets[i].blBox.y - camera.y), NULL, Tank->face);
                // printf("Bullets %d image is being rendered\n", i);
            }
        }
    }

  }
else if (!Tank->userBelong && Platform.gEnemyTankTexture!=NULL) {
        // SOMEHOW the enemy tank positions changed to keep in bound while the text one is not
        // if (((Tank->mBox.x >= camera.x) && (Tank->mBox.x <= camera.x + camera.w)) &&((Tank->mBox.y >= camera.y) && (Tank->mBox.y <= camera.y + camera.w)) ){            
        Platform.gEnemyTankTexture->render( Platform.gRenderer ,Tank->mBox.x - camera.x,Tank->mBox.y - camera.y, NULL, Tank->face);
        if(Platform.gEnemyBulletTexture!=NULL) {             
        for (int i = 0; i < TOTAL_BULLET_PER_TANK; i++){
            if (Tank->Bullets[i].Launched){                
                Platform.gEnemyBulletTexture->render(Platform.gRenderer, (Tank->Bullets[i].blBox.x - camera.x), (Tank->Bullets[i].blBox.y - camera.y), NULL, Tank->face);
                printf("Enemy tanks and Bullets %d image is being rendered\n", i);
            }
        }
    }
        
        // }
    }
    
}

void renderText(uint32 StartTime, uint32 EndTime, const TankInfo* userTank){
    
                // NOTE: The reason that I can not use TTF_OpenFont or TTF_Init is
                // that I forgot to link ttf.lib in compile link
                // carefull to link, remember to put dll files in system32 folder
                
                char OutPut[256];
                gFont = TTF_OpenFont( "Roboto-Thin.ttf", 28 );
                // SDL_Color TextColor = {249 ,166 ,2};
                SDL_Color TextColor = {0 ,0 ,0};                
                EndTime = (uint32)SDL_GetTicks();
                real32 FPS = (real32)(1000.0f*(EndTime - StartTime));
                // NOTE: Somehow The exe file can't find out the TTF_OpenFont and TTF_Solid_Render which is in the ttf lib. Got to find out and fix
                
                sprintf(OutPut ,"FPS: %d \n",int(FPS));

                // printf(OutPut);
                
                if (!Platform.gTextTexture->loadFromRenderedText(OutPut, TextColor, gFont, Platform.gRenderer)) {
                            printf( "Can not Load Text to render! SDL Error: %s\n", SDL_GetError() );                            
                } else {                           Platform.gTextTexture->render(Platform.gRenderer, 0, 0);                    
                        }

                if(userTank->BulletsNumber == 1){
                    sprintf(OutPut, "Tank Bullets: Loading\n");
                    //Update screen
                    if (!Platform.gTextTexture->loadFromRenderedText(OutPut, TextColor, gFont, Platform.gRenderer)) {
                        printf( "Can not Load Text to render! SDL Error: %s\n", SDL_GetError() );                            
                    } else {                           Platform.gTextTexture->render(Platform.gRenderer, SCREEN_WIDTH - 300, 0);                    
                    }
                    
                    // printf(OutPut);
                } else {                    
                    sprintf(OutPut, "Tank Bullets :%d \n", int(userTank->BulletsNumber));
				//Update screen
                if (!Platform.gTextTexture->loadFromRenderedText(OutPut, TextColor, gFont, Platform.gRenderer)) {
                            printf( "Can not Load Text to render! SDL Error: %s\n", SDL_GetError() );                            
                } else {                           Platform.gTextTexture->render(Platform.gRenderer, SCREEN_WIDTH - 200, 0);                    
                }                    
                    // printf(OutPut);
                } 
}
