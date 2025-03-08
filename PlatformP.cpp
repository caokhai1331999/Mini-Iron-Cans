/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include <PlatformP.h>
// NOTE: One the way to make Platform in SIMD and Procedure styles.


bool IsArrow(SDL_Scancode KeyCode){
    if (KeyCode > (SDL_Scancode)78 && KeyCode < (SDL_Scancode)83){
        return true;
    }
    return false;
};

//Frees media and shuts down SDL
void close( PlatformP* Platform){
	//Free loaded images
        TTF_CloseFont( Platform->gFont );
        Platform->gFont = NULL;
        
        xfree(Platform->gTileTexture);
        Platform->gTileTexture->mTexture = nullptr;
        delete Platform->gTileTexture;
        Platform->gTileTexture = nullptr;
        
        xfree(Platform->gMenuTexture);
        Platform->gMenuTexture->mTexture = nullptr;
        delete Platform->gMenuTexture;
        Platform->gMenuTexture = nullptr;

        xfree(Platform->gTextTexture);
        Platform->gTextTexture->mTexture = nullptr;
        delete Platform->gTextTexture;
        Platform->gTextTexture = nullptr;

        xfree(Platform->gUserTankTexture);
        Platform->gUserTankTexture->mTexture = nullptr;
        delete Platform->gUserTankTexture;
        Platform->gUserTankTexture = nullptr;

        xfree(Platform->gEnemyTankTexture);
        Platform->gEnemyTankTexture->mTexture = nullptr;
        delete Platform->gEnemyTankTexture;
        Platform->gEnemyTankTexture = nullptr;

        xfree(Platform->gUserBulletTexture);
        Platform->gUserBulletTexture->mTexture = nullptr;
        delete Platform->gUserBulletTexture;
        Platform->gUserBulletTexture = nullptr;

        xfree(Platform->gEnemyBulletTexture);
        Platform->gEnemyBulletTexture->mTexture = nullptr;
        delete Platform->gEnemyBulletTexture;
        Platform->gEnemyBulletTexture = nullptr;

        xfree(Platform->gExplosionTexture);
        Platform->gExplosionTexture->mTexture = nullptr;
        delete Platform->gExplosionTexture;
        Platform->gExplosionTexture = nullptr;

        
	//Destroy window	
    SDL_DestroyRenderer(Platform->gRenderer);
    SDL_DestroyWindow(Platform->gWindow);

    Platform->gRenderer = nullptr;
    Platform->gWindow = nullptr;
    // NOTE: Still don't know why this function immediately close the app
    // including terminal one
 	//Quit SDL subsystems
    TTF_Quit();
	IMG_Quit();
	SDL_Quit();   
}

