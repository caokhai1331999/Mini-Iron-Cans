/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include "Gameh.h"

void displayMenu(Game* g){
    gFont = TTF_OpenFont( "Roboto.ttf", 28 );
    // SDL_Color TextColor = {249 ,166 ,2};
    SDL_Color TextColor = {0 ,0 ,0};
    float scale = 0.0f;

    for(uint8_t i = 0; i < 4; ++i){

        // NOTE: Scale up the text whenever it is pointed to
        if(g->pointed_option == i+1){            
            scale = 1.5f;   
        } else {
            scale = 1.0f;
        }

        if (!loadFromRenderedText(Menu[i], scale, TextColor, gFont, g->Platform->gRenderer, g->Platform->gMenuTexture)) {
            printf( "Can not Load Text to render! SDL Error: %s\n", SDL_GetError() );
        } else {
            render(g->Platform->gRenderer, 100, 30 + i*50, g->Platform->gMenuTexture);
        }                       

    }    
}

void get_Menu_choice(Game* g, KeyState* currentKey){
        if (currentKey->pressed){
        switch(currentKey->key){
            case SDL_SCANCODE_DOWN:
                g->pointed_option++;
                if(g->pointed_option > 4){
                    g->pointed_option = 1;
                }
                break;
            case SDL_SCANCODE_UP :

                if(g->pointed_option == 1 || g->pointed_option == 0){
                    g->pointed_option = 4;
                }else {
                    g->pointed_option--;
                }
                
                break;
            case SDL_SCANCODE_RETURN:
                if(g->pointed_option > 0 && g->pointed_option < 5){                    
                    if(g->chosen_option != g->pointed_option)
                    {
                        g->chosen_option = static_cast<MENUCHOICE>(g->pointed_option);
                        g->chosen_option == NEW_GAME||g->chosen_option == RESUME?printf("Start enter the game\n"):printf("not yet\n");
                        g->chosen_option == NONE?printf("Option doesn't change after hitting enter yet. Why??\n"):printf("Options changed\n");
                        g->chosen_option == EXIT?printf("Option is now exit\n"):printf("why option is not exit yet\n");
                    }
                }
                break;                                 
        }
    }
}

bool Start(Game* g){
        if(!init(g->Platform)){
            return false;
        }else{
            if( !LoadMedia(g->tileSet, g->Platform) )
            {
                printf( "Failed to load media!\n" );
                return false;
            }
            else
            {
                InitializeTankPos(g->TankPos);
            
                for (int i = 0 ; i < TOTAL_ENEMY_TANK; i++){
                   g-> enemyTank[i] = InitializeTankInfo(g->TankPos[i].x, g->TankPos[i].y);
                }
                
                //Level camera
                camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

                if(frame != nullptr){
                    delete []frame;
                    frame = nullptr;
                }

                frame = new uint8_t[5];

                for(int i = 0; i < 5; i++){
                    frame[i] = -1;
                };

                Ecollided = false;
                Ucollided = false;
                // uint32 LastFrameTime[TOTAL_ENEMY_TANK] ={};
                return true;
            }            
        }
}

void changeState(Game* g, KeyState* key){
    if(g->state != EMPTY){
        switch(g->state){
            case MENU_INIT:
                if (g->chosen_option == NEW_GAME){
                    g->state = GAME_NEW;
                    // Game Constantly reset 
                    g->chosen_option = NONE;
                } else if (g->chosen_option == EXIT || key->type == SDL_QUIT){
                    key->type == SDL_QUIT?printf("quit event is triggered\n"):printf("exit option is chosen\n");
                    g->state = EMPTY;
                }
                break;
            case PAUSE:
                if(g->chosen_option == NEW_GAME){
                    // NOTE: Try to put Reset fx here to see anything different
                    resetGame(g);
                    // ====================
                    g->state = GAME_NEW;
                    g->chosen_option = NONE;
                }else if (g->chosen_option == RESUME){
                    g->state = GAME_RELOADED;
                    g->chosen_option = NONE;
                }else if (g->chosen_option == EXIT || key->type == SDL_QUIT){
                    g->state = EMPTY;
                };
                
                break;
            case GAME_NEW:
                if (key->key == SDL_SCANCODE_ESCAPE && key->pressed){
                    g->state = PAUSE;
                } else if (key->type == SDL_QUIT){
                    g->state = EMPTY;
                }
                break;

            case GAME_RELOADED:
                if (key->key == SDL_SCANCODE_ESCAPE && key->pressed){
                    g->state = PAUSE;
                } else if (key->type == SDL_QUIT){
                    g->state = EMPTY;
                }
                break;
        }            
    }    
}

