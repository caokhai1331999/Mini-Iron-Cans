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
    
    Tem.x = std::rand()%LEVEL_WIDTH;

    if(Tem.x > LEVEL_WIDTH - TANK_WIDTH){
        Tem.x = LEVEL_WIDTH - TANK_WIDTH;
    }
    Tem.y = std::rand()%LEVEL_HEIGHT;

    if(Tem.y > LEVEL_HEIGHT - TANK_HEIGHT){
        Tem.y = LEVEL_HEIGHT - TANK_HEIGHT;                        
    }
    return Tem ;
}


void InitializeTankPos(Position* RealTankPos){
    bool valid = true;
    std::srand(std::time(nullptr));
    RealTankPos[0] = GeneratePosition();
    Position* TempPos = new Position;
        // Loop comparing newly created pos to the previous valid one
    for (int i= 1; i < TOTAL_ENEMY_TANK; i++){
        valid = false;
        while (!valid) {
            *TempPos = GeneratePosition();
            for (int p = 0; p < i; p++) {
                if (((TempPos->x - RealTankPos[p].x) > ( 200)) || (abs(TempPos->y - RealTankPos[p].y) > (150))) {
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
        // printf("Position x: %d, y: %d", TempPos->x, TempPos->y);
        RealTankPos[i] = *TempPos;
    }

    delete TempPos;
}                                 

TankInfo InitializeTankInfo(int x, int y){
    TankInfo* Tank = nullptr;
    Tank = new TankInfo;
    // TODO: Mark the Tank bullet
    Tank->mBox.x = x;
    Tank->mBox.y = y;
    return *Tank;
    delete Tank;
}

void fire(TankInfo* Tank){
    // TODO: Initialize bullet and send it fly, send signal when hit
    // && !Tank->isHit
    if(!Tank->destroyed) {
            // if(Tank->userBelong){
            //     printf("Looping through tank bullets\n");
            // }
        for(int i = 0; i < TOTAL_BULLET_PER_TANK ; i++){
            if(Tank->Bullets[i].Launched)
            {
                continue;
            } else if (!Tank->Bullets[i].Launched) {

                // TODO: Fix bugs right here
                // if(Tank->userBelong){
                //     printf("Bullet chosen\n");
                // }

                Tank->BulletsNumber--;
                    if(Tank->BulletsNumber < 0){
                        Tank->BulletsNumber = 0;
                    }                    

                
                switch((int)Tank->face)
                {
                    case (int)UP:
                        Tank->Bullets[i].blBox.x = Tank->mBox.x + Tank->mBox.w/2 + 15;
                        Tank->Bullets[i].blBox.y = Tank->mBox.y + 5;
                        Tank->Bullets[i].BlVelY -= BULLET_VEL;
                        // if(Tank->userBelong){
                        //     printf("Give bullet veclocity\n");                    
                        // }                        
                        break; 
                    case (int)DOWN:
                        Tank->Bullets[i].blBox.x = Tank->mBox.x + Tank->mBox.w/2 + 15;
                        Tank->Bullets[i].blBox.y = Tank->mBox.y + Tank->mBox.h + 5;
                        Tank->Bullets[i].BlVelY += BULLET_VEL;
                        // if(Tank->userBelong){
                        //     printf("Give bullet veclocity\n");                    
                        // }                                                
                        break; 
                    case (int)RIGHT:
                        Tank->Bullets[i].blBox.x = Tank->mBox.x + Tank->mBox.w + 5;
                        Tank->Bullets[i].blBox.y = Tank->mBox.y + Tank->mBox.h/2 + 15;
                        Tank->Bullets[i].BlVelX += BULLET_VEL;
                        // if(Tank->userBelong){
                        //     printf("Give bullet veclocity\n");                    
                        // }                                                
                        break; 
                    case (int)LEFT:
                        Tank->Bullets[i].blBox.x = Tank->mBox.x + 5;
                        Tank->Bullets[i].blBox.y = Tank->mBox.y + Tank->mBox.h/2 + 15;
                        Tank->Bullets[i].BlVelX -= BULLET_VEL;
                        // if(Tank->userBelong){
                        //     printf("Give bullet veclocity\n");                    
                        // }                                                
                        break;             
                }
                // if(Tank->userBelong)
                // {
                //     printf("Out of loop\n");                
                // }
                Tank->Bullets[i].Launched = true;
                // Tank->Bullets[i].Launched?printf("Bullet Launched\n"):printf("Bullet wasn't launched\n");                
                break;
            }
        }
    }
            // printf("End function\n");    
}

void respawn(TankInfo* Tank){
    if(Tank->destroyed){
        real32 respawnStartTime = 0.0f;
        real32 respawnEndTime = 0.0f;        
        real32 respawnTime = 0.0f;
        
        respawnStartTime = SDL_GetTicks();
        respawnEndTime = SDL_GetTicks();
        
        while (respawnTime < 500.0f){
            // Why this make game so gotten bogged down
            respawnEndTime = SDL_GetTicks();
            respawnTime = respawnEndTime - respawnStartTime;
            // printf("Wait Time: %f\n", respawnTime);
        }                    

        // NOTE: Don't know why game stuck when put this inside while loop
        if(respawnTime = 500.0f){
            // printf("Wait Time: %f\n", respawnTime);
            respawnTime = 0.0f;
            resetTank(Tank);

            Tank->mBox.x = rand()%LEVEL_WIDTH;

            if(Tank->mBox.x > LEVEL_WIDTH - Tank->mBox.w){
                Tank->mBox.x = LEVEL_WIDTH - Tank->mBox.w;
            } else if(Tank->mBox.x < Tank->mBox.w) {
                Tank->mBox.x = Tank->mBox.w;
            }

            Tank->mBox.y = rand()%LEVEL_HEIGHT;

            if(Tank->mBox.y > LEVEL_HEIGHT - Tank->mBox.h){
                Tank->mBox.y = LEVEL_HEIGHT - Tank->mBox.w;                        
            } else if(Tank->mBox.y < Tank->mBox.h) {
                Tank->mBox.y = Tank->mBox.h;
            }
        }
    }                        
}

void handleEventForTank(KeyState* CurrentBut, TankInfo* Tank) {
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
                        Tank->mVelY -= TANK_VEL;
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
                            // printf("Button %d is released\n", CurrentBut->key);                                           
                                           
                case SDL_SCANCODE_DOWN:
                    Tank->mVelY -= TANK_VEL;
                    // printf("Button %d is released\n", CurrentBut->key);
                    break;
                case SDL_SCANCODE_LEFT:
                    Tank->mVelX += TANK_VEL;
                    // printf("Button %d is released\n", CurrentBut->key);                                                               
                    break;
                case SDL_SCANCODE_RIGHT:
                    Tank->mVelX -= TANK_VEL;
                    // printf("Button %d is released\n", CurrentBut->key);                                                               
                    break;
            }
        }
    }
}

