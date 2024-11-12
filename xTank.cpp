/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include "xTank.h"

//Takes key presses and adjusts the Tank's velocity
// Move this function to PlatForm one

Position GeneratePosition(){
    Position Tem;
    Tem.X = std::rand()%(LEVEL_WIDTH);
    Tem.Y = std::rand()%(LEVEL_HEIGHT);
    return Tem ;
}


Position* InitializeTankPos(){
    bool valid = true;
    std::srand(std::time(nullptr));
    Position* TankPos = new Position[TOTAL_ENEMY_TANK];
    TankPos[0] = GeneratePosition();
    Position* TempPos = new Position;
    for (int i= 1; i < TOTAL_ENEMY_TANK; i++){
        // Loop comparing newly created pos to the previous valid one
        valid = false;
        while (!valid) {
            *TempPos = GeneratePosition();
            for (int p = 0; p < i; p++) {
                if (((TempPos->X - TankPos[p].X) > ( 200)) || (abs(TempPos->Y - TankPos[p].Y) > (150))) {
                    if ( i - p > 1) {
                        continue;
                    } else {
                        valid = true;
                    }
                } else {
                    break;
                }                
            }
        }
        TankPos[i] = *TempPos;
    }

    delete TempPos;
    return TankPos;
}                                 

TankInfo InitializeTankInfo(int x, int y){
    TankInfo Tank;
    // TODO: Mark the Tank bullet
    Tank.mBox.x = x;
    Tank.mBox.y = y;
    return Tank;
}

void fire(TankInfo* Tank){
    // TODO: Initialize bullet and send it fly, send signal when hit

    if(!Tank->destroyed) {
        for(int i = 0; i < TOTAL_BULLET_PER_TANK - 1; i++){
            printf("Looping through tank bullets\n");
            if(Tank->Bullets[i].Launched)
            {
                continue;
            } else {

                // TODO: Fix bugs right here
                printf("Bullet chosen\n");
                Tank->BulletsNumber -- ;
                Tank->Bullets[i].Launched = true;
                
                switch((int)Tank->face)
                {
                    printf("Give bullet veclocity\n");                    
                    case (int)UP:
                        Tank->Bullets[i].blBox.x = Tank->mBox.x + Tank->mBox.w/2 + 15;
                        Tank->Bullets[i].blBox.y = Tank->mBox.y + 5;
                        Tank->Bullets[i].BlVelY -= BULLET_VEL;
                        break; 
                    case (int)DOWN:
                        Tank->Bullets[i].blBox.x = Tank->mBox.x + Tank->mBox.w/2 + 15;
                        Tank->Bullets[i].blBox.y = Tank->mBox.y + Tank->mBox.h + 5;
                        Tank->Bullets[i].BlVelY += BULLET_VEL;
                        break; 
                    case (int)RIGHT:
                        Tank->Bullets[i].blBox.x = Tank->mBox.x + Tank->mBox.w + 5;
                        Tank->Bullets[i].blBox.y = Tank->mBox.y + Tank->mBox.h/2 + 15;
                        Tank->Bullets[i].BlVelX += BULLET_VEL;
                        break; 
                    case (int)LEFT:
                        Tank->Bullets[i].blBox.x = Tank->mBox.x + 5;
                        Tank->Bullets[i].blBox.y = Tank->mBox.y + Tank->mBox.h/2 + 15;
                        Tank->Bullets[i].BlVelX -= BULLET_VEL;
                        break;             
                }
                break; 
                // printf("Out of loop\n");                
            }
        }
    }
            // printf("End function\n");    
}

void handleEvent(KeyState* CurrentBut, TankInfo* Tank) {
    //If a key was pressed
    if (Tank->userBelong) {
         if( CurrentBut->pressed && CurrentBut->repeat == 0)
        {
            //Adjust the velocity
                switch( CurrentBut->key )
                {
                    // NOTE: Solved conflicted moving input by setting
                    // Tank Vel = 0 everytime before assign mVelX/Y to new Vel
                    // to bind it to equal to Tank vel or 0
                    case SDL_SCANCODE_UP: Tank->face = UP;
                        Tank->mVelY = 0; 
                        Tank->mVelY = -TANK_VEL;
                        // printf("TANK FACE is %f \n", Tank->face);
                        // printf("Button %d is being pressed\n", CurrentBut->key);
                        break;
                        
                        case SDL_SCANCODE_DOWN: Tank->face = DOWN;
                            Tank->mVelY = 0; 
                            Tank->mVelY += TANK_VEL;
                            // printf("TANK FACE is %f \n", Tank->face);
                            // printf("Button %d is being pressed\n", CurrentBut->key);                       
                        break;
                        case SDL_SCANCODE_RIGHT:
                            Tank->face = RIGHT;
                            Tank->mVelX = 0;                             
                            Tank->mVelX += TANK_VEL;
                            // printf("TANK FACE is %f \n", Tank->face);                            
                            // printf("Button %d is being pressed\n", CurrentBut->key);                       
                        break;
                        case SDL_SCANCODE_LEFT: Tank->face = LEFT;
                            Tank->mVelX = 0;                             
                            Tank->mVelX -= TANK_VEL;
                            // printf("TANK FACE is %f \n", Tank->face);
                            // printf("Button %d being pressed\n", CurrentBut->key);
                        break;

                        // TODO: Fix the bullet not showed
                    case SDL_SCANCODE_SPACE: fire(Tank);
                        // printf("Button space being pressed button%d \n", CurrentBut->key);
                        break;
                }
        }  else if( !CurrentBut->pressed && CurrentBut->repeat == 0)
        {
            //Adjust the velocity
            switch(CurrentBut->key )
            {
            // printf("Tank is moving up\n", CurrentBut->key);
                case SDL_SCANCODE_UP:
                    Tank->mVelY += TANK_VEL;
                    break;
                            printf("Button %d is released\n", CurrentBut->key);                                           
                                           
                case SDL_SCANCODE_DOWN:
                    Tank->mVelY -= TANK_VEL;
                    printf("Button %d is released\n", CurrentBut->key);
                    break;
                case SDL_SCANCODE_LEFT:
                    Tank->mVelX += TANK_VEL;
                    printf("Button %d is released\n", CurrentBut->key);                                                               
                    break;
                case SDL_SCANCODE_RIGHT:
                    Tank->mVelX -= TANK_VEL;
                    printf("Button %d is released\n", CurrentBut->key);                                                               
                    break;
            }
        }
    }
}