void ProcessInput(Game* g, bool* done){
    // printf("Start process input \n");
    if (!*done){
    KeyState PreviousBut = {};
    KeyState CurrentBut = {};
    
    while( SDL_PollEvent( &g->Platform->e ) != 0 )
    {
        //User requests quit

        // Modulize this part to reuse it
        //===================================================

        // NOTE: If I have time try to apply FSM to this input filter
        if ((g->Platform->e.key.state == SDL_PRESSED || g->Platform->e.key.state == SDL_RELEASED) && g->Platform->e.key.keysym.scancode != SDL_SCANCODE_UNKNOWN || g->Platform->e.type == SDL_QUIT){                        
            // NOTE: Forgot to add SDL_QUIT to filter conditions
            if((!CurrentBut.pressed && g->Platform->e.key.state == SDL_PRESSED && CurrentBut.key != g->Platform->e.key.keysym.scancode) || (g->Platform->e.key.keysym.scancode == CurrentBut.key && CurrentBut.pressed != (g->Platform->e.key.state == SDL_PRESSED))|| CurrentBut.init == 0 || g->Platform->e.key.keysym.scancode == SDL_SCANCODE_SPACE)

                                
                if(CurrentBut.init == 0){
                    CurrentBut.init = 1;
                }
            
            PreviousBut = CurrentBut;
            CurrentBut.type = g->Platform->e.type;

            CurrentBut.pressed = (g->Platform->e.key.state == SDL_PRESSED);
            CurrentBut.key = g->Platform->e.key.keysym.scancode;
            CurrentBut.repeat = g->Platform->e.key.repeat;

            if(CurrentBut.type == SDL_QUIT){
                *done = true;
            }
            
                switch(g->state){
                    case MENU_INIT:
                        get_Menu_choice(g, &CurrentBut);
                        break;
                    case PAUSE:
                        get_Menu_choice(g, &CurrentBut);
                        break;
                    case GAME_NEW:
                        if (CurrentBut.key != 41){
                            handleEventForTank(&CurrentBut, g->userTank);
                        }
                        break;
                    case GAME_RELOADED:
                        if (CurrentBut.key != 41){
                            handleEventForTank(&CurrentBut, g->userTank);
                        }
                        break;
                };
                changeState(g, &CurrentBut);
        }
    }                    
    } 
}

void runMainScene(Game* g){

    if(!g->userTank->destroyed && !g->userTank->isHit){
        move(false, Ucollided, g->userTank);
    }else{
        // NOTE:
        if(g->userTank->destroyed){
            // NOTE: The SDL_GetTicks() give the current time output
            // So how to calculate right spawn time every time user Tank
            // is hit
            respawn(g->userTank);
        };
    }
    
    for (int i = 0; i < TOTAL_ENEMY_TANK; i++){
        // NOTE: a Sole Ecollided is not enough
        // Rewrite the collision check and moving system
        
        Ucollided = BiTankCheck(&g->enemyTank[i], g->userTank);

        // ===============================================================
        // for (int p = 0; p < i; p++){
        //     if (i > 1 && p < i){

        //         Ecollided = checkCollision(&g->enemyTank[i].mBox, &g->enemyTank[p].mBox) | checkCollision(&g->userTank->mBox, &g->enemyTank[i].mBox);
                
        //         // printf(Ecollided?"EnemyTank collide each other in minor loop\n":"No collision detected\n");                            
        //     }
        //     // littleGuide(&g->enemyTank[i], g->userTank, collided);
        //     // move(false, collided, &g->enemyTank[p]);
        // }
        //========================================================================

        // NOTE: AI ways
        for (int j = i+1; j < TOTAL_ENEMY_TANK; j++){

            Ecollided = checkCollision(&g->enemyTank[i].mBox, &g->enemyTank[j].mBox) | checkCollision(&g->userTank->mBox, &g->enemyTank[i].mBox);
                
                // printf(Ecollided?"EnemyTank collide each other in minor loop\n":"No collision detected\n");                            
            }
            // littleGuide(&g->enemyTank[i], g->userTank, collided);
            // NOTE: BUG lies inside this fx
            // add the solution to the specific colliding case 
        littleGuide(&g->enemyTank[i], g->userTank, Ecollided);

        // NOTE: Temporary not use touchwall here
        if(!g->enemyTank[i].destroyed){
            move(false, Ecollided, &g->enemyTank[i]);
        }
    // ===============================================
    

        // printf(Ucollided?"EnemyTank collide each other out of minor loop\n":"No collision detected\n");

    }    
    setCamera(camera, g->userTank);        
}

void Update(Game* g){
    // NOTE: STILL IN WORK HERE
    // The fms loop through state is right here
    // Still haven't figure out how to change state right here

    // NOTE: Put the fsm here to loop through states change
    // Do I need to jam all stuffs into game : update, render, input process
    // printf("Start update game based on state \n");
    if (g->state != EMPTY){        
        switch (g->state){
            case MENU_INIT:
                // Duplicated code here??
                displayMenu(g);
                break;

            case PAUSE:
                displayMenu(g);
                break;

            case GAME_NEW:
                // NOTE: resetGame() contain alot of bugs here!!!!
                // because we constantly delete and create new game objects
                // in game loop 
                // How to tell apart which come from menu_init
                //================================================
                runMainScene(g);
                break;

            case GAME_RELOADED:
               // STORE ALL the Game stats here(already in game var)
                runMainScene(g);
                break;
        }   
    }
}