//Moves the Tank and check collision against tiles
// Add one more tank and user tank to check (Tile *tiles[],)
void move(bool touchesWall, bool collided, TankInfo* Tank) {

    // NOTE: Apply dijkstra's algo here to make bot tank'move more smart

    // checkCollision(Tank->mBox, all current active bullets boxes)
    if(!Tank->destroyed){
        // if (Tank->userBelong){
            // And swivel the tank by the way
            //||touchesWall
        //
        //TODO: Add collision code here
            Tank->mBox.x += Tank->mVelX;
            // (Tank->mBox.x < 10)||(Tank->mBox.x + Tank->mBox.w > LEVEL_WIDTH - 50)||
           if ((Tank->mBox.x < 0)||(Tank->mBox.x  > LEVEL_WIDTH - TANK_WIDTH) || collided){
                    Tank->mBox.x -= Tank->mVelX;
               }
                    // printf("Tank Pos X is: %d /n", Tank->mBox.x);                    
            
            Tank->mBox.y += Tank->mVelY;
            // (Tank->mBox.y < 10)||(Tank->mBox.y + Tank->mBox.h > LEVEL_HEIGHT  - 50) ||
            if ((Tank->mBox.y < 0)||(Tank->mBox.y > LEVEL_HEIGHT  - TANK_HEIGHT) || collided)
            {
                    Tank->mBox.y -= Tank->mVelY;
                    // printf("Tank Pos Y is: %d /n", Tank->mBox.y);
            }
            
             // Move the bullet
            //TODO: Time to detach the checking function out of moving one
            // to avoid unwanted duplicated work (WORKING!)

            for(int i = 0 ; i < TOTAL_BULLET_PER_TANK; i++) {
                if (Tank->Bullets[i].Launched){
                    Tank->Bullets[i].blBox.x += Tank->Bullets[i].BlVelX;
                    Tank->Bullets[i].blBox.y += Tank->Bullets[i].BlVelY;
                }
        }    
    }
}

