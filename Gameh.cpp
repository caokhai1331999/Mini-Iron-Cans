/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include "Gameh.h"

void displayMenu(Game* g){
    gFont = TTF_OpenFont( "Roboto-Variable.ttf", 28 );
    // SDL_Color TextColor = {249 ,166 ,2};
    SDL_Color TextColor = {0 ,0 ,0};
    float scale = 0.0f;

    for(uint8 i = 0; i < 3; ++i){

        // NOTE: Scale up the text whenever it is pointed to
        if((uint8)g->pointed_option == i){            
            scale = 1.5f;   
        } else {
            scale = 1.0f;
        }

        if (!g->Platform->gMenuTexture->loadFromRenderedText(Menu[i], scale, scale, TextColor, gFont, g->Platform->gRenderer)) {
            printf( "Can not Load Text to render! SDL Error: %s\n", SDL_GetError() );
        } else {                           g->Platform->gMenuTexture->render(g->Platform->gRenderer, SCREEN_WIDTH - 200, 30 + i*30);
        }                       
    }
    
}

void get_Menu_choice(Game* g, KeyState* currentKey){

    uint8_t cursor = 0;
    if(g->state == MENU_INIT || g->state == PAUSE){
        cursor = 1;
        if(currentKey->key == SDL_SCANCODE_DOWN && currentKey->pressed){
            cursor ++;
            g->pointed_option = (MENUCHOICE)cursor;
            if(g->pointed_option > MENUCHOICE(3)){
                cursor = 0;
                g->pointed_option = MENUCHOICE(cursor);
            };
        }

        if(currentKey->key == SDL_SCANCODE_KP_ENTER && currentKey->pressed)
        {            
            if(g->chosen_option != g->pointed_option)
            {
                g->chosen_option = g->pointed_option;
            }
        }
    }
    
};

bool Start(Game* g){
        if(!init(g->Platform)){
            return false;
        }else{
            if( ! LoadMedia(g->tileSet, g->Platform) )
            {
                printf( "Failed to load media!\n" );
            }
            else
            {
                g->TankPos = InitializeTankPos();
            
                for (int i = 0 ; i < TOTAL_ENEMY_TANK; i++){
                   g-> enemyTank[i] = InitializeTankInfo(g->TankPos[i].x, g->TankPos[i].y);
                }
            
                //Level camera
                camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
                Uframe = 0;
                frame[4] = {} ;
                StartTime =  SDL_GetTicks();

                StartTime = 0;
                EndTime = 0;
                TimeElapsed = 0;

                Ecollided = false;
                Ucollided = false;
                // uint32 LastFrameTime[TOTAL_ENEMY_TANK] ={};
                return true;
            }            
        }
}

void changeState(Game* g, KeyState* key, bool done){

    switch(g->state){
        case MENU_INIT:
            if (g->chosen_option == NEW_GAME){
                g->state = GAME_NEW; 
            } else if (g->chosen_option == EXIT){
                g->state = EMPTY;
            }
            break;
        case PAUSE:
            if(g->chosen_option == NEW_GAME){
                g->state = GAME_NEW;
            }else if (g->chosen_option == RESUME){
                g->state = GAME_RELOADED;
            }else if (g->chosen_option == EXIT){
                g->state = EMPTY;
            };
            break;
        case GAME_NEW:
            if (key->key == SDL_SCANCODE_ESCAPE && key->pressed){
                g->state = PAUSE;
            }
            break;

        case GAME_RELOADED:
            if (key->key == SDL_SCANCODE_ESCAPE && key->pressed){
                g->state = PAUSE;
            }
            break;
        case EMPTY: done = (done!=true)?true:false;
            break;
    }    
    
}

