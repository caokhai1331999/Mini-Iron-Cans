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

global_variable real32 StartTime;
global_variable real32 EndTime;
global_variable real32 TimeElapsed = 0;
global_variable real32 FPS = 0;
const global_variable real32 StandardFPS = 60.0f;

const global_variable uint32 FRAME_DELAY = 1000/60;
// const global_variable int KEY_COUNT = SDL_NUM_SCANCODES;

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
                enemyTank[i] = InitializeTankInfo(TankPos[i].x, TankPos[i].y);
            }
            
			//Level camera
			SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            KeyState PreviousBut = {};
            KeyState CurrentBut = {};
            int Uframe = 0;
            int testFrame = 0;
            int frame[4] = {} ;
            StartTime =  SDL_GetTicks();
            real32 Startframe = 0;
            real32 Endframe = 0;
            real32 FrameElapse = 0;
            // uint32 LastFrameTime[TOTAL_ENEMY_TANK] ={};
            int k;
			while( !quit )
			{
                EndTime = SDL_GetTicks();
                SDL_GetWindowSize(Platform.gWindow, &SCREEN_WIDTH, &SCREEN_HEIGHT);
                // Time per frame
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT || e.key.keysym.scancode == 41 )
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
				//Move the dot
                bool collided = false;
                // NOTE: Bugs lied here
                move(tileSet, touchesWall(&userTank->mBox, tileSet), collided, userTank);

                for (int i = 0; i < TOTAL_ENEMY_TANK; i++){
                    collided = false;
                    for (int p = 0; p < i; p++){
                        if (i > 1 && p < i){
                            collided = checkCollision(&enemyTank[i].mBox, &enemyTank[p].mBox);
                        }
                    }
                    //

                    BiTankCheck(&enemyTank[i], userTank);
                    littleGuide(&enemyTank[i], userTank, collided);
                    move(tileSet,touchesWall(&enemyTank[i].mBox, tileSet), collided, &enemyTank[i]);
                }                                
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
                
				//Render evel
				for( int i = 0; i < TOTAL_TILES; ++i )
				{
                    // NOTE: The tile position must subtract for the camera
                    // pos to created moving illusiion
                    // printf("tile %d is rendered\n", i);
					tileSet[ i ]->render( camera, Platform.gRenderer,  Platform.gTileTexture,  Platform.gTileClips, touchesWall(&userTank->mBox, tileSet));
					// tileSet[ i ]->render( camera, Platform.GetRenderer(),  Platform.GetgTileTexture(),  Platform.GetgTileClips(), checkCollision(&camera, tileSet[ i ]->getBox()));
				}
               
               render(userTank, Uframe, camera);
               // NOTE: Bugs lied here
                k = 0;
                while(k < TOTAL_ENEMY_TANK)
                {
                    if(enemyTank[k].isHit){
                        if (!enemyTank[k].userBelong && !enemyTank[k].destroyed)
                        {
                            // NOTE: Because of the interupt of the loop caused
                            // Frame scrolling not fast enough to make animation
                            //
                            //But This one is like too fast for nake eye to see
                            //the animation
                            //
                            // Uint32 CurrentFrameTime = SDL_GetTicks();
                            
                                //     //Cycle animation
                                // {                                
                                    if (frame[k]/12 == ANIMATING_FRAMES+1){
                                        resetTank(&enemyTank[k]);
                                        frame[k] = -1;
                                    }
                                    //Go to next frame
                                    //NOTE: Why the first explosion animation just


                                    // SDL_Delay(16);
                                    if(frame[k]!=-1){
                                        // if(CurrentFrameTime > LastFrameTime[k] + FRAME_DELAY)                            {
                                            // printf("Last frame time is :%d, and Current frame Time is: %d\n", LastFrameTime[k], CurrentFrameTime);
                                        // LastFrameTime[k] = CurrentFrameTime;
                                        Platform.gExplosionTexture->render( Platform.gRenderer ,(enemyTank[k].mBox.x - camera.x), (enemyTank[k].mBox.y - camera.y), &Platform.gExplosionClips[frame[k]]);
                                        SDL_Delay(0.032);
                                        frame[k]==0?printf("Tank %d is hit and start to display\n", k):printf("Tank %d explosion at frame %d\n", k, frame[k]);
                                        frame[k]++;
                                        // }
                                    }
                        }
                    } else {
                        render(&enemyTank[k], frame[k], camera);                        
                    }
                    if (frame[k] == -1 || !enemyTank[k].isHit){
                        k++;
                    }
                };
                   
                
                renderText(FPS, userTank);
				SDL_RenderPresent( Platform.gRenderer);
                TimeElapsed = EndTime - StartTime;
                // printf("TimeElapsed is: %f\n", (real32)TimeElapsed);
                FPS = 1/(TimeElapsed/1000.0f);
                // printf("FPS is: %f\n", (real32)FPS);
                // if(TimeElapsed < (1000*(1/StandardFPS))){
                //     SDL_Delay((uint32)((1000.0f*(1/StandardFPS)) - TimeElapsed));
                // }
                // EndTime is what come later
                StartTime = EndTime;
				// SDL_RenderPresent( Platform.GetRenderer());

				//Render dot
				// dot.render( Platform.gDotTexture, camera, currentClip,  Platform.gRenderer);
				// dot.render( Platform.GetgDotTexture(), camera, currentClip,  Platform.GetRenderer());
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