bool LoadMedia(Tile* tiles,PlatformP* Platform){
	//Loading success flag
	bool success = true;

	//Load dot texture
    // NOTE: The Bugs lied here
	if( !loadFromFile( "media/myTank.png", Platform->gRenderer, (int)100, (int)100, Platform->gUserTankTexture) )
	{
		printf( "Failed to load User tank texture!\n" );
		success = false;
	}
// else {

      // Platform->gSpriteClips[ 0 ].x = 0;
      // Platform->gSpriteClips[ 0 ].y = 0;
      // Platform->gSpriteClips[ 0 ].w = 256;
      // Platform->gSpriteClips[ 0 ].h = 170;

      // Platform->gSpriteClips[ 1 ].x = 256;
      // Platform->gSpriteClips[ 1 ].y = 0;
      // Platform->gSpriteClips[ 1 ].w = 256;
      // Platform->gSpriteClips[ 1 ].h = 170;

      // Platform->gSpriteClips[ 2 ].x = 512;
      // Platform->gSpriteClips[ 2 ].y = 0;
      // Platform->gSpriteClips[ 2 ].w = 256;
      // Platform->gSpriteClips[ 2 ].h = 170;

      // Platform->gSpriteClips[ 3 ].x = 768;
      // Platform->gSpriteClips[ 3 ].y = 0;
      // Platform->gSpriteClips[ 3 ].w = 256;
      // Platform->gSpriteClips[ 3 ].h = 170;

//      gSpriteClips[ 4 ].x = 1000;
//      gSpriteClips[ 4 ].y = 0;
//      gSpriteClips[ 4 ].w = 250;
//      gSpriteClips[ 4 ].h = 170;
     
    // }
    
	if( !loadFromFile( "media/enemyTank.png", Platform->gRenderer, (int)100, (int)100, Platform->gEnemyTankTexture))
	{
		printf( "Failed to load Enemy tank texture!\n" );
		success = false;
	} // else {

    // NOTE: Prepare a animated background here
	// if( !Platform->gMenuTexture->loadFromFile( "media/background.png", Platform->gRenderer, (int)100, (int)100))
	// {
	// 	printf( "Failed to load menu background texture!\n" );
	// 	success = false;
	// } 
// else {


	//Load tile texture
    if( !loadFromFile( "media/32x32_map_tile v3.1 [MARGINLESS].bmp", Platform->gRenderer, (int)30, (int)30, Platform->gTileTexture) )
	{
		printf( "Failed to load tile set texture!\n" );
		success = false;
	}
    
	if( !loadFromFile( "media/myBullet.png", Platform->gRenderer, (int)20, (int)20, Platform->gUserBulletTexture) )
	{
		printf( "Failed to load user bullet set texture!\n" );
		success = false;
	}
    
	if( !loadFromFile( "media/EnemyBullet.png", Platform->gRenderer, (int)20, (int)20, Platform->gEnemyBulletTexture) )
	{
		printf( "Failed to load enemy bullet set texture!\n" );
		success = false;
	}
    
	//Load explosion texture
	if( !loadFromFile( "media/explosion3(background removed).png", Platform->gRenderer, (int)100, (int)100, Platform->gExplosionTexture) )
	{
		printf( "Failed to load explosion texture!\n" );
		success = false;
	} else {

      Platform->gExplosionClips[ 0 ].x = 0;
      Platform->gExplosionClips[ 0 ].y = 0;
      Platform->gExplosionClips[ 0 ].w = 90;
      Platform->gExplosionClips[ 0 ].h = 90;

      Platform->gExplosionClips[ 1 ].x = 95;
      Platform->gExplosionClips[ 1 ].y = 0;
      Platform->gExplosionClips[ 1 ].w = 90;
      Platform->gExplosionClips[ 1 ].h = 90;

      Platform->gExplosionClips[ 2 ].x = 185;
      Platform->gExplosionClips[ 2 ].y = 0;
      Platform->gExplosionClips[ 2 ].w = 90;
      Platform->gExplosionClips[ 2 ].h = 90;

      Platform->gExplosionClips[ 3 ].x = 275;
      Platform->gExplosionClips[ 3 ].y = 0;
      Platform->gExplosionClips[ 3 ].w = 90;
      Platform->gExplosionClips[ 3 ].h = 90;

      Platform->gExplosionClips[ 4 ].x = 365;
      Platform->gExplosionClips[ 4 ].y = 0;
      Platform->gExplosionClips[ 4 ].w = 90;
      Platform->gExplosionClips[ 4 ].h = 90;

      Platform->gExplosionClips[ 5 ].x = 0;
      Platform->gExplosionClips[ 5 ].y = 90;
      Platform->gExplosionClips[ 5 ].w = 90;
      Platform->gExplosionClips[ 5 ].h = 90;

      Platform->gExplosionClips[ 6 ].x = 95;
      Platform->gExplosionClips[ 6 ].y = 90;
      Platform->gExplosionClips[ 6 ].w = 90;
      Platform->gExplosionClips[ 6 ].h = 90;      

      Platform->gExplosionClips[ 7 ].x = 185;
      Platform->gExplosionClips[ 7 ].y = 90;
      Platform->gExplosionClips[ 7 ].w = 90;
      Platform->gExplosionClips[ 7 ].h = 90;      

      Platform->gExplosionClips[ 8 ].x = 275;
      Platform->gExplosionClips[ 8 ].y = 90;
      Platform->gExplosionClips[ 8 ].w = 90;
      Platform->gExplosionClips[ 8 ].h = 90;      

      Platform->gExplosionClips[ 9 ].x = 365;
      Platform->gExplosionClips[ 9 ].y = 90;
      Platform->gExplosionClips[ 9 ].w = 90;
      Platform->gExplosionClips[ 9 ].h = 90;      
    }

	//Load tile map
	if( !setTiles( tiles, Platform) )
	{
		printf( "Failed to load tile set!\n" );
		success = false;
	}

	return success;
}

