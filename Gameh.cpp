/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include <Gameh.h>
// NOTE: Fixed the menu issue
void displayMenu(PlatformP* p, Game* g){
    float scale = 1.0f;
    for(uint8_t i = 0; i < 4; ++i){

        // NOTE: Scale up the text whenever it is pointed to
        if(g->pointed_option == i+1){            
            scale = 1.5f;   
        } else {
            scale = 1.0f;
        }
            
        if (!loadFromRenderedText(Menu[i], scale, p->TextColor, p->gFont, p->gRenderer, &p->gMenuTexture)) {
            printf( "Can not Load Text to render! SDL Error: %s\n", SDL_GetError() );
        } else{
            render(p->gRenderer, 100, 30 + i*50, &p->gMenuTexture);
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
                        // g->chosen_option == NONE?printf("Option doesn't change after hitting enter yet. Why??\n"):printf("Options changed\n");
                        // g->chosen_option == EXIT?printf("Option is now exit\n"):printf("why option is not exit yet\n");
                    }
                }
                break;                                 
        }
    }
}

bool Start(PlatformP* p, Game* g){
        if(!init(p)){
            return false;
        }else{
                
            if( !LoadMedia(g->tileSet, p) )
            {
                printf( "Failed to load media!\n" );
                return false;
            }
            else
            {
                InitializeTankPos(g->TankPos);                
                for (int i = 0 ; i < TOTAL_ENEMY_TANK; i++){
                    g->enemyTank[i] = InitializeTankInfo(g->TankPos[i].x, g->TankPos[i].y, false);
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
    SDL_Event e = {};
    SDL_zero(e);
    
    KeyState PreviousBut = {};
    KeyState CurrentBut = {};
    while( SDL_PollEvent(&e) != 0 )
    {
        //User requests quit
        // Modulize this part to reuse it
        //===================================================

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
                            handleEventForTank(&CurrentBut, &g->userTank);
                        }
                        break;
                    case GAME_RELOADED:
                        if (CurrentBut.key != 41){
                            handleEventForTank(&CurrentBut, &g->userTank);
                        }
                        break;
                };
                changeState(g, &CurrentBut);
        }
    }                    
}

void runMainScene(Game* g){

    if(!g->userTank.destroyed && !g->userTank.isHit){
        move(false, Ucollided, &g->userTank);
    }else{
        // NOTE:
        if(&g->userTank.destroyed){
            // NOTE: The SDL_GetTicks() give the current time output
            // So how to calculate right spawn time every time user Tank
            // is hit
            respawn(&g->userTank);
        };
    }
    
    for (int i = 0; i < TOTAL_ENEMY_TANK; i++){
        // NOTE: a Sole Ecollided is not enough
        // Rewrite the collision check and moving system
        
        Ucollided = BiTankCheck(&g->enemyTank[i], &g->userTank);

        // ===============================================================
        // for (int p = 0; p < i; p++){
        //     if (i > 1 && p < i){

        //         Ecollided = checkCollision(&g->enemyTank[i].mBox, &g->enemyTank[p].mBox) | checkCollision(&g->userTank.mBox, &g->enemyTank[i].mBox);
                
        //         // printf(Ecollided?"EnemyTank collide each other in minor loop\n":"No collision detected\n");                            
        //     }
        //     // littleGuide(&g->enemyTank[i], &g->userTank, collided);
        //     // move(false, collided, &g->enemyTank[p]);
        // }
        //========================================================================

        // NOTE: AI ways
        for (int j = i+1; j < TOTAL_ENEMY_TANK; j++){

            Ecollided = checkCollision(&g->enemyTank[i].mBox, &(g->enemyTank[j].mBox)) | checkCollision(&g->userTank.mBox, &g->enemyTank[i].mBox);
                
                // printf(Ecollided?"EnemyTank collide each other in minor loop\n":"No collision detected\n");                            
            }
            // littleGuide(&g->enemyTank[i], &g->userTank, collided);
            // NOTE: BUG lies inside this fx
            // add the solution to the specific colliding case 
        littleGuide(&g->enemyTank[i], &g->userTank, Ecollided);

        // NOTE: Temporary not use touchwall here
        if(!g->enemyTank[i].destroyed){
            move(false, Ecollided, &g->enemyTank[i]);
        }
    // ===============================================
    

        // printf(Ucollided?"EnemyTank collide each other out of minor loop\n":"No collision detected\n");

    }    
    setCamera(camera, &g->userTank);        
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
                // displayMenu(p, g);
                break;

            case PAUSE:
                // displayMenu(p, g);
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
    

    for(int i = 0; i < TOTAL_ENEMY_TANK; i++){
        resetTank(&g->enemyTank[i]);
    }
    resetTank(&g->userTank);
    // delete []g->TankPos;
    // g->TankPos = nullptr;
    // g->TankPos = new Position[TOTAL_ENEMY_TANK];
    

    InitializeTankPos(g->TankPos);
            
    for (int i = 0 ; i < TOTAL_ENEMY_TANK; i++){
        g->enemyTank[i] = InitializeTankInfo(g->TankPos[i].x, g->TankPos[i].y);
    };

}        