void ProcessInput(Game* g, bool done){
    printf("Start process input \n");
    KeyState PreviousBut = {};
    KeyState CurrentBut = {};
    while( SDL_PollEvent( &g->Platform->e ) != 0 )
    {
        //User requests quit
        if( g->Platform->e.type == SDL_QUIT || g->Platform->e.key.keysym.scancode == 41 )
        {
            done = true;
        }

        // Modulize this part to reuse it
        //===================================================

        // NOTE: If I have time try to apply FSM to this input filter
        if ((g->Platform->e.key.state == SDL_PRESSED || g->Platform->e.key.state == SDL_RELEASED) && g->Platform->e.key.keysym.scancode != SDL_SCANCODE_UNKNOWN){                        

            if((!CurrentBut.pressed && g->Platform->e.key.state == SDL_PRESSED && CurrentBut.key != g->Platform->e.key.keysym.scancode) || (g->Platform->e.key.keysym.scancode == CurrentBut.key && CurrentBut.pressed != (g->Platform->e.key.state == SDL_PRESSED))|| CurrentBut.init == 0 || g->Platform->e.key.keysym.scancode == SDL_SCANCODE_SPACE)

                                
                if(CurrentBut.init == 0){
                    CurrentBut.init = 1;
                }
            
            PreviousBut = CurrentBut;
            CurrentBut.pressed = (g->Platform->e.key.state == SDL_PRESSED);
            CurrentBut.key = g->Platform->e.key.keysym.scancode;
            CurrentBut.repeat = g->Platform->e.key.repeat;

            // NOTE: Consider removing this one
            // (This seemed kind of unproper)
            switch(g->state){
                case MENU_INIT: get_Menu_choice(g, &CurrentBut);
                    break;
                case PAUSE: get_Menu_choice(g, &CurrentBut);
                    break;
                case GAME_NEW:
                    handleEvent(&CurrentBut, g->userTank);
                    break;
                case GAME_RELOADED:
                    handleEvent(&CurrentBut, g->userTank);
                    break;
                case EMPTY:
                    break;
            };
            changeState(g, &CurrentBut, done);
        }
    }            
}

void runMainScene(Game* g){
    move( false, Ucollided, g->userTank);
    for (int i = 0; i < TOTAL_ENEMY_TANK; i++){
        BiTankCheck(&g->enemyTank[i], g->userTank);

        for (int p = 0; p < i; p++){
            if (i > 1 && p < i){

                Ecollided = checkCollision(&g->enemyTank[i].mBox, &g->enemyTank[p].mBox) | checkCollision(&g->userTank->mBox, &g->enemyTank[i].mBox);
                            
                // printf(Ecollided?"EnemyTank collide each other in minor loop\n":"No collision detected\n");                            
            }
            // littleGuide(&g->enemyTank[i], g->userTank, collided);
            // move(false, collided, &g->enemyTank[p]);
        }

        Ucollided = checkCollision(&g->userTank->mBox, &g->enemyTank[i].mBox);
        littleGuide(&g->enemyTank[i], g->userTank, Ecollided);

        // NOTE: Temporary not use touchwall here

        move(false, Ecollided, &g->enemyTank[i]);
        // printf(Ucollided?"EnemyTank collide each other out of minor loop\n":"No collision detected\n");

    }
                
    // NOTE: If user Tank is destroyed add spawnTank here                
    if(g->userTank->isHit){
        // NOTE: The SDL_GetTicks() give the current time output
        // So how to calculate right spawn time every time user Tank
        // is hit
        respawnTime = SDL_GetTicks();
        while (respawnTime - StartTime < 1000.0f){
            respawnTime = SDL_GetTicks();
        }
        if(respawnEndTime - respawnStartTime > 1000.0f){
            printf("Wait Time: %f\n", respawnTime - StartTime);
            respawn(g->userTank);
            respawnTime = StartTime;
        }                
    };

    setCamera(camera, g->userTank);        
}

