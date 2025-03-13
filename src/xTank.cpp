/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include <xTank.h>

//Takes key presses and adjusts the Tank's velocity
// Move this function to PlatForm one

void GenerateSinglePosition(int* x, int* y){
    int a,b;
    a = std::rand()%LEVEL_WIDTH;
    
    if(a > LEVEL_WIDTH - TANK_WIDTH){
        a = LEVEL_WIDTH - TANK_WIDTH - 20;
    }
    *x = a;
    b = std::rand()%LEVEL_HEIGHT;

    if(b > LEVEL_HEIGHT - TANK_HEIGHT){
        b = LEVEL_HEIGHT - TANK_HEIGHT - 20;                        
    }
    *y = b;
}


void InitializeTankPos(Position* RealTankPos){
    bool valid = true;
    std::srand(std::time(nullptr));
    GenerateSinglePosition(&RealTankPos[0].x, &RealTankPos[0].y);
    Position* TempPos = nullptr;
    TempPos = (struct Position*)malloc(sizeof(struct Position ));
        // Loop comparing newly created pos to the previous valid one
    for (int i= 1; i < TOTAL_ENEMY_TANK; i++){
        valid = false;
        while (!valid) {
            GenerateSinglePosition(&TempPos->x, &TempPos->y);
            // printf("Temp Tank pos is: [%d, %d]", TempPos->x, TempPos->y);
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

   // delete TempPos;
    free(TempPos);
}                                 

void InitializeTankInfo(Position* TankPos,TankInfo* Tank){

    int TankNumber = Tank->Belong?1:TOTAL_ENEMY_TANK;
    if(TankNumber > 1){
        for(int i = 0; i < TankNumber; i++){        
            Tank[i].mBox = {TankPos[i].x, TankPos[i].y, TANK_WIDTH, TANK_HEIGHT};
            // userBelong?&Tank[i]->Bullets[i].type = userB:&Tank[i]->Bullets[i].type = enemyB;
        }        
    }
    else {
        Tank->mBox.x = TankPos->x;
        Tank->mBox.y = TankPos->y;
    }
}

void fire(TankInfo* Tank){
    if(!Tank->destroyed && !Tank->isHit) {
        for(int i = 0; i < TOTAL_BULLET_PER_TANK ; i++){
            if(Tank->Bullets[i].Launched)
            {
                continue;
            } else if (!Tank->Bullets[i].Launched) {


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
                Tank->Bullets[i].Launched = true;
                break;
            }
        }
    }
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
            Tank->isHit = false;
            Tank->destroyed = false;
            
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

    if (Tank->Belong) {
         if( CurrentBut->pressed && CurrentBut->repeat == 0)
        {
            //Adjust the velocity
                switch( CurrentBut->key )
                {
                    case SDL_SCANCODE_UP: Tank->face = UP;
                        Tank->mVelY = 0; 
                        Tank->mVelY -= TANK_VEL;
                        break;
                        
                        case SDL_SCANCODE_DOWN: Tank->face = DOWN;
                            Tank->mVelY = 0; 
                            Tank->mVelY += TANK_VEL;
                        break;

                    case SDL_SCANCODE_RIGHT:
                            Tank->face = RIGHT;
                            Tank->mVelX = 0;                             
                            Tank->mVelX += TANK_VEL;
                        break;

                    case SDL_SCANCODE_LEFT: Tank->face = LEFT;
                            Tank->mVelX = 0;                             
                            Tank->mVelX -= TANK_VEL;
                        break;
                        
                    case SDL_SCANCODE_SPACE: fire(Tank);
                        break;
                }
                
        }
         else if( !CurrentBut->pressed && CurrentBut->repeat == 0)
        {
            //Adjust the velocity
            switch(CurrentBut->key )
            {
                case SDL_SCANCODE_UP:
                    Tank->mVelY += TANK_VEL;
                    break;
                                           
                case SDL_SCANCODE_DOWN:
                    Tank->mVelY -= TANK_VEL;

                    break;
                case SDL_SCANCODE_LEFT:
                    Tank->mVelX += TANK_VEL;
                    break;

                case SDL_SCANCODE_RIGHT:
                    Tank->mVelX -= TANK_VEL;
                    break;
            }
        }
    }
}

void move(bool touchesWall, bool collided, TankInfo* Tank) {

    if(!Tank->isHit && !Tank->destroyed){

        Tank->mBox.x += Tank->mVelX;
        if ((Tank->mBox.x < 0)||(Tank->mBox.x  > LEVEL_WIDTH - (TANK_WIDTH + 20)) || collided){
            Tank->mBox.x -= Tank->mVelX;
        }
            
        Tank->mBox.y += Tank->mVelY;
        if ((Tank->mBox.y < 0)||(Tank->mBox.y > LEVEL_HEIGHT - (TANK_HEIGHT + 20)) || collided)
        {
            Tank->mBox.y -= Tank->mVelY;
        }
            
        for(int i = 0 ; i < TOTAL_BULLET_PER_TANK; i++) {

            if (Tank->Bullets[i].Launched){
                Tank->Bullets[i].blBox.x += Tank->Bullets[i].BlVelX;
                Tank->Bullets[i].blBox.y += Tank->Bullets[i].BlVelY;

                if((Tank->Bullets[i].blBox.x < 0)||(Tank->Bullets[i].blBox.x + Tank->Bullets[i].blBox.w > LEVEL_WIDTH||Tank->Bullets[i].blBox.y < 0)||(Tank->Bullets[i].blBox.y + Tank->Bullets[i].blBox.h > LEVEL_HEIGHT)){

                    Tank->BulletsNumber++;
                    if(Tank->BulletsNumber > TOTAL_BULLET_PER_TANK){
                        Tank->BulletsNumber = TOTAL_BULLET_PER_TANK;
                    };
                    // char text [50] = {};

                    // if(Tank->Belong){
                    //     sprintf(text, "User Tank bullet number is :%d\n", (int)(Tank->BulletsNumber));                    
                    // };

                    // printf(text);
                    resetBullet(&Tank->Bullets[i]);
                }
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
    // Prioritize the shorter axis first to shoot 
    // NOTE: Wandering mode

    std::srand(std::time(nullptr));
    if(targetTank->mVelX == 0 && targetTank->mVelY == 0){        
        switch((int)targetTank->face){
            case (int)UP: targetTank->mVelY = 0;
                targetTank->mVelY = -TANK_VEL;
                break;
            case (int)DOWN: targetTank->mVelY = 0;
                targetTank->mVelY = TANK_VEL;
                break;
            case (int)RIGHT: targetTank->mVelX = 0;
                targetTank->mVelX = TANK_VEL;
                break;                
            case (int)LEFT: targetTank->mVelX = 0;
                targetTank->mVelX = -TANK_VEL;
                break;
        }
    }
    
    if (targetTank->mBox.x < 0 || targetTank->mBox.x  > LEVEL_WIDTH - TANK_WIDTH -30 || targetTank->mBox.y < 0 || targetTank->mBox.y > LEVEL_HEIGHT - TANK_HEIGHT - 30 || collided){
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
            if (targetTank->mBox.x  < LEVEL_WIDTH - TANK_WIDTH){                
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
            if (targetTank->mVelY < LEVEL_HEIGHT - TANK_HEIGHT ){                
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

            if(!targetTank->Belong){
                // NOTE: How to make bot tank look less stupid when they firing
                // and how to make fire less frequent
                fire(targetTank);
            }
} 
}

void resetTank(TankInfo* Tank){
    Tank->mBox.x = -1;
    Tank->mBox.y = -1;
    Tank->isHit = false;
    Tank->destroyed = false;
    // NOTE: Forgot to destroy bullet pointer. My bad!!!
    for (int i = 0; i<TOTAL_BULLET_PER_TANK; i++){
        resetBullet(&Tank->Bullets[i]);
    };
    Tank->BulletsNumber = TOTAL_BULLET_PER_TANK;
}

void resetBullet(Bullet* bullet){
    bullet->Launched = false;
    bullet->BlVelX = 0;
    bullet->BlVelY = 0;
}

bool BiTankCheck(TankInfo* ATank, TankInfo* BTank){
    // NOTE: Put this function in the Moving function
    // ON WORK and Experiment
    bool TwoTankcollided = false;
    if(!ATank->isHit && !BTank->isHit){
        TwoTankcollided = checkCollision(&ATank->mBox, &BTank->mBox);

        for(int i = 0 ; i < TOTAL_BULLET_PER_TANK; i++) {

            if (ATank->Bullets[i].Launched && !BTank->isHit){
                if(checkCollision(&ATank->Bullets[i].blBox,&BTank->mBox) && !BTank->isHit){
                    BTank->isHit = true;
                    BTank->destroyed = false;
                    
                    ATank->BulletsNumber++;                
                    if(ATank->BulletsNumber > TOTAL_BULLET_PER_TANK){
                        ATank->BulletsNumber = TOTAL_BULLET_PER_TANK;
                    }
                    resetBullet(&ATank->Bullets[i]);
                }
            }

            if (BTank->Bullets[i].Launched && !ATank->isHit){
            
                if(checkCollision(&BTank->Bullets[i].blBox,&ATank->mBox) && !ATank->isHit){
                    ATank->isHit = true;
                    ATank->destroyed = false;
                    
                    BTank->BulletsNumber++;                
                    if(BTank->BulletsNumber > TOTAL_BULLET_PER_TANK){
                        BTank->BulletsNumber = TOTAL_BULLET_PER_TANK;
                    }
                    resetBullet(&BTank->Bullets[i]);
                }
            }        
        }
    }
    return TwoTankcollided;
}
