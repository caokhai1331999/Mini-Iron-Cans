/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include <Gameh.h>
// NOTE: Fixed the menu issue
// NOTE: *= is a dangerous operator. If I don't know exactly what it will bring
// void resize(int* w, int* h, g->platformlatformP* p){
//     *w = default_w*(g->platform->screen_w/DEFAULT_SCREEN_WIDTH);
//     *h = default_h*(g->platform->screen_h/DEFAULT_SCREEN_HEIGHT);
// }

#if _WIN32
#include "windows.h"
BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,  // handle to DLL module
    DWORD fdwReason,     // reason for calling function
    LPVOID lpReserved )  // reserved
{
    return (TRUE);  // Successful DLL_PROCESS_ATTACH.
}
#endif

// Exported function
extern "C" __declspec(dllexport) void GameFunction() {
    std::cout << "Game code exported!\n";
}

void displayMenu(Game* g){

    // printf("Load Game menu\n");
    
    float scaleW = g->platform->screen_w/DEFAULT_SCREEN_WIDTH;
    float scaleH = g->platform->screen_h/DEFAULT_SCREEN_HEIGHT;
    
    int wD = 0;
    int hD = 0;
    for(uint8_t i = 0; i < 4; ++i){

        // NOTE: Scale up the text whenever it is pointed to
        if(g->pointed_option == i+1){
            if (scaleH == g->platform->screen_w/DEFAULT_SCREEN_WIDTH
                ,scaleW == g->platform->screen_w/DEFAULT_SCREEN_WIDTH){                
                scaleH *= 1.5f;   
                scaleW *= 1.5f;   
            }
        } else {
            if (scaleH > g->platform->screen_w/DEFAULT_SCREEN_WIDTH
                ,scaleW > g->platform->screen_w/DEFAULT_SCREEN_WIDTH){                
                scaleH /= 1.5f;   
                scaleW /= 1.5f;   
            }            
        }
            
        if (!loadFromRenderedText(Menu[i], scaleW, scaleH, g->platform->TextColor, g->platform->gFont, g->platform->gRenderer, g->platform->gMenuTexture)) {
            printf( "Can not Load Text to render! SDL Error: %s\n", SDL_GetError() );
        } else{
            // How to exactly put these sprite betweeen the screen
            wD = (int) ((0.2f)*(g->platform->screen_w));
            hD = (int) ((i*((0.2f)*(g->platform->screen_h))));
            
            // printf("Screen Width is :%d\n", g->platform->screen_w);
            // printf("SCreen Height is :%d\n", g->platform->screen_h);
            render(g->platform->gRenderer, wD, hD, g->platform->gMenuTexture);
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
                    }
                }
                break;                                 
        }
    }
}

bool Start(Game* g){
        if(!init(g->platform)){
            return false;
        }else{
                
            if( !LoadMedia(g->tileSet, g->platform) )
            {
                printf( "Failed to load media!\n" );
                return false;
            }
            else
            {
                GenerateSinglePosition(&g->userTank->mBox.x, &g->userTank->mBox.y);
                // printf("user Tank position is: %d %d", g->userTank->mBox.x, g->userTank->mBox.y);
                
                InitializeTankPos(g->TankPos);
                InitializeTankInfo(g->TankPos, g->enemyTank);
                
                //Level camera
                camera = { 0, 0, g->platform->screen_w, g->platform->screen_h};

                if(ExplosionFrame != nullptr){
                    delete []ExplosionFrame;
                    ExplosionFrame = nullptr;
                }

                ExplosionFrame = new uint8_t[5]();

                for(int i = 0; i < 5; i++){
                    ExplosionFrame[i] = 0;
                };

                MovingFrame = new uint8_t[5]();

                for(int i = 0; i < 5; i++){
                    MovingFrame[i] = 0;
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
                    g->state = GAME_RELOADED;
                    // Game Constantly reset 
                    g->chosen_option = NONE;
                    if(!g->stateChange){
                        g->stateChange = CHANGED;
                    }
                } else if (g->chosen_option == EXIT || key->type == SDL_QUIT){
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
                    if(!g->stateChange){
                        g->stateChange = CHANGED;
                    }
                }else if (g->chosen_option == RESUME){
                    g->state = GAME_RELOADED;
                    g->chosen_option = NONE;
                    if(!g->stateChange){
                        g->stateChange = CHANGED;
                    }
                }else if (g->chosen_option == EXIT || key->type == SDL_QUIT){
                    g->state = EMPTY;
                };                

                break;
                
            case GAME_NEW:
                if (key->key == SDL_SCANCODE_ESCAPE && key->pressed){
                    g->state = PAUSE;
                    if(!g->stateChange){
                        g->stateChange = CHANGED;
                    }
                } else if (key->type == SDL_QUIT){
                    g->state = EMPTY;
                }

                break;

            case GAME_RELOADED:
                if (key->key == SDL_SCANCODE_ESCAPE && key->pressed){
                    g->state = PAUSE;
                    if(!g->stateChange){
                        g->stateChange = CHANGED;
                    }
                } else if (key->type == SDL_QUIT){
                    g->state = EMPTY;
                }
                break;
        }            
    }
}