//Moves the Tank and check collision against tiles
void move( Tile *tiles[], bool touchesWall, TankInfo* Tank) {

    // NOTE: Apply dijkstra's algo here to make bot tank'move more smart

    // checkCollision(Tank->mBox, all current active bullets boxes)
   
    if(!Tank->destroyed){
        // if (Tank->userBelong){
            // And swivel the tank by the way
            //||touchesWall
            Tank->mBox.x += Tank->mVelX;
            if ((Tank->mBox.x < 0)||(Tank->mBox.x + Tank->mBox.w > LEVEL_WIDTH - 50)){
                Tank->mBox.x -= Tank->mVelX;
                // Tank stuck at the boundaries because of this line of code
                // How to solve this ??? TODO: Make the bot tanks be able to move
                // backward whenever it hit the obstacles
            }
    
            Tank->mBox.y += Tank->mVelY;
            if ((Tank->mBox.y < 0)||(Tank->mBox.y + Tank->mBox.h > LEVEL_HEIGHT - 50)){
                Tank->mBox.y -= Tank->mVelY;
            }
            
             // Move the bullet
            for(int i = 0 ; i < TOTAL_BULLET_PER_TANK; i++){
                                // TODO: Constantly push the bullet right after the being fired
                if (Tank->Bullets[i].Launched){

                    Tank->Bullets[i].blBox.x += Tank->Bullets[i].BlVelX;
                    if ((Tank->Bullets[i].blBox.x < 0)||(Tank->Bullets[i].blBox.x + Tank->Bullets[i].blBox.w > LEVEL_WIDTH)){
                        printf(" Bullet %d hit the boundaries and being destroyed\n", i);
                        Tank->Bullets[i].Launched = false;
                        Tank->Bullets[i].BlVelX = 0;
                        Tank->Bullets[i].BlVelY = 0;
                        Tank->BulletsNumber ++;
                        if (Tank->BulletsNumber > TOTAL_BULLET_PER_TANK){
                            Tank->BulletsNumber = TOTAL_BULLET_PER_TANK;
                        }
                    }
                    
                    Tank->Bullets[i].blBox.y += Tank->Bullets[i].BlVelY;
                    // printf("Bullet position is [%d %d]\n", Tank->Bullets[i].blBox.x, Tank->Bullets[i].blBox.y);
                    if ((Tank->Bullets[i].blBox.y < 0)||(Tank->Bullets[i].blBox.y + Tank->Bullets[i].blBox.h > LEVEL_HEIGHT)){
                        printf(" Bullet %d hit the boundaries and being destroyed\n", i);                                                
                        Tank->Bullets[i].Launched = false;
                        Tank->Bullets[i].BlVelX = 0;
                        Tank->Bullets[i].BlVelY = 0;
                        Tank->BulletsNumber ++;
                        if (Tank->BulletsNumber > TOTAL_BULLET_PER_TANK){
                            Tank->BulletsNumber = TOTAL_BULLET_PER_TANK;
                        }                        
                    }                    
                }
            // }
        }    
    // for (int i = 0; i < TOTAL_ENEMY_TANK; i++) {

    //     // TODO: How to properly check moving bullets DONE!

    //     // TODO: Figure out how to constantly check whether it hit any object on the way it move
    //     // Create aloop collision check for all bullets and tanks in Game    
    //     for (int j = 0; j < TOTAL_BULLET_PER_TANK; j++){    
    //         EnemyTank[i]->Bullets[j]->blBox.x += EnemyTank[i]->Bullets[j]->BlVel;    
    //         if ((EnemyTank[i]->Bullets[j]->mBox.x < 0)||(EnemyTank[i]->Bullets[j]->mBox.x + EnemyTank[i]->Bullets[j]->mBox.w>LEVEL_WIDTH)||checkCollision(EnemyTank[i]->Bullet[i]->mBox, )||(checkCollision())){
    //             EnemyTank[i]->Bullets[j]->destroyed = true;
    //             EnemyTank[i]->Bullets[j]->Launched = false;
    //         }

    //         EnemyTank[i]->Bullets[j]->blBox.y += EnemyTank[i]->Bullets[j]->BlVel;
    
    //         if ((EnemyTank[i]->Bullets[j]->mBox.y < 0)||(EnemyTank[i]->Bullets[j]->mBox.x + EnemyTank[i]->Bullets[j]->mBox.h > LEVEL_HEIGHT) || ){
    //             EnemyTank[i]->Bullets[j]->destroyed = true;
    //             EnemyTank[i]->Bullets[j]->Launched = false;            
    //         }        
    //     }                
    // }
}
}

