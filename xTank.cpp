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
    Tem.X = std::rand()%(LEVEL_WIDTH*2);
    Tem.Y = std::rand()%(LEVEL_HEIGHT*2);
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
    // && !Tank->isHit

    if(!Tank->destroyed) {
            if(Tank->userBelong){
                printf("Looping through tank bullets\n");
            }
        for(int i = 0; i < TOTAL_BULLET_PER_TANK ; i++){
            if(Tank->Bullets[i].Launched)
            {
                continue;
            } else if (!Tank->Bullets[i].Launched){

                // TODO: Fix bugs right here
                if(Tank->userBelong){
                    printf("Bullet chosen\n");
                }

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
                        if(Tank->userBelong){
                            printf("Give bullet veclocity\n");                    
                        }                        
                        break; 
                    case (int)DOWN:
                        Tank->Bullets[i].blBox.x = Tank->mBox.x + Tank->mBox.w/2 + 15;
                        Tank->Bullets[i].blBox.y = Tank->mBox.y + Tank->mBox.h + 5;
                        Tank->Bullets[i].BlVelY += BULLET_VEL;
                        if(Tank->userBelong){
                            printf("Give bullet veclocity\n");                    
                        }                                                
                        break; 
                    case (int)RIGHT:
                        Tank->Bullets[i].blBox.x = Tank->mBox.x + Tank->mBox.w + 5;
                        Tank->Bullets[i].blBox.y = Tank->mBox.y + Tank->mBox.h/2 + 15;
                        Tank->Bullets[i].BlVelX += BULLET_VEL;
                        if(Tank->userBelong){
                            printf("Give bullet veclocity\n");                    
                        }                                                
                        break; 
                    case (int)LEFT:
                        Tank->Bullets[i].blBox.x = Tank->mBox.x + 5;
                        Tank->Bullets[i].blBox.y = Tank->mBox.y + Tank->mBox.h/2 + 15;
                        Tank->Bullets[i].BlVelX -= BULLET_VEL;
                        if(Tank->userBelong){
                            printf("Give bullet veclocity\n");                    
                        }                                                
                        break;             
                }
                // if(Tank->userBelong)
                // {
                //     printf("Out of loop\n");                
                // }
                printf("Out of loop\n");                
                Tank->Bullets[i].Launched = true;
                break;
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
// Add one more tank and user tank to check
void move( Tile *tiles[], bool touchesWall, bool collided, TankInfo* Tank) {

    // NOTE: Apply dijkstra's algo here to make bot tank'move more smart

    // checkCollision(Tank->mBox, all current active bullets boxes)
   
    if(!Tank->destroyed){
        // if (Tank->userBelong){
            // And swivel the tank by the way
            //||touchesWall
        //
        //TODO: Add collision code here
            Tank->mBox.x += Tank->mVelX;
            // (Tank->mBox.x < 10)||(Tank->mBox.x + Tank->mBox.w > LEVEL_WIDTH*2 - 50)||
           if ((Tank->mBox.x < 10)||(Tank->mBox.x + Tank->mBox.w > LEVEL_WIDTH*2 - 50)||collided){
                    Tank->mBox.x -= Tank->mVelX;
               }
                    // printf("Tank Pos X is: %d /n", Tank->mBox.x);                    
            
            Tank->mBox.y += Tank->mVelY;
            // (Tank->mBox.y < 10)||(Tank->mBox.y + Tank->mBox.h > LEVEL_HEIGHT *2 - 50) ||
            if ((Tank->mBox.y < 10)||(Tank->mBox.y + Tank->mBox.h > LEVEL_HEIGHT *2 - 50) || collided)
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
            // }
        }    
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
	if( camera.x > LEVEL_WIDTH*2 - camera.w )
	{
		camera.x = LEVEL_WIDTH*2 - camera.w;
	}
	if( camera.y > LEVEL_HEIGHT*2 - camera.h )
	{
		camera.y = LEVEL_HEIGHT*2 - camera.h;
	}
    
}

void littleGuide(TankInfo* botTank, TankInfo* UserTank, bool collided){
    // TODO: This function is a little AI that use Dijktra algorithm to drive every
    // bot tank

    int distance = sqrt(pow(botTank->mBox.x - UserTank->mBox.x,2) + pow(botTank->mBox.y - UserTank->mBox.y,2));

    // NOTE: The idea is simple: moving the bot Tank toward User's one and fire
    // when it is near
    // Prioritize the shorter axis first to shoot if any 

    // NOTE: Wandering mode

    std::srand(std::time(nullptr));
    if (botTank->mVelX == 0 || botTank->mVelY == 0 || botTank->mBox.x < TANK_WIDTH || botTank->mBox.x + TANK_WIDTH > LEVEL_WIDTH*2|| botTank->mBox.y < TANK_HEIGHT || botTank->mBox.x + TANK_HEIGHT > LEVEL_HEIGHT*2 || collided){
    int FaceID = std::rand()%3;
    switch(FaceID){
        case 0: botTank->face = UP; botTank->mVelY = 0; botTank->mVelY = -TANK_VEL;
        case 1: botTank->face = DOWN; botTank->mVelY = 0;botTank->mVelY = TANK_VEL;
        case 2: botTank->face = RIGHT; botTank->mVelX = 0;botTank->mVelX = TANK_VEL;
        case 3: botTank->face = LEFT; botTank->mVelX = 0; botTank->mVelX = -TANK_VEL;
    };        
    }

    // distance > 250 && 
    if (!collided){
        if (abs(botTank->mBox.x - UserTank->mBox.x) < abs(botTank->mBox.y - UserTank->mBox.y)) {
        // NOTE: Prioritize which axis is choose to move first
        if(botTank->mVelX != 0){
            botTank->mVelX = 0;            
        }
        
    if (botTank->mBox.x - UserTank->mBox.x < 250*2){
            if (botTank->face != LEFT){
            botTank->face = LEFT;
            }
            if (botTank->mBox.x > 0){                
                botTank->mVelX -= TANK_VEL;
                botTank->mVelY = 0;            
            }
        } else if (botTank->mBox.x - UserTank->mBox.x > - 250*2){
            if(botTank->face != RIGHT){
                botTank->face = RIGHT;
            }
            if (botTank->mBox.x + TANK_WIDTH < LEVEL_WIDTH * 2){                
                botTank->mVelX += TANK_VEL;
                botTank->mVelY = 0;
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
            if (botTank->mBox.y > 0){                
                botTank->mVelX = 0;
                botTank->mVelY -= TANK_VEL;
            }
        } else if (botTank->mBox.y - UserTank->mBox.y > - 100){
            if(botTank->face != DOWN){
            botTank->face = DOWN;                
            }
            if (botTank->mVelY + TANK_HEIGHT < LEVEL_HEIGHT * 2){                
                botTank->mVelX = 0;                
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
                        } else if (botTank->mBox.x - UserTank->mBox.x < 0){
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
            if(!botTank->userBelong){
                fire(botTank);
            }
} 
}

void resetBullet(Bullet* bullet){
    bullet->Launched = false;
    bullet->BlVelX = 0;
    bullet->BlVelY = 0;
}

void BiTankCheck(TankInfo* ATank, TankInfo* BTank){
    // NOTE: Put this function in the Moving function
    // ON WORK and Experiment
    bool TwoTankcollided = checkCollision(&ATank->mBox, &BTank->mBox);

    for(int i = 0 ; i < TOTAL_BULLET_PER_TANK; i++) {
        if (ATank->Bullets[i].Launched){
            if ((ATank->Bullets[i].blBox.x < 0)||(ATank->Bullets[i].blBox.x + ATank->Bullets[i].blBox.w > LEVEL_WIDTH)){
                BTank->isHit = true;
                resetBullet(&ATank->Bullets[i]);
            }
                    
            ATank->Bullets[i].blBox.y += ATank->Bullets[i].BlVelY;
            if ((ATank->Bullets[i].blBox.y < 0)||(ATank->Bullets[i].blBox.y + ATank->Bullets[i].blBox.h > LEVEL_HEIGHT)){
                BTank->isHit = true;
                resetBullet(&ATank->Bullets[i]);                
            }

            if(checkCollision(&ATank->Bullets[i].blBox,&BTank->mBox)){
                BTank->isHit = true;
                ATank->BulletsNumber++;                
                if(ATank->BulletsNumber > TOTAL_BULLET_PER_TANK){
                    ATank->BulletsNumber = TOTAL_BULLET_PER_TANK;
                }
                resetBullet(&ATank->Bullets[i]);                
            }
        }

        if (BTank->Bullets[i].Launched){

            if ((BTank->Bullets[i].blBox.x < 0)||(BTank->Bullets[i].blBox.x + BTank->Bullets[i].blBox.w > LEVEL_WIDTH)){
                BTank->BulletsNumber++;
                if(BTank->BulletsNumber > TOTAL_BULLET_PER_TANK){
                    BTank->BulletsNumber = TOTAL_BULLET_PER_TANK;
                }
                resetBullet(&BTank->Bullets[i]);
            }
                    
            if ((BTank->Bullets[i].blBox.y < 0)||(BTank->Bullets[i].blBox.y + BTank->Bullets[i].blBox.h > LEVEL_HEIGHT)){
                BTank->BulletsNumber++;                
                if(BTank->BulletsNumber > TOTAL_BULLET_PER_TANK){
                    BTank->BulletsNumber = TOTAL_BULLET_PER_TANK;
                }
                resetBullet(&BTank->Bullets[i]);                
            }

            if(checkCollision(&BTank->Bullets[i].blBox,&ATank->mBox)){
                ATank->isHit = true;
                BTank->BulletsNumber++;                
                if(BTank->BulletsNumber > TOTAL_BULLET_PER_TANK){
                    BTank->BulletsNumber = TOTAL_BULLET_PER_TANK;
                }
                resetBullet(&BTank->Bullets[i]);
            }
        }        
    }    
}
