/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */

void displayMenu(Game* g){
    gFont = TTF_OpenFont( "Roboto-Thin.ttf", 28 );
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

        if (!g->Platform.gMenuTexture->loadFromRenderedText(Menu[i], scale, scale, TextColor, gFont, Platform.gRenderer)) {
            printf( "Can not Load Text to render! SDL Error: %s\n", SDL_GetError() );
        } else {                           Platform.gMenuTexture->render(Platform.gRenderer, SCREEN_WIDTH - 200, 30 + i*30);
        }                       
    }
    
}

void get_Menu_choice(Game* g, KeyState* currentKey){

    if(g->state == MENU_IDLE || g->state == PAUSE){        
        if(currentKey->key == SDL_SCANCODE_DOWN && currentKey->pressed){
            g->pointed_option ++;
            if(g->pointed_option > MENUCHOICE(3)){
                g->pointed_option = MENUCHOICE(0);
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

void Menu(Game* g){
    // If the the current choice is that option and the input is Enter then we
    // proces accordingly
    // Time to jam this fx to update one
    while(currentChoice == NONE){
        displayMenu(g);
        get_Menu_choice(g);
        //NOTE: Still on working
        switch(g->chosen_option){
            case NEW_GAME: changeState(g); break; // Create game state from scratch
            case RESUME: changeState(g); break;   // 
            case OPTIONS: break;  // not decide yet temporary do nothing
            case EXIT: changeState(g); break;     // kill the game clean the asset
            default : g->chosen_option = NONE; break;
    }
}

bool Start(Game* g){
        if(!g->platform.init()){
            return false;
        }else{
            if( ! LoadMedia(tileSet) )
            {
                printf( "Failed to load media!\n" );
            }
            else
            {
                TankPos = InitializeTankPos();
            
                for (int i = 0 ; i < TOTAL_ENEMY_TANK; i++){
                    enemyTank[i] = InitializeTankInfo(TankPos[i].x, TankPos[i].y);
                }
            
                //Level camera
                SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
                int Uframe = 0;
                int testFrame = 0;
                int frame[4] = {} ;
                StartTime =  SDL_GetTicks();

                real32 Startframe = 0;
                real32 Endframe = 0;
                real32 FrameElapse = 0;

                bool Ecollided = false;
                bool Ucollided = false;
                // uint32 LastFrameTime[TOTAL_ENEMY_TANK] ={};
                int k;        
                return true;
            }            
        }
}

void changeState(Game* g, KeyState* key){
    //NOTE: Now WORK ON THIS
    // HOW!!!!
    switch(g->state){
        case MENU_IDLE: if(key.key == );break;
        case RESUME: g->state = IN_GAME;break;
        case EXIT: g->state = EMPTY;break;
    }
}

void ProcessInput(Game* g, bool done){
    KeyState PreviousBut = {};
    KeyState CurrentBut = {};
    while( SDL_PollEvent( &g->platform->e ) != 0 )
    {
        //User requests quit
        if( e.type == SDL_QUIT || e.key.keysym.scancode == 41 )
        {
            done = true;
        }

        // Modulize this part to reuse it
        //===================================================

        // NOTE: If I have time try to apply FSM to this input filter
        if ((e.key.state == SDL_PRESSED || e.key.state == SDL_RELEASED) && e.key.keysym.scancode != SDL_SCANCODE_UNKNOWN){                        

            if((!CurrentBut.pressed && e.key.state == SDL_PRESSED && CurrentBut.key != e.key.keysym.scancode) || (e.key.keysym.scancode == CurrentBut.key && CurrentBut.pressed != (e.key.state == SDL_PRESSED))|| CurrentBut.init == 0 || e.key.keysym.scancode == SDL_SCANCODE_SPACE)

                                
                if(CurrentBut.init == 0){
                    CurrentBut.init = 1;
                }
            
            PreviousBut = CurrentBut;
            CurrentBut.pressed = (e.key.state == SDL_PRESSED);
            CurrentBut.key = e.key.keysym.scancode;
            CurrentBut.repeat = e.key.repeat;


            switch(g->state){
                case MENU_IDLE: get_Menu_choice(g, &CurrentBut); changeState(g);break;
                case PAUSE: get_Menu_choice(g, &CurrentBut);changeState(g, &CurrentBut);break;
                case IN_GAME: changeState(g, &CurrentBut);handleEvent(&CurrentBut, g->userTank);break;
                case EMPTY: done = (done!=true)?true:false;break;
            }
               }
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
    changeState(g);
    switch (g->state){
        case MENU_IDLE:
            Menu(g);
            break;

        case IN_GAME:
            runMainScene(g);
            ProcessInput(g);
            break;

        case PAUSE:
            // STORE ALL the Game stats here(already in game var)
            Menu(g);
            ProcessInput(g);
            break;
    }
    
}

void Render(Game* g){

 //Clear screen
 SDL_RenderClear( Platform.gRenderer);
 SDL_SetRenderDrawColor( Platform.gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
 if (g->state == IN_GAME){
     // NOTE: Render main scene
 for( int i = 0; i < TOTAL_TILES; ++i )
 {
     //touchesWall(&userTank->mBox, tileSet)
     tileSet[ i ]->render( camera, Platform.gRenderer,  Platform.gTileTexture,  Platform.gTileClips, false);
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

             //But This one is like too fast for nake eye to see
             //the animation
                            
             //Cycle animation
             if (frame[k]/12 == ANIMATING_FRAMES+1){
                 resetTank(&enemyTank[k]);
                 frame[k] = -1;
             }
             //Go to next frame
             if(frame[k]!=-1){
                 Platform.gExplosionTexture->render( Platform.gRenderer ,(enemyTank[k].mBox.x - camera.x), (enemyTank[k].mBox.y - camera.y), &Platform.gExplosionClips[frame[k]]);
                 SDL_Delay(0.032);
                 frame[k]++;
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
 FPS = 1/(TimeElapsed/1000.0f);
 StartTime = EndTime;         
    } else if (g->state == MENU_IDLE || g->state == PAUSE){
     Platform.gMenuTexture->render(Platform.gRenderer, SCREEN_WIDTH - 200, 30 + i*30);        
    }
 
}

void close (Game* g){
    delete g->TankPos;    
    delete g->Platform;
    delete g->userTank;
    delete[] g->enemyTank;
}