//Centers the camera over the Tank
void setCamera( SDL_Rect& camera, TankInfo* UserTank ){
    // Initialize the collision box

	camera.x = ( UserTank->mBox.x + TANK_WIDTH / 2 ) - camera.w/2;
	camera.y = ( UserTank->mBox.y + TANK_HEIGHT / 2 ) - camera.h/2;    
        
	//Keep the camera in bounds
	if( camera.x < 0 )
	{ 
		camera.x = 0;
	}
	if( camera.y < 0 )
	{
		camera.y = 0;
	}
	if( camera.x > LEVEL_WIDTH - camera.w )
	{
		camera.x = LEVEL_WIDTH - camera.w;
	}
	if( camera.y > LEVEL_HEIGHT - camera.h )
	{
		camera.y = LEVEL_HEIGHT - camera.h;
	}
    
}

void littleGuide(TankInfo* botTank, TankInfo* UserTank){
    // TODO: This function is a little AI that use Dijktra algorithm to drive every
    // bot tank

    int distance = sqrt(pow(botTank->mBox.x - UserTank->mBox.x,2) + pow(botTank->mBox.y - UserTank->mBox.y,2));

    // NOTE: The idea is simple: moving the bot Tank toward User's one and fire
    // when it is near
    // Prioritize the shorter axis first to shoot if any 
    //
  
    if (distance > 250){
        if (abs(botTank->mBox.x - UserTank->mBox.x) < abs(botTank->mBox.y - UserTank->mBox.y)){
        // NOTE: Prioritize which axis is choose to move first
        if(botTank->mVelX != 0){
            botTank->mVelX = 0;            
        }
    if (botTank->mBox.x - UserTank->mBox.x < 250){
            if (botTank->face != LEFT){
            botTank->face = LEFT;
            }
            if(botTank->mVelY == 0){                
            botTank->mVelX -= TANK_VEL;
            }
        } else if (botTank->mBox.x - UserTank->mBox.x > - 250){
            if(botTank->face = RIGHT){
                botTank->face = RIGHT;
            }
            if(botTank->mVelY == 0){
            botTank->mVelX += TANK_VEL;
            }
        } else {
            botTank->mVelX = 0;            
        }        
    } else if (abs(botTank->mBox.x - UserTank->mBox.x) > abs(botTank->mBox.y - UserTank->mBox.y)) {
        
        if(botTank->mVelY != 0){
            botTank->mVelY = 0;            
        }        
        if (botTank->mBox.y - UserTank->mBox.y < 100){
            if(botTank->face != UP){                
            botTank->face = UP;
            }
            if (botTank->mVelX == 0)
            {
                botTank->mVelY -= TANK_VEL;
            }
        } else if (botTank->mBox.y - UserTank->mBox.y > - 100){
            if(botTank->face != DOWN){
            botTank->face = DOWN;                
            }
            if (botTank->mVelX == 0)            
            {
                botTank->mVelY += TANK_VEL;
            }
        } else {
            botTank->mVelY = 0;
        }   
    }
}        // NOTE: Seem Like I don't need to use complicate Dijktra algorigthm yet
    else {
            if (abs(botTank->mBox.x - UserTank->mBox.x) < TANK_WIDTH){
                        if (botTank->mBox.x - UserTank->mBox.x > 0){                            
                            if(botTank->face != LEFT){
                                botTank->face = LEFT;
                            }                            
                        } else if (botTank->mBox.x - UserTank->mBox.y < 0){
                            if(botTank->face != RIGHT){
                                botTank->face = RIGHT;
                            }                            
                        }
            } else if(abs(botTank->mBox.y - UserTank->mBox.y) < TANK_HEIGHT){
                        if(botTank->mBox.y - UserTank->mBox.y > 0){                            
                            if(botTank->face != UP){
                                botTank->face = UP;
                            }
                        }

                        else if(botTank->mBox.y - UserTank->mBox.y < 0){                            
                        if(botTank->face != DOWN ){
                                botTank->face =DOWN;
                            }
                        }
                    }
                        fire(botTank);
} 
}
