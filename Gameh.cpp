/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */

void displayMenu(Game* g){
    
}

bool Start(Game* g){
    if(!g->platform.init()){
        return false;
    }else{
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

void ProcessInput(Game* g){
    KeyState PreviousBut = {};
    KeyState CurrentBut = {};
    while( SDL_PollEvent( &g->platform->e ) != 0 )
    {
        //User requests quit
        if( e.type == SDL_QUIT || e.key.keysym.scancode == 41 )
        {
            quit = true;
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
            handleEvent(&CurrentBut, g->userTank);                       
        }
    }
}    

void Update(Game* g){
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

void Render(Game* g){
                
 //Clear screen
 SDL_SetRenderDrawColor( Platform.gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
 SDL_RenderClear( Platform.gRenderer);
 
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
}