void RenderMainScene(PlatformP* p, Game* g){
    EndTime = SDL_GetTicks();
    int k = 0;
 //Clear screen
     // NOTE: Render main scene
 for( int i = 0; i < TOTAL_TILES; ++i )
 {
     //touchesWall(&userTank.mBox, tileSet)
     renderTile( camera, p->gRenderer, g->tileSet[ i ], &p->gTileTexture,  p->gTileClips, false);
     // tileSet[ i ]->render( camera, Platform->GetRenderer(),  Platform->GetgTileTexture(),  Platform->GetgTileClips(), checkCollision(&camera, tileSet[ i ]->getBox()));
 }

 if(!g->userTank.isHit){
     renderTank(&g->userTank, Uframe, camera, p);
 }else{
     // The additional loop just make the explostion clip run incredibly faster
     // I didn't understand the game loop up until now
     // Just need the checking cycle for that explosion effect
     if(&g->userTank.isHit && !g->userTank.destroyed){
         if(frame!=nullptr){
             renderExplosionFrame(&g->userTank, p, &camera, frame, 4);
         }
     }     
 }

 k = 0;
 while(k < TOTAL_ENEMY_TANK)
 {
     if(!g->enemyTank[k].isHit && !g->enemyTank[k].destroyed){
         renderTank(&g->enemyTank[k], frame[k], camera, p);                        
     } else {
     if(g->enemyTank[k].isHit && !g->enemyTank[k].destroyed){
         if(frame!=nullptr){
             renderExplosionFrame(&g->enemyTank[k], p, &camera, frame, k);              
         }
     }
     }
     k++;
 };
                                   
 renderText(FPS, &g->userTank, p);
 TimeElapsed = EndTime - StartTime;
 FPS = 1/(TimeElapsed/1000.0f);
 StartTime = EndTime;         
}


void Close(PlatformP* p, Game* g){
    delete []frame;
    frame = nullptr;    
    
    // delete[] g->enemyTank;
    // g->enemyTank = nullptr;
    // free(g->enemyTank);
    // g->enemyTank = NULL;
    
    // delete[] g->userTank.Bullets;
    // g->userTank.Bullets = NULL;
    // free(&g->userTank.Bullets);
    // g->userTank.Bullets = NULL;
    
    // free(&g->userTank);
    // g->userTank = NULL;
    
    // free(g->tileSet);
    // g->tileSet = NULL;
    // delete[] g->tileSet;
    // g->tileSet = NULL;

    
    // NOTE: Still leak memmory????
    // =====================
    
// NOTE: I think I see the problem now. I delete platform before I properly
    // close everything in it
    // delete g->tileSet;

    close(p);        
        // p->gWindow == nullptr?printf("Window is destroyed\n"):printf("Window is not destroyed yet. why??");
}


void Render (PlatformP* p, Game* g){

    if (g->state != EMPTY){
        SDL_RenderClear( p->gRenderer);
        SDL_SetRenderDrawColor( p->gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
        // printf("Render accordingly to state\n");    
        if (g->state == MENU_INIT || g->state == PAUSE){
            // NOTE: The render part is already in the menu fx. So what will I put in this one???
            displayMenu(p, g);
        } else if (g->state == GAME_NEW || g->state == GAME_RELOADED){

            RenderMainScene(p, g);
            
        }
        SDL_RenderPresent( p->gRenderer);        
        // NOTE: Else do nothing
    } else {
        //??? WHY LOOP ONLY END AFTER I CALL SDL_QUIT IT IN GAME LOOP
        // Close(g);
        printf("End of Game, Thanks so much for playing my game\n");
    }    
}