bool init(PlatformP* Platform)
{
	//Initialization flag
	bool success = true;

    if(Platform->gWindow != nullptr){
        Platform->gWindow = nullptr;
    }

    if (Platform->gRenderer != nullptr)
    {
        Platform->gRenderer = nullptr;
    }
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

		// //Set turn off VSYNC
		// if( !SDL_SetHint( SDL_HINT_RENDER_VSYNC, "0" ) )
		// {
		// 	printf( "Warning: Can not turn VSYNC off!" );
		// }

		//Create window
        if (Platform->gWindow != nullptr){
            Platform->gWindow = nullptr;
        }
        // Platform->gWindow = (struct SDL_Window*)malloc(struct SDL_Window);
		Platform->gWindow = SDL_CreateWindow( "SDL Tank Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE );
		if( Platform->gWindow == nullptr )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
            if (Platform->gRenderer != nullptr){
                Platform->gRenderer = nullptr;
            }
			Platform->gRenderer = SDL_CreateRenderer( Platform->gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( Platform->gRenderer == nullptr )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( Platform->gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

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
                if(Platform->gFont != nullptr){
                    Platform->gFont = nullptr;
                }
                Platform->gFont = TTF_OpenFont( "Roboto.ttf", 28 );
                Platform->TextColor = {249 ,166 ,2};
                // SDL_Color TextColor = {0 ,0 ,0};
			}
		}
	}
    
	return success;    
}


