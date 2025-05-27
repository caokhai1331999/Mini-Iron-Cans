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
        for(int i = 0; i < TankNumber; i++)
        {
            Tank[i].mBox = {TankPos[i].x, TankPos[i].y, TANK_WIDTH, TANK_HEIGHT};
        }        
    }
    else
    {
        Tank->mBox.x = TankPos->x;
        Tank->mBox.y = TankPos->y;
    }

}

void fire(TankInfo* Tank){
    FACE tempface = NOPE;
    tempface = Tank->Belong?Tank->face:Tank->firingface;
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
                
                    Tank->Bullets[i].face = tempface;
                switch((int)tempface)
                {
                    case (int)UP:
                        Tank->Bullets[i].blBox.x = Tank->mBox.x + Tank->mBox.w/4;
                        Tank->Bullets[i].blBox.y = Tank->mBox.y;
                        Tank->Bullets[i].BlVelY = -BULLET_VEL;
                        break; 

                    case (int)DOWN:
                        Tank->Bullets[i].blBox.x = Tank->mBox.x + Tank->mBox.w/4;
                        Tank->Bullets[i].blBox.y = Tank->mBox.y + Tank->mBox.h;
                        Tank->Bullets[i].BlVelY = +BULLET_VEL;
                        break; 

                    case (int)RIGHT:
                        Tank->Bullets[i].blBox.x = Tank->mBox.x + Tank->mBox.w;
                        Tank->Bullets[i].blBox.y = Tank->mBox.y + Tank->mBox.h/4;
                        Tank->Bullets[i].BlVelX = +BULLET_VEL;
                        break; 

                    case (int)LEFT:
                        Tank->Bullets[i].blBox.x = Tank->mBox.x;
                        Tank->Bullets[i].blBox.y = Tank->mBox.y + Tank->mBox.h/4;
                        Tank->Bullets[i].BlVelX = -BULLET_VEL;
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
         if( CurrentBut->pressed && CurrentBut->repeat == 0 )
        {
                switch( CurrentBut->key )
                {
                    case SDL_SCANCODE_UP:
                        Tank->face = UP;
                        Tank->mVelY = 0; 
                        Tank->mVelY -= TANK_VEL;
                        break;
                        
                        case SDL_SCANCODE_DOWN:
                            Tank->face = DOWN;
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
            //Adjust the velocity
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

void move(TankInfo* Tank) {

    // NOTE: System constantly check the blocked face so we don't need to keep old face block at the previous frame.

    if(!Tank->isHit && !Tank->destroyed){

        if((!Tank->Belong && Tank->MovingWaitTime >= 10 && !Tank->stuck && Tank->ShouldMove) || Tank->Belong)
        {
            Tank->mBox.x += Tank->mVelX;

            if ((Tank->mBox.x <= 0) || (Tank->mBox.x  >= LEVEL_WIDTH - (TANK_WIDTH)) || Tank->collidedFace[2] == LEFT || Tank->collidedFace[2] == RIGHT){

                if(Tank->collidedFace[2] == NOPE){
                    Tank->collidedFace[0] = (Tank->mBox.x <= 0)?LEFT:RIGHT;
                }

                if(!Tank->stuck){
                    Tank->stuck = true;
                }

                Tank->mBox.x -= Tank->mVelX;
            };
            
            Tank->mBox.y += Tank->mVelY;

            if ((Tank->mBox.y <= 0) || (Tank->mBox.y >= LEVEL_HEIGHT - (TANK_HEIGHT)) || Tank->collidedFace[2] == UP || Tank->collidedFace[2] == DOWN)
            {
                if(Tank->collidedFace[2] == NOPE){
                    Tank->collidedFace[1] = (Tank->mBox.y <= 0)?UP:DOWN;
                }

                if(!Tank->stuck){
                    Tank->stuck = true;
                }

                Tank->mBox.y -= Tank->mVelY;
            }            

        if( (Tank->face == Tank->collidedFace[0] || Tank->face == Tank->collidedFace[1]|| (Tank->face == Tank->collidedFace[2] ))){
                if(!Tank->stuck){
                    Tank->stuck = true;
                }
        }
        Tank->MovingWaitTime = 0;
        }
        else {
            Tank->MovingWaitTime++;
        }

        for(int i = 0 ; i < TOTAL_BULLET_PER_TANK; i++) {

            if (Tank->Bullets[i].Launched && !Tank->Bullets[i].destroyed){

                // ConstructRectJoint(&Tank->Bullets[i].blBox, BulletScaffold);
                    Tank->Bullets[i].blBox.x += Tank->Bullets[i].BlVelX;
                    Tank->Bullets[i].blBox.y += Tank->Bullets[i].BlVelY;
                
                if((Tank->Bullets[i].blBox.x < 0)||(Tank->Bullets[i].blBox.x + Tank->Bullets[i].blBox.w > LEVEL_WIDTH||Tank->Bullets[i].blBox.y < 0)||(Tank->Bullets[i].blBox.y + Tank->Bullets[i].blBox.h > LEVEL_HEIGHT)){

                    Tank->BulletsNumber++;
                    if(Tank->BulletsNumber > TOTAL_BULLET_PER_TANK){
                        Tank->BulletsNumber = TOTAL_BULLET_PER_TANK;
                    };

                    resetBullet(&Tank->Bullets[i]);
                }
            }                
        }
    }    
}

//Centers the camera over the Tank
void setCamera( SDL_Rect* camera, TankInfo* UserTank ){

    // NOTE: Center the userTank inside the camera
    // Initialize the collision box
	camera->x = ( UserTank->mBox.x + (TANK_WIDTH/2) ) - ((camera->w)/2);
	camera->y = ( UserTank->mBox.y + (TANK_HEIGHT/2) ) - ((camera->h)/2);
    // Give a fair distance between the camera and the main tank

    // printf("User tank pos:%d %d\n",UserTank->mBox.x, UserTank->mBox.y);
    // printf("Camera pos:%d %d\n",camera->x, camera->y);
    // NOTE: so with this formula why camera pos is alway ahead of (>) userTank one
    // Why camera w, h turn to 0
    
	//Keep the camera in bounds
	if( camera->x < 0)
	{ 
		camera->x = 0;
	}

	if( camera->y < 0)
	{
		camera->y = 0;
	}

	if( camera->x > LEVEL_WIDTH - camera->w )
	{
		camera->x = LEVEL_WIDTH - camera->w;
	}
	if( camera->y > LEVEL_HEIGHT - camera->h )
	{
		camera->y = LEVEL_HEIGHT - camera->h;
	}
    // printf("Camera size:%d %d\n",camera->w, camera->h);    
}

FACE faceCalculate(SDL_Rect* userBox, SDL_Rect* targetBox){
    if(targetBox->x - userBox->x >= targetBox->y - userBox->y){
        if(targetBox->x >= userBox->x){
                return LEFT;
            } else {
                return RIGHT;
            }
    }  else {
            if(targetBox->y >= userBox->y){
                return UP;
            } else {
                return DOWN;
            }            
        }
}

void littleGuide(TankInfo* targetTank, TankInfo* UserTank) {
    // TODO: This function is a little AI that use Dijktra algorithm to drive every
    // bot tank
    std::srand(std::time(nullptr));

    int distance = sqrt(pow(targetTank->mBox.x - UserTank->mBox.x,2) + pow(targetTank->mBox.y - UserTank->mBox.y,2));

    // Check relative face between bot and user Tank
    bool verticalCheck = false;
    bool horizontalCheck = false;

    if (targetTank->mBox.x + targetTank->mBox.w/2 - BULLET_WIDTH/2 >= UserTank->mBox.x && targetTank->mBox.x + targetTank->mBox.w/2 - BULLET_WIDTH/2 <= UserTank->mBox.x + UserTank->mBox.w){

        if(!verticalCheck){
            verticalCheck = true;
        }
        printf("Time for vertical check\n");
        if(targetTank->mBox.y <= UserTank->mBox.y){
            if(targetTank->firingface != DOWN){
                targetTank->firingface = DOWN;
            }
        }  else {
            if(targetTank->firingface != UP){
                targetTank->firingface = UP;
            }
        }
    } else {
        if(verticalCheck){
            verticalCheck = false;
        }
    }

    if(targetTank->mBox.y + targetTank->mBox.h/2 - BULLET_HEIGHT/2 >= UserTank->mBox.y && targetTank->mBox.y + targetTank->mBox.h/2 - BULLET_HEIGHT/2 <= UserTank->mBox.y + UserTank->mBox.h) {

        if(!horizontalCheck){
            horizontalCheck = true;
        }

        printf("Time for horizontal check\n");

        if(targetTank->mBox.x <= UserTank->mBox.x){
            if(targetTank->firingface != RIGHT){
                targetTank->firingface = RIGHT;
            }
            
        }  else {
            if(targetTank->firingface != LEFT){
                targetTank->firingface = LEFT;
            }
        }

        // NOTE: Run away if needed
        
    } else {                    
        if(horizontalCheck){
            horizontalCheck = false;
        }
    }

    if(verticalCheck || horizontalCheck){
        if(!targetTank->isFiring){
            targetTank->isFiring = true;
        }
    }else{
        if(targetTank->isFiring){
            targetTank->isFiring = false;
        }        
    }
    
    if(targetTank->FireWaitTime >= 10)
    {
        fire(targetTank);
        targetTank->FireWaitTime = 0;
    } else {            
        targetTank->FireWaitTime++;
    }
    
    // NOTE: Try to keep bot Tank in a secure region
    FACE tempface = targetTank->firingface>=DOWN?targetTank->firingface-180.0:targetTank->firingface+180.0;
    
    if(distance <= SECURE_DISTANCE){
            if(!targetTank->ShouldMove){
                targetTank->ShouldMove = true;
            };
        } else if(distance >= 2 *SECURE_DISTANCE) {
            if(targetTank->ShouldMove){
                targetTank->ShouldMove = false;
            };
        }    
    
    if(targetTank->stuck && targetTank->ShouldMove) {
        printf("AutoReroute the tank \n");
        FACE rollfaces[4] = {UP, RIGHT, DOWN, LEFT};

        while(targetTank->face == targetTank->collidedFace[0]
              ||targetTank->face == targetTank->collidedFace[1]||targetTank->face == targetTank->collidedFace[2]){
            targetTank->face += 90.0;
            if ((double)targetTank->face >= 360.0){
                targetTank->face = UP;
            }
        }
        targetTank->stuck = false;
    }    

    
    if(tempface != targetTank->face && tempface != targetTank->collidedFace[0] && tempface != targetTank->collidedFace[1] && tempface != targetTank->collidedFace[2]){
        targetTank->face = tempface;
    }

    // DONE!: Success reroute Tank whenever collided
    // TODO: Time to make tank smarter

    switch((int)targetTank->face){
        case (int)UP:
            targetTank->mVelY = -TANK_VEL;
            targetTank->mVelX = 0;
            break;
        case (int)DOWN:
            targetTank->mVelY = TANK_VEL;
            targetTank->mVelX = 0;
            break;
        case (int)RIGHT:
            targetTank->mVelX = TANK_VEL;
            targetTank->mVelY = 0;
            break;                
        case (int)LEFT:
            targetTank->mVelX = -TANK_VEL;
            targetTank->mVelY = 0;
            break;
    }

    // NOTE: The idea is simple: moving the bot Tank toward User's one and fire
    // when it is near
    // Prioritize the shorter axis first to shoot 

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
    bullet->destroyed = false;
    bullet->BlVelX = 0;
    bullet->BlVelY = 0;
}

bool BiTankCheck(TankInfo* ATank, TankInfo* BTank){
    // NOTE: Put this function in the Moving function
    // ON WORK and Experiment
    bool Collided = false;
    CollisionData* Data = nullptr;
    Data = new CollisionData();
    if(!ATank->isHit && !BTank->isHit){
        *Data = checkCollisionS(&ATank->mBox, &BTank->mBox);
         ATank->collidedFace[2] = Data->TankAface;
         BTank->collidedFace[2] = Data->TankBface;
         Collided = Data->collided;
         
        for(int i = 0 ; i < TOTAL_BULLET_PER_TANK; i++) {

            if (ATank->Bullets[i].Launched && !BTank->isHit){
                if(checkCollision(&ATank->Bullets[i].blBox,&BTank->mBox) && !BTank->isHit){
                    BTank->isHit = true;
                    BTank->destroyed = false;
                    ATank->Bullets[i].destroyed = true;                                        
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
                    BTank->Bullets[i].destroyed = true;                    
                    BTank->BulletsNumber++;                

                    if(BTank->BulletsNumber > TOTAL_BULLET_PER_TANK){
                        BTank->BulletsNumber = TOTAL_BULLET_PER_TANK;
                    }
                    resetBullet(&BTank->Bullets[i]);
                }
            }        
        }
    }
    delete Data;
    Data = nullptr;
    return Collided;
}