void Update(Game* g){
    // NOTE: STILL IN WORK HERE
    // The fms loop through state is right here
    // Still haven't figure out how to change state right here

    // NOTE: Put the fsm here to loop through states change
    // Do I need to jam all stuffs into game : update, render, input process
    printf("Start update game based on state \n");
    switch (g->state){
        case MENU_INIT:
            displayMenu(g);
            break;

        case PAUSE:
            displayMenu(g);
            break;

        case GAME_NEW:
            // STORE ALL the Game stats here(already in game var)
            // How to ensure the memory safety here. Note that
            // the g is just the address of the game variable
            // how can I delete the whole struct and create new
            // or is there any way else???
            resetGame(g);
            runMainScene(g);
            break;

        case GAME_RELOADED:
            // STORE ALL the Game stats here(already in game var)
            runMainScene(g);
            break;
    }
    
}

void resetGame(Game*g){
    g->state = GAME_NEW;
    // Memory allocated using new must be freed by delete
    delete g->TankPos;
    delete g->userTank;
    delete g->enemyTank;
    
    g->TankPos = new Position;
    g->userTank = new TankInfo(true);
    g->enemyTank = new TankInfo[TOTAL_ENEMY_TANK];
    g->chosen_option = NONE;
}        

void RenderMainScene(Game* g){

    int k = 0;
 //Clear screen
 SDL_RenderClear( g->Platform->gRenderer);
 SDL_SetRenderDrawColor( g->Platform->gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
 if (g->state == GAME_NEW || g->state == GAME_RELOADED){
     // NOTE: Render main scene
 for( int i = 0; i < TOTAL_TILES; ++i )
 {
     //touchesWall(&userTank->mBox, tileSet)
     g->tileSet[ i ]->render( camera, g->Platform->gRenderer,  g->Platform->gTileTexture,  g->Platform->gTileClips, false);
     // tileSet[ i ]->render( camera, Platform->GetRenderer(),  Platform->GetgTileTexture(),  Platform->GetgTileClips(), checkCollision(&camera, tileSet[ i ]->getBox()));
 }
               
 render(g->userTank, Uframe, camera, g->Platform);
 // NOTE: Bugs lied here
 while(k < TOTAL_ENEMY_TANK)
 {
     if(g->enemyTank[k].isHit){
         if (!g->enemyTank[k].userBelong && !g->enemyTank[k].destroyed)
         {
             // NOTE: Because of the interupt of the loop caused
             // Frame scrolling not fast enough to make animation

             //But This one is like too fast for nake eye to see
             //the animation
                            
             //Cycle animation
             if (frame[k]/12 == ANIMATING_FRAMES+1){
                 resetTank(&g->enemyTank[k]);
                 frame[k] = -1;
             }
             //Go to next frame
             if(frame[k]!=-1){
                 g->Platform->gExplosionTexture->render( g->Platform->gRenderer ,(g->enemyTank[k].mBox.x - camera.x), (g->enemyTank[k].mBox.y - camera.y), &g->Platform->gExplosionClips[frame[k]]);
                 SDL_Delay(0.032);
                 frame[k]++;
             }

         }
     } else {
         render(&g->enemyTank[k], frame[k], camera, g->Platform);                        
     }
     if (frame[k] == -1 || !g->enemyTank[k].isHit){
         k++;
     }
 };
                                   
 renderText(FPS, g->userTank, g->Platform);
 SDL_RenderPresent( g->Platform->gRenderer);
 TimeElapsed = EndTime - StartTime;
 FPS = 1/(TimeElapsed/1000.0f);
 StartTime = EndTime;         
    } 
}

void Render (Game* g){
    printf("Render accordingly to stae\n");    
    if (g->state == MENU_INIT || g->state == PAUSE){
     // NOTE: The render part is already in the menu fx. So what will I put in this one???
        displayMenu(g);
    } else if (g->state == GAME_NEW || g->state == GAME_RELOADED){
        RenderMainScene(g);
    }
    // NOTE: Else do nothing
}

void Close (Game* g){
    delete g->TankPos;    
    delete g->Platform;
    delete g->userTank;
    delete[] g->enemyTank;
}