void ProcessInput(Game* g, bool* done){
    // printf("Start process input \n");
    SDL_Event e = {};
    SDL_zero(e);
    
    KeyState PreviousBut = {};
    KeyState CurrentBut = {};
    while( SDL_PollEvent(&e) != 0 )
    {
        //User requests quit
        // Modulize this part to reuse it
        //===================================================
        if(e.type == SDL_WINDOWEVENT)
        {
            if(e.window.event == SDL_WINDOWEVENT_RESIZED){                
                SDL_GetWindowSize(g->platform->gWindow, &g->platform->screen_w, &g->platform->screen_h);
                // printf("Screen width and height now are:%d %d\n", g->platform->screen_w, g->platform->screen_h);
            }
        }
        
        // NOTE: If I have time try to apply FSM to this input filter
        if ((e.key.state == SDL_PRESSED || e.key.state == SDL_RELEASED) && e.key.keysym.scancode != SDL_SCANCODE_UNKNOWN || e.type == SDL_QUIT){                        
            // NOTE: Forgot to add SDL_QUIT to filter conditions
            if((!CurrentBut.pressed && e.key.state == SDL_PRESSED && CurrentBut.key != e.key.keysym.scancode) || (e.key.keysym.scancode == CurrentBut.key && CurrentBut.pressed != (e.key.state == SDL_PRESSED))|| CurrentBut.init == 0 || e.key.keysym.scancode == SDL_SCANCODE_SPACE)

                                
                if(CurrentBut.init == 0){
                    CurrentBut.init = 1;
                }
            
            PreviousBut = CurrentBut;
            CurrentBut.type = e.type;

            CurrentBut.pressed = (e.key.state == SDL_PRESSED);
            CurrentBut.key = e.key.keysym.scancode;
            CurrentBut.repeat = e.key.repeat;

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

void runMainScene(Game* g){

    if(!g->userTank->destroyed && !g->userTank->isHit){
        move(false, Ucollided, g->userTank);
    }else{
        if(g->userTank->destroyed){
            respawn(g->userTank);
        };
    }

    bool Ecollided = false;
    for (int i = 0; i < TOTAL_ENEMY_TANK; i++){
        // NOTE: a Sole Ecollided is not enough
        // Rewrite the collision check and moving system
        
        Ucollided = BiTankCheck(&g->enemyTank[i], g->userTank);

        // ===============================================================
        // for (int p = 0; p < i; p++){
        //     if (i > 1 && p < i){

        //         Ecollided = checkCollision(&g->enemyTank[i].mBox, &g->enemyTank[p].mBox) | checkCollision(g->userTank->mBox, &g->enemyTank[i].mBox);
                
        //         // printf(Ecollided?"EnemyTank collide each other in minor loop\n":"No collision detected\n");                            
        //     }
        //     // littleGuide(&g->enemyTank[i], g->userTank, collided);
        //     // move(false, collided, &g->enemyTank[p]);
        // }
        //========================================================================

        // NOTE: AI ways
        for (int j = i+1; j < TOTAL_ENEMY_TANK; j++){

            Ecollided = checkCollision(&g->enemyTank[i].mBox, &(g->enemyTank[j].mBox));


        // NOTE: Temporary not use touchwall here
        if(!g->enemyTank[i].isHit && !g->enemyTank[i].destroyed){
            move(false, Ecollided, &g->enemyTank[i]);
        }
        littleGuide(&g->enemyTank[i], g->userTank, Ecollided);
    // ===============================================
    }    
    setCamera(camera, g->userTank);        
}
}

void Update(Game* g){
    if (g->state != EMPTY){        
        switch (g->state){
            case MENU_INIT:
                // Duplicated code here??
                // displayMenu(p, g);
                break;

            case PAUSE:
                // displayMenu(p, g);
                break;

            case GAME_NEW:
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
    g->pointed_option = 0;

    StartTime = 0.0f;
    EndTime = 0.0f;
    TimeElapsed = 0.0f;

    StartTime = SDL_GetTicks();

    for(int i = 0; i < 5; i++){
       MovingFrame[i] = 0;
    };

    for(int i = 0; i < 5; i++){
       ExplosionFrame[i] = 0;
    };    

    for(int i = 0; i < TOTAL_ENEMY_TANK; i++){
        resetTank(&g->enemyTank[i]);
    }

    resetTank(g->userTank);
    GenerateSinglePosition(&g->userTank->mBox.x, &g->userTank->mBox.y);

    InitializeTankPos(g->TankPos);    
    InitializeTankInfo(g->TankPos, g->enemyTank);

}        

void RenderMainScene(Game* g){
    EndTime = SDL_GetTicks();
    int k = 0;
 //Clear screen
     // NOTE: Render main scene
 for( int i = 0; i < TOTAL_TILES; ++i )
 {
     // resize(&g->tileSet[i].mBox.w, g->tileSet[i].mBox.h, g->platform);
     //touchesWall(&userTank->mBox, tileSet)
     renderTile( camera, g->platform->gRenderer, g->tileSet[ i ], g->platform->gTileTexture,  g->platform->gTileClips, false);
 }

 if(!g->userTank->isHit){
     // resize(&g->userTank.mBox.w, &g->userTank->mBox.h, g->platform);     
     renderTank(g->userTank, &MovingFrame[4], camera, g->platform);
 }else{
     // The additional loop just make the explostion clip run incredibly faster
     // I didn't understand the game loop up until now
     // Just need the checking cycle for that explosion effect
     if(g->userTank->isHit && !g->userTank->destroyed){
         if(ExplosionFrame!=nullptr){
             renderExplosionFrame(g->userTank, g->platform, &camera, ExplosionFrame, 4);
         }
     }     
 }

 k = 0;
 while(k < TOTAL_ENEMY_TANK)
 {
     // resize(&g->enemyTank[k].mBox.w, &g->enemyTank[k].mBox.h, g->platform);
     if(!g->enemyTank[k].isHit && !g->enemyTank[k].destroyed){
         renderTank(&g->enemyTank[k], &MovingFrame[k], camera, g->platform);                        
     } else {
     if(g->enemyTank[k].isHit && !g->enemyTank[k].destroyed){
         if(ExplosionFrame!=nullptr){
             renderExplosionFrame(&g->enemyTank[k], g->platform, &camera, ExplosionFrame, k);              
         }
     }
     }
     k++;
 };
                                   
 renderText(FPS, g->userTank, g->platform);
 TimeElapsed = EndTime - StartTime;
 FPS = 1/(TimeElapsed/1000.0f);
 StartTime = EndTime;         
}


void Close(Game* g){
    delete []ExplosionFrame;
    ExplosionFrame = nullptr;    

    delete []MovingFrame;
    MovingFrame = nullptr;    

    delete[] g->TankPos;
    g->TankPos = nullptr;

    delete[] g->userTank->Bullets;
    g->userTank->Bullets = nullptr;
    delete g->userTank;
    g->userTank = nullptr;

    for(int i = 0; i < TOTAL_ENEMY_TANK; i++){
        delete[] g->enemyTank[i].Bullets;
        g->enemyTank[i].Bullets = nullptr;      
    }

    delete[] g->enemyTank;
    g->enemyTank = nullptr;
    
    delete[] g->tileSet;
    g->tileSet = NULL;
    
    close(g->platform);        
}


void Render (Game* g){

    if (g->state != EMPTY){
        SDL_RenderClear( g->platform->gRenderer);
        SDL_SetRenderDrawColor( g->platform->gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
        // printf("Render accordingly to state\n");
        if(camera.w != g->platform->screen_w){
            camera.w = g->platform->screen_w;
        }

        if(camera.h != g->platform->screen_h){
            camera.h = g->platform->screen_h;
        }

        if (g->state == MENU_INIT || g->state == PAUSE){

            if (g->stateChange == CHANGED){
                g->stateChange = NOT_YET;
                SDL_SetWindowSize(g->platform->gWindow, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);                
                SDL_SetWindowPosition(g->platform->gWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
                SDL_GetWindowSize(g->platform->gWindow, &g->platform->screen_w, &g->platform->screen_h);
            }
            
            // NOTE: The render part is already in the menu fx. So what will I put in this one???
            displayMenu(g);
        } else if (g->state == GAME_NEW || g->state == GAME_RELOADED){

            if (g->stateChange == CHANGED){
                SDL_SetWindowSize(g->platform->gWindow, 2 * DEFAULT_SCREEN_WIDTH, 2*DEFAULT_SCREEN_HEIGHT);
                SDL_SetWindowPosition(g->platform->gWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
                g->stateChange = NOT_YET;
                SDL_GetWindowSize(g->platform->gWindow, &g->platform->screen_w, &g->platform->screen_h);
            }
            RenderMainScene(g);

            // NOTE: Set the position too
        }
        SDL_RenderPresent( g->platform->gRenderer);        
        // NOTE: Else do nothing
    }    
}