//Centers the camera over the Tank
void setCamera( SDL_Rect& camera, TankInfo* UserTank ){
    // Initialize the collision box
	camera.x = ( UserTank->mBox.x + TANK_WIDTH / 2 ) - camera.w/2;
    // Give a fair distance between the camera and the main tank
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

void littleGuide(TankInfo* targetTank, TankInfo* UserTank, bool collided){
    // TODO: This function is a little AI that use Dijktra algorithm to drive every
    // bot tank

    int distance = sqrt(pow(targetTank->mBox.x - UserTank->mBox.x,2) + pow(targetTank->mBox.y - UserTank->mBox.y,2));

    // NOTE: The idea is simple: moving the bot Tank toward User's one and fire
    // when it is near
    // Prioritize the shorter axis first to shoot if any 

    // NOTE: Wandering mode

    std::srand(std::time(nullptr));
    if(targetTank->mVelX == 0 && targetTank->mVelY == 0){        
        switch((int)targetTank->face){
            case (int)UP: targetTank->mVelY = 0; targetTank->mVelY = -TANK_VEL;
            case (int)DOWN: targetTank->mVelY = 0;targetTank->mVelY = TANK_VEL;
            case (int)RIGHT: targetTank->mVelX = 0;targetTank->mVelX = TANK_VEL;
            case (int)LEFT: targetTank->mVelX = 0; targetTank->mVelX = -TANK_VEL;
        }
    }
    
    if (targetTank->mBox.x < TANK_WIDTH || targetTank->mBox.x + TANK_WIDTH + 50 > LEVEL_WIDTH|| targetTank->mBox.y < TANK_HEIGHT + 50 || targetTank->mBox.y + TANK_HEIGHT + 50 > LEVEL_HEIGHT || collided){
        if(targetTank->face < 180){
            targetTank->face += 180.0f;
        } else {
            targetTank->face -= 180.0f;
        };        
        targetTank->mVelX = -targetTank->mVelX;
        targetTank->mVelY = -targetTank->mVelY;
    }

    // distance > 250 && 
    if (!collided){
        if (abs(targetTank->mBox.x - UserTank->mBox.x) < abs(targetTank->mBox.y - UserTank->mBox.y)) {
        // NOTE: Prioritize which axis is choose to move first
        if(targetTank->mVelX != 0){
            targetTank->mVelX = 0;            
        }
        
    if (targetTank->mBox.x - UserTank->mBox.x < 250){
            if (targetTank->face != LEFT){
            targetTank->face = LEFT;
            }
            if (targetTank->mBox.x > 0){                
                targetTank->mVelX = -TANK_VEL;
                targetTank->mVelY = 0;            
            }
        } else if (targetTank->mBox.x - UserTank->mBox.x > - 250){
            if(targetTank->face != RIGHT){
                targetTank->face = RIGHT;
            }
            if (targetTank->mBox.x + TANK_WIDTH < LEVEL_WIDTH){                
                targetTank->mVelX = TANK_VEL;
                targetTank->mVelY = 0;
            }
        } else {
            targetTank->mVelX = 0;            
        }
    
    } else if (abs(targetTank->mBox.x - UserTank->mBox.x) > abs(targetTank->mBox.y - UserTank->mBox.y)) {
        
        if(targetTank->mVelY != 0){
            targetTank->mVelY = 0;            
        }        
        if (targetTank->mBox.y - UserTank->mBox.y < 100){
            if(targetTank->face != UP){                
            targetTank->face = UP;
            }
            if (targetTank->mBox.y > 0){                
                targetTank->mVelX = 0;
                targetTank->mVelY = -TANK_VEL;
            }
        } else if (targetTank->mBox.y - UserTank->mBox.y > - 100){
            if(targetTank->face != DOWN){
            targetTank->face = DOWN;                
            }
            if (targetTank->mVelY + TANK_HEIGHT < LEVEL_HEIGHT ){                
                targetTank->mVelX = 0;                
                targetTank->mVelY = TANK_VEL;
            }
        } else {
            targetTank->mVelY = 0;
        }   
    }
}        // NOTE: Seem Like I don't need to use complicate Dijktra algorigthm yet
    else {
            if (abs(targetTank->mBox.x - UserTank->mBox.x) < TANK_WIDTH){
                        if (targetTank->mBox.x - UserTank->mBox.x > 0){                            
                            if(targetTank->face != LEFT){
                                targetTank->face = LEFT;
                            }                            
                        } else if (targetTank->mBox.x - UserTank->mBox.x < 0){
                            if(targetTank->face != RIGHT){
                                targetTank->face = RIGHT;
                            }                            
                        }
            } else if(abs(targetTank->mBox.y - UserTank->mBox.y) < TANK_HEIGHT){
                        if(targetTank->mBox.y - UserTank->mBox.y > 0){                            
                            if(targetTank->face != UP){
                                targetTank->face = UP;
                            }
                        }

                        else if(targetTank->mBox.y - UserTank->mBox.y < 0){                            
                        if(targetTank->face != DOWN ){
                                targetTank->face =DOWN;
                            }
                        }
                    }
            if(!targetTank->userBelong){
                fire(targetTank);
            }
} 
}

void resetTank(TankInfo* Tank){
    Tank->mBox.x = -1;
    Tank->mBox.y = -1;
    Tank->isHit = false;
    Tank->destroyed = false;
}

void resetBullet(Bullet* bullet){
    bullet->Launched = false;
    bullet->BlVelX = 0;
    bullet->BlVelY = 0;
}

bool BiTankCheck(TankInfo* ATank, TankInfo* BTank){
    // NOTE: Put this function in the Moving function
    // ON WORK and Experiment
    bool TwoTankcollided = checkCollision(&ATank->mBox, &BTank->mBox);
    // printf("Start checking the whether tank or bullet is collided\n");
    for(int i = 0 ; i < TOTAL_BULLET_PER_TANK; i++) {
        if (ATank->Bullets[i].Launched && !BTank->destroyed){
            if ((ATank->Bullets[i].blBox.x < 0)||(ATank->Bullets[i].blBox.x + ATank->Bullets[i].blBox.w > LEVEL_WIDTH||ATank->Bullets[i].blBox.y < 0)||(ATank->Bullets[i].blBox.y + ATank->Bullets[i].blBox.h > LEVEL_HEIGHT)){
                resetBullet(&ATank->Bullets[i]);
            }
                    
            if(checkCollision(&ATank->Bullets[i].blBox,&BTank->mBox) && !BTank->isHit){
                BTank->isHit = true;
                ATank->BulletsNumber++;                
                if(ATank->BulletsNumber > TOTAL_BULLET_PER_TANK){
                    ATank->BulletsNumber = TOTAL_BULLET_PER_TANK;
                }
                resetBullet(&ATank->Bullets[i]);
            }
        }

        if (BTank->Bullets[i].Launched && !ATank->destroyed){

            if ((BTank->Bullets[i].blBox.x < 0)||(BTank->Bullets[i].blBox.x + BTank->Bullets[i].blBox.w > LEVEL_WIDTH||BTank->Bullets[i].blBox.y < 0)||(BTank->Bullets[i].blBox.y + BTank->Bullets[i].blBox.h > LEVEL_HEIGHT)){
                BTank->BulletsNumber++;
                if(BTank->BulletsNumber > TOTAL_BULLET_PER_TANK){
                    BTank->BulletsNumber = TOTAL_BULLET_PER_TANK;
                }
                resetBullet(&BTank->Bullets[i]);
            }
                    
            if(checkCollision(&BTank->Bullets[i].blBox,&ATank->mBox) && !ATank->isHit){
                ATank->isHit = true;
                BTank->BulletsNumber++;                
                if(BTank->BulletsNumber > TOTAL_BULLET_PER_TANK){
                    BTank->BulletsNumber = TOTAL_BULLET_PER_TANK;
                }
                resetBullet(&BTank->Bullets[i]);
            }
        }        
    }
    return TwoTankcollided;
}