//Sets tiles from tile map
bool setTiles( Tile *tiles,PlatformP* Platform){
    
	//Success flag
	bool tilesLoaded = true;

    //The tile offsets
    int x = 0, y = 0;

    //Open the map
    std::ifstream map( "smap.map" );

    //If the map couldn't be loaded
    if( map.fail() )
    {
		printf( "Unable to load map file!\n" );
		tilesLoaded = false;
    }
	else
	{
        int i = 0, k = 0;
        char TileType[2] = {};
		//Initialize the tiles
		for (int i = 0; i < TOTAL_TILES; i ++ )
		{

            // printf('Current Tile: %c\n');
			//Determines what kind of tile will be made
        	uint8 tileType = -1;
            
			//Read tile from map file

            map >> TileType[0];
			map >> TileType[1];
            
            tileType = (TileType[0] - 48)*10 + (TileType[1] - 48);

            // map >> tileType;
            
			//If the was a problem in reading the map
			if( map.fail() )
			{
				//Stop loading map
				printf( "Error loading map: Unexpected end of file at iteration: %d !\n", i);
				tilesLoaded = false;
				break;
			}

			//If the number is a valid tile number
			if( ( tileType >= 0 ) && ( tileType < TOTAL_TILE_SPRITES ) )
			{
                // if (tileType < 11 && tileType > 6)
                // {
                //     if(x == 0){
                //     tiles[ i ] = new Tile( LEVEL_WIDTH - SMALL_TILE_WIDTH , y - SMALL_TILE_HEIGHT, tileType);
                //     }else{
                //     tiles[ i ] = new Tile( x - SMALL_TILE_WIDTH , y, tileType);                        
                //     }
                // }else{
                // tiles[ i ] = new Tile( x , y, tileType);                    
                // }
                tiles[i].mBox = {x, y, SMALL_TILE_WIDTH, SMALL_TILE_HEIGHT};
                tiles[i].mType = tileType;
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
			x += SMALL_TILE_WIDTH;

			//If we've gone too far
			if( x >= LEVEL_WIDTH)
			{
				//Move back
				x = 0;

                // NOTE: Why when go down there are no tile there any more
                
				//Move to the next row
				y += SMALL_TILE_HEIGHT;
			}
		}
		
		//Clip the sprite sheet
		if( tilesLoaded )
		{
            // NOTE: TILE_RED or TILE_GREEN is TileTypes
            // printf("Loading Map\n");
			// Platform->gTileClips[ SMALL_GRASS_TILE ].x = 0;
			// Platform->gTileClips[ SMALL_GRASS_TILE ].y = 30;
			// Platform->gTileClips[ SMALL_GRASS_TILE ].w = SMALL_TILE_WIDTH;
			// Platform->gTileClips[ SMALL_GRASS_TILE ].h = SMALL_TILE_HEIGHT;

			Platform->gTileClips[ SMALL_GRASS_TILE_1 ].x = 0;
			Platform->gTileClips[ SMALL_GRASS_TILE_1 ].y = 130;
			Platform->gTileClips[ SMALL_GRASS_TILE_1 ].w = SMALL_TILE_WIDTH;
			Platform->gTileClips[ SMALL_GRASS_TILE_1 ].h = SMALL_TILE_HEIGHT;

			Platform->gTileClips[ SMALL_GRASS_TILE_2 ].x = 30;
			Platform->gTileClips[ SMALL_GRASS_TILE_2 ].y = 130;
			Platform->gTileClips[ SMALL_GRASS_TILE_2 ].w = SMALL_TILE_WIDTH;
			Platform->gTileClips[ SMALL_GRASS_TILE_2 ].h = SMALL_TILE_HEIGHT;

			Platform->gTileClips[ SMALL_GRASS_TILE_3 ].x = 60;
			Platform->gTileClips[ SMALL_GRASS_TILE_3 ].y = 130;
			Platform->gTileClips[ SMALL_GRASS_TILE_3 ].w = SMALL_TILE_WIDTH;
			Platform->gTileClips[ SMALL_GRASS_TILE_3 ].h = SMALL_TILE_HEIGHT;

			Platform->gTileClips[ SMALL_GRASS_TILE_4 ].x = 60;
			Platform->gTileClips[ SMALL_GRASS_TILE_4 ].y = 160;
			Platform->gTileClips[ SMALL_GRASS_TILE_4 ].w = SMALL_TILE_WIDTH;
			Platform->gTileClips[ SMALL_GRASS_TILE_4 ].h = SMALL_TILE_HEIGHT;

			Platform->gTileClips[ SMALL_GRASS_TILE_5 ].x = 60;
			Platform->gTileClips[ SMALL_GRASS_TILE_5 ].y = 160;
			Platform->gTileClips[ SMALL_GRASS_TILE_5 ].w = SMALL_TILE_WIDTH;
			Platform->gTileClips[ SMALL_GRASS_TILE_5 ].h = SMALL_TILE_HEIGHT;

			Platform->gTileClips[ SMALL_GRASS_TILE_6 ].x = 30;
			Platform->gTileClips[ SMALL_GRASS_TILE_6 ].y = 190;
			Platform->gTileClips[ SMALL_GRASS_TILE_6 ].w = SMALL_TILE_WIDTH;
			Platform->gTileClips[ SMALL_GRASS_TILE_6 ].h = SMALL_TILE_HEIGHT;

			Platform->gTileClips[ SMALL_STUMP_TILE_1 ].x = 0;
			Platform->gTileClips[ SMALL_STUMP_TILE_1 ].y = 190;
			Platform->gTileClips[ SMALL_STUMP_TILE_1 ].w = SMALL_TILE_WIDTH;
			Platform->gTileClips[ SMALL_STUMP_TILE_1 ].h = SMALL_TILE_HEIGHT;

			Platform->gTileClips[ SMALL_STUMP_TILE_2 ].x = 0;
			Platform->gTileClips[ SMALL_STUMP_TILE_2 ].y = 220;
			Platform->gTileClips[ SMALL_STUMP_TILE_2 ].w = SMALL_TILE_WIDTH;
			Platform->gTileClips[ SMALL_STUMP_TILE_2 ].h = SMALL_TILE_HEIGHT;

			Platform->gTileClips[ SMALL_STUMP_TILE_3 ].x = 0;
			Platform->gTileClips[ SMALL_STUMP_TILE_3 ].y = 250;
			Platform->gTileClips[ SMALL_STUMP_TILE_3 ].w = SMALL_TILE_WIDTH;
			Platform->gTileClips[ SMALL_STUMP_TILE_3 ].h = SMALL_TILE_HEIGHT;

			Platform->gTileClips[ SMALL_STUMP_TILE_4 ].x = 0;
			Platform->gTileClips[ SMALL_STUMP_TILE_4 ].y = 280;
			Platform->gTileClips[ SMALL_STUMP_TILE_4 ].w = SMALL_TILE_WIDTH;
			Platform->gTileClips[ SMALL_STUMP_TILE_4 ].h = SMALL_TILE_HEIGHT;

			Platform->gTileClips[ SMALL_WATER_1 ].x = 0;
			Platform->gTileClips[ SMALL_WATER_1 ].y = 510;
			Platform->gTileClips[ SMALL_WATER_1 ].w = SMALL_TILE_WIDTH;
			Platform->gTileClips[ SMALL_WATER_1 ].h = SMALL_TILE_HEIGHT;

			Platform->gTileClips[ SMALL_WATER_2 ].x = 0;
			Platform->gTileClips[ SMALL_WATER_2 ].y = 540;
			Platform->gTileClips[ SMALL_WATER_2 ].w = SMALL_TILE_WIDTH;
			Platform->gTileClips[ SMALL_WATER_2 ].h = SMALL_TILE_HEIGHT;

			Platform->gTileClips[ SMALL_WOOD_PATH_1 ].x = 320;
			Platform->gTileClips[ SMALL_WOOD_PATH_1 ].y = 30;
			Platform->gTileClips[ SMALL_WOOD_PATH_1 ].w = SMALL_TILE_WIDTH;
			Platform->gTileClips[ SMALL_WOOD_PATH_1 ].h = SMALL_TILE_HEIGHT;
		}
	}

    //Close the file
    map.close();

    //If the map was loaded fine
    return tilesLoaded;    
}

//Shows the Tank on the screen
void renderTank(TankInfo* Tank, int frame, SDL_Rect& camera, PlatformP* Platform) {

    if(!Tank->destroyed) {
    //NOTE: Show the tank and bullet here
    if (Tank->Belong && Platform->gUserTankTexture!=nullptr) {
        // printf("User Tank image is being rendered\n");
        render( Platform->gRenderer ,(Tank->mBox.x - camera.x), (Tank->mBox.y - camera.y), Platform->gUserTankTexture, nullptr,Tank->face);

        // NOTE: Now the bullets
        // TODO: The bullet not show, Time to check this one
        // The bullet stuck with number 0 while get near bot Tank
        if(Platform->gUserBulletTexture!=nullptr) {             
        for (int i = 0; i < TOTAL_BULLET_PER_TANK; i++){
            if (Tank->Bullets[i].Launched){                
                // printf("Bullets %d image is being rendered\n", i);
                render(Platform->gRenderer, (Tank->Bullets[i].blBox.x - camera.x), (Tank->Bullets[i].blBox.y - camera.y), Platform->gUserBulletTexture, nullptr, Tank->face);
            }
        }
    }

  }
else if (!Tank->Belong && Platform->gEnemyTankTexture != nullptr) {
        // SOMEHOW the enemy tank positions changed to keep in bound while the text one is not
        // if (((Tank->mBox.x >= camera.x) && (Tank->mBox.x <= camera.x + camera.w)) &&((Tank->mBox.y >= camera.y) && (Tank->mBox.y <= camera.y + camera.w)) ){
    // printf("Tank is being rendered\n");
    render( Platform->gRenderer ,Tank->mBox.x - camera.x,Tank->mBox.y - camera.y, Platform->gEnemyTankTexture, nullptr, Tank->face);
        if(Platform->gEnemyBulletTexture!=nullptr) {             
        for (int i = 0; i < TOTAL_BULLET_PER_TANK; i++){
            if (Tank->Bullets[i].Launched){                
                render(Platform->gRenderer, (Tank->Bullets[i].blBox.x - camera.x), (Tank->Bullets[i].blBox.y - camera.y), Platform->gEnemyBulletTexture, nullptr, Tank->face);
                // printf("Enemy tanks and Bullets %d image is being rendered\n", i);
            }
        }
    }        
        // }
    }        
    }//  else {
    //     Tank->userBelong?printf("user Tank "):printf("Bot Tank ");
    //     printf("is hit and destroyed\n");
    // }

    // if(Tank->isHit && !Tank->userBelong){
    //     //TODO: Run the destroying clip here and then make the tanks disappear
    // }
    
}

void renderText(real32 FPS, const TankInfo* userTank, PlatformP* Platform){
    
    // NOTE: The reason that I can not use TTF_OpenFont or TTF_Init is
    // that I forgot to link ttf.lib in compile link
    // carefull to link, remember to put dll files in system32 folder
                
    char OutPut[50];
    // NOTE: Somehow The exe file can't find out the TTF_OpenFont and TTF_Solid_Render which is in the ttf lib. Got to find out and fix
                
    sprintf(OutPut ,"FPS: %d \n",int(FPS));

    // printf(OutPut);
                
    if (!loadFromRenderedText(OutPut, 1.0f, Platform->TextColor, Platform->gFont, Platform->gRenderer, Platform->gTextTexture)) {
        printf( "Can not Load Text to render! SDL Error: %s\n", SDL_GetError() );                            
    } else {
        render(Platform->gRenderer, 0, 0, Platform->gTextTexture);                    
    }

    if(userTank->BulletsNumber == 0){
        sprintf(OutPut, "Tank Bullets: Loading\n");
        if (!loadFromRenderedText(OutPut, 1.0f, Platform->TextColor, Platform->gFont, Platform->gRenderer, Platform->gTextTexture)) {
        //Update screen
            printf( "Can not Load Text to render! SDL Error: %s\n", SDL_GetError() );                            
        } else {
            render(Platform->gRenderer, SCREEN_WIDTH - 300, 0, Platform->gTextTexture);                    
        }                    
        // printf(OutPut);
    } else {                    
        sprintf(OutPut, "Tank Bullets :%d \n", int(userTank->BulletsNumber));
        //Update screen
        if (!loadFromRenderedText(OutPut, 1.0f, Platform->TextColor, Platform->gFont, Platform->gRenderer, Platform->gTextTexture)) {
            printf( "Can not Load Text to render! SDL Error: %s\n", SDL_GetError() );                            
        } else {
            render(Platform->gRenderer, SCREEN_WIDTH - 200, 0, Platform->gTextTexture);                    
        }                    
        // printf(OutPut);
    } 
}

void renderExplosionFrame(TankInfo* Tank, PlatformP* Platform, SDL_Rect* camera , uint8_t* frame, int frameIndex){

    // real32 ExploFrameStartTime = 0.0f;
    // real32 ExploFrameEndTime = 0.0f;
    // real32 ExploFrameTime = 0.0f; ;

         // NOTE: This secure the game check isHit flag first then
         // destroyed one
         if(frame[frameIndex] == -1){
             frame[frameIndex] = 0;
         }

         if(frame[frameIndex]/12 < ANIMATING_FRAMES+1 && frame[frameIndex]/12 != -1){
             render( Platform->gRenderer ,(Tank->mBox.x - camera->x), (Tank->mBox.y - camera->y), Platform->gExplosionTexture, &Platform->gExplosionClips[frame[frameIndex]/9]);            

             (frame[frameIndex])++;
         }

         if(frame[frameIndex]/12 == ANIMATING_FRAMES+1){
             frame[frameIndex] = -1;
             Tank->destroyed = true;
         };
}    
