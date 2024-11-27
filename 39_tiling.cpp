/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */

//Using SDL, SDL_image, standard IO, strings, and file streams
#include <string>
#include <cstring>
#include "SDL2/SDL_timer.h"
#include "Tile.h"
#include "xTank.h"
#include "GamePhysics.h"
// #include "Dot.h"
// #include "BasicPlatform.h"


// NOTE: I think I bassically done shipping code to different parts
// But seem like the platform write in OOP styles is kind of slow
// Got to try in the procedure way
// Time to test it with fps DONE!
// The procedure style seem indeed squeeze way more performace than the OOP style
// Then Try to search for OpenGl document source  

#define internal static
#define global_variable static
#define uint32 uint32_t
#define real32 float

#include "PlatformP.h"

global_variable uint32 StartTime;
global_variable uint32 EndTime;
// const global_variable int KEY_COUNT = SDL_NUM_SCANCODES;

global_variable uint32 FPS;
int main( int argc, char* args[] )
{
    // BasicPlatform Platform;
	//Start up SDL and create window
    TankInfo* userTank = nullptr;
    TankInfo* enemyTank = nullptr;
    
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
        //NOTE: Don't know why window and renderer pointer still be NULL after inittialization DONE!
        
		Tile* tileSet[ TOTAL_TILES ];

		if( ! LoadMedia(tileSet) )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;
			//The dot that will be moving around on the screen
			// Dot dot;

            bool User = true;
 
            userTank = new TankInfo(User);
            enemyTank = new TankInfo[TOTAL_ENEMY_TANK];            
            
            Position* TankPos = nullptr;

            TankPos = InitializeTankPos();
            
            for (int i = 0 ; i < TOTAL_ENEMY_TANK; i++){
                enemyTank[i] = InitializeTankInfo(TankPos[i].X, TankPos[i].Y);
            }
            
			//Level camera
			SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            
            KeyState PreviousBut = {};
            KeyState CurrentBut = {};
            int frame = 0;
			//While application is running
			while( !quit )
			{
                SDL_GetWindowSize(Platform.gWindow, &SCREEN_WIDTH, &SCREEN_HEIGHT);
                // Time per frame
                StartTime = (uint32) SDL_GetTicks();
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}

                    // Modulize this part to reuse it
                    //===================================================

                    if ((e.key.state == SDL_PRESSED || e.key.state == SDL_RELEASED) && e.key.keysym.scancode != SDL_SCANCODE_UNKNOWN){                        
                            // if (!CurrentArrow.pressed){
                            //     printf("Current Arrow is released\n");
                            // } else {
                            //     printf("Current Arrow is pressed\n");                                
                            // }

                            // if (e.key.state == SDL_PRESSED){
                            //     printf("Next Arrow is pressed\n");
                            // }  else {
                            //     printf("Next Arrow is released\n");
                            // } 
                        // NOTE: || (e.key.keysym.scancode == CurrentArrow.key)
                        // || e.key.keysym.scancode == SDL_SCANCODE_SPACE
                        //                        //
                                // printf("Current Button :%d ", CurrentBut.key);
                                // CurrentBut.pressed?printf("is pressed \n"):printf("is released\n");
                                
                                // printf("Next Button to be checked :%d ",e.key.keysym.scancode);
                                // e.key.state == SDL_PRESSED?printf("is pressed \n"):printf("is released\n");

                        //
                                if((!CurrentBut.pressed && e.key.state == SDL_PRESSED && CurrentBut.key != e.key.keysym.scancode) || (e.key.keysym.scancode == CurrentBut.key && CurrentBut.pressed != (e.key.state == SDL_PRESSED))|| CurrentBut.init == 0 || e.key.keysym.scancode == SDL_SCANCODE_SPACE)

                        //TODO: How about pressing Space and 2 arrow key at the same
                        //time and then hit different arrow
                            {
                                // printf("Previous Button :%d ", PreviousBut.key);
                                // PreviousBut.pressed?printf("is pressed \n"):printf("is released\n");
                                
                                if(CurrentBut.init == 0){
                                    CurrentBut.init = 1;
                                }
                                // printf("Current Button init: %d\n", CurrentBut.init);
                                PreviousBut = CurrentBut;
                                CurrentBut.pressed = (e.key.state == SDL_PRESSED);
                                CurrentBut.key = e.key.keysym.scancode;
                                CurrentBut.repeat = e.key.repeat;
                                handleEvent(&CurrentBut, userTank);                                
                            }
                    }
                    // ==================================================
					//handle input for the dot
					// dot.handleEvent( e );
                    // }
				}
                // TODO: Time to Fix !!!
                //
                        //Go to next frame
                        frame++;
                        //Cycle animation
                        if( frame / 4 > ANIMATING_FRAMES)
                        {
                            frame = 0;
                        }

				//Move the dot
                bool collided = false;
                move(tileSet, touchesWall(&userTank->mBox, tileSet), collided, userTank);
                for (int i = 0; i < TOTAL_ENEMY_TANK; i++){
                    // TODO: Constantly check collision between bot tanks
                    collided = false;
                    // for (int p = 0; p < i; p++){
                    //     // NOTE: Collision with each with User Tank and
                    //     // with the bullets here
                    //     if (i > 1 && p < i){
                    //         //TODO: Separate these bot tank and move on to
                    //         //enable bullet to kill userTank
                    //         //?printf("Tank %d collided tank %d \nm ", i, p):printf("checking\n")
                    //         collided = checkCollision(&enemyTank[i].mBox, &enemyTank[p].mBox);
                    //     }
                    // }
                    //

                    BiTankCheck(&enemyTank[i], userTank);
                    
                    littleGuide(&enemyTank[i], userTank, collided);
                    move(tileSet,touchesWall(&enemyTank[i].mBox, tileSet), collided, &enemyTank[i]);
                }                

                // LATER!: Move Bot tank use little guild(DONE!)
                //

				// dot.move( tileSet, touchesWall(dot.GetmBox(), tileSet));
                setCamera(camera, userTank);

                
				//Clear screen
				SDL_SetRenderDrawColor( Platform.gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				// SDL_SetRenderDrawColor( Platform.GetRenderer(), 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( Platform.gRenderer);
				// SDL_RenderClear( Platform.GetRenderer());

				//Render current frame
				// SDL_Rect* currentClip = &Platform.gSpriteClips[ frame / 7 ];
				// SDL_Rect* currentClip = &Platform.GetgSpriteClips()[ frame / 7 ];
                
				// dot.setCamera( camera );                
                
				//Render level
				for( int i = 0; i < TOTAL_TILES; ++i )
				{
                    // NOTE: The tile position must subtract for the camera
                    // pos to created moving illusiion
                    
					tileSet[ i ]->render( camera, Platform.gRenderer,  Platform.gTileTexture,  Platform.gTileClips, checkCollision(&camera, tileSet[ i ]->getBox()));
					// tileSet[ i ]->render( camera, Platform.GetRenderer(),  Platform.GetgTileTexture(),  Platform.GetgTileClips(), checkCollision(&camera, tileSet[ i ]->getBox()));
				}


                render(userTank, frame, camera);

                for (int i = 0; i < TOTAL_ENEMY_TANK; i++){
                    render(&enemyTank[i], frame, camera);
                }
                
				//Render dot
				// dot.render( Platform.gDotTexture, camera, currentClip,  Platform.gRenderer);
				// dot.render( Platform.GetgDotTexture(), camera, currentClip,  Platform.GetRenderer());
                renderText(StartTime, EndTime, userTank);
				SDL_RenderPresent( Platform.gRenderer);
				// SDL_RenderPresent( Platform.GetRenderer());
		}
		//Free resources and close SDL
		 // Platform.close( tileSet);
                close( tileSet);
	}            
    }
    delete userTank;
    delete[] enemyTank;
    return 0;
}