void resetGame(Game*g){
    // Memory allocated using new must be freed by delete
    g->chosen_option = NONE;

    StartTime = 0.0f;
    EndTime = 0.0f;
    TimeElapsed = 0.0f;

    StartTime = SDL_GetTicks();

    for(int i = 0; i < 5; i++){
        frame[i] = 0;
    };
    
    delete []g->enemyTank;
    g->enemyTank = nullptr;
    g->enemyTank = new TankInfo[TOTAL_ENEMY_TANK];

    delete g->userTank;
    g->userTank = nullptr;
    g->userTank = new TankInfo(true);

    delete []g->TankPos;
    g->TankPos = nullptr;
    g->TankPos = new Position[TOTAL_ENEMY_TANK];
    

    InitializeTankPos(g->TankPos);
            
    for (int i = 0 ; i < TOTAL_ENEMY_TANK; i++){
        g->enemyTank[i] = InitializeTankInfo(g->TankPos[i].x, g->TankPos[i].y);
    };

}        

void RenderMainScene(Game* g){
    EndTime = SDL_GetTicks();
    int k = 0;
 //Clear screen
     // NOTE: Render main scene
 for( int i = 0; i < TOTAL_TILES; ++i )
 {
     //touchesWall(&userTank->mBox, tileSet)
     renderTile( camera, g->Platform->gRenderer, g->tileSet[ i ], g->Platform->gTileTexture,  g->Platform->gTileClips, false);
     // tileSet[ i ]->render( camera, Platform->GetRenderer(),  Platform->GetgTileTexture(),  Platform->GetgTileClips(), checkCollision(&camera, tileSet[ i ]->getBox()));
 }

 if(!g->userTank->isHit){
     renderTank(g->userTank, Uframe, camera, g->Platform);
 }else{
     // The additional loop just make the explostion clip run incredibly faster
     // I didn't understand the game loop up until now
     // Just need the checking cycle for that explosion effect
     if(g->userTank->isHit && !g->userTank->destroyed){
         if(frame!=nullptr){
             renderExplosionFrame(g->userTank, g->Platform, &camera, frame, 4);
         }
     }     
 }
 k = 0;
 while(k < TOTAL_ENEMY_TANK)
 {
     if(!g->enemyTank[k].isHit){
         renderTank(&g->enemyTank[k], frame[k], camera, g-> Platform);                        
     } else {
     if(g->enemyTank[k].isHit && !g->enemyTank[k].destroyed){
         if(frame!=nullptr){
             renderExplosionFrame(&g->enemyTank[k], g->Platform, &camera, frame, k);              
         }
     }
     }
     k++;
 };
                                   
 renderText(FPS, g->userTank, g->Platform);
 TimeElapsed = EndTime - StartTime;
 FPS = 1/(TimeElapsed/1000.0f);
 StartTime = EndTime;         
}


void Close(Game* g){
    delete []frame;
    frame = nullptr;
    
    delete[] g->TankPos;
    g->TankPos = nullptr;
    
    delete[] g->enemyTank;
    g->enemyTank = nullptr;
    
    delete g->userTank;
    g->userTank = nullptr;
    
    delete[] g->tileSet;
    g->tileSet = nullptr;

    
    // NOTE: Still leak memmory????
    // =====================
    
// NOTE: I think I see the problem now. I delete platform before I properly
    // close everything in it
    // delete g->tileSet;

    SDL_DestroyRenderer(g->Platform->gRenderer);
    
    if(g->Platform->gRenderer != nullptr){
        g->Platform->gRenderer = nullptr;
    }

    SDL_DestroyWindow(g->Platform->gWindow);
    if(	g->Platform->gWindow != nullptr){        
        g->Platform->gWindow = nullptr;

        // g->Platform->gWindow == nullptr?printf("Window is destroyed\n"):printf("Window is not destroyed yet. why??");
    }

    close(g->Platform);        
    delete g->Platform;
    g->Platform = nullptr;


	//Quit SDL subsystems
    TTF_Quit();
	IMG_Quit();
	SDL_Quit();

}


void Render (Game* g){

    if (g->state != EMPTY){
        SDL_RenderClear( g->Platform->gRenderer);
        SDL_SetRenderDrawColor( g->Platform->gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
        // printf("Render accordingly to state\n");    
        if (g->state == MENU_INIT || g->state == PAUSE){
            // NOTE: The render part is already in the menu fx. So what will I put in this one???
            displayMenu(g);
        } else if (g->state == GAME_NEW || g->state == GAME_RELOADED){

            RenderMainScene(g);
            
        }
        SDL_RenderPresent( g->Platform->gRenderer);        
        // NOTE: Else do nothing
    } else {
        //??? WHY LOOP ONLY END AFTER I CALL SDL_QUIT IT IN GAME LOOP
        // Close(g);
        printf("End of Game, Thanks so much for playing my game\n");
    }
    
}
