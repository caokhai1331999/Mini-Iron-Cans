#if !defined(XTANK_H)

/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <ctime>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Tile.h"
#include "GamePhysics.h"

using namespace std;

#define TANKFACE double
#define UP 0.0
#define DOWN 180.0
#define RIGHT 90.0
#define LEFT 270.0

enum BulletType{enemyB, userB};

const global_variable int TANK_VEL = 10 ;
const global_variable int TANK_WIDTH = 50 ;
const global_variable int TANK_HEIGHT = 50 ;
const global_variable int TOTAL_ENEMY_TANK = 3 ;
const global_variable int TOTAL_BULLET_PER_TANK = 10 ;

const global_variable int BULLET_VEL = 20 ;
const global_variable int BULLET_WIDTH = 10 ;
const global_variable int BULLET_HEIGHT = 10 ;

struct Position{
    int x;
    int y;
};

struct Bullet{
    // TODO: How to properly decide which is bot and which is user bullet
    BulletType type;
    int BlVelX, BlVelY;
    bool Launched;
    SDL_Rect blBox;
    Bullet(int TankX = 0, int TankY = 0, bool launched = false, bool userBelong = false)
    {
        blBox.x = TankX;
        blBox.y = TankY;
        Launched = launched;
        type = userBelong?userB:enemyB;
        blBox.w = BULLET_WIDTH;
        blBox.h = BULLET_HEIGHT;
    }
};

struct TankInfo
{
    bool userBelong;

    int BulletsNumber;
    int name;
    bool destroyed;
    bool isHit;
    SDL_Rect mBox;
    int mVelX, mVelY;
    TANKFACE face;
    Bullet* Bullets;
    // NOTE: Bullet created when Tank is created, follow them until being fired
    // after fire, create the second bullet to follow it when the previous is sent
    //flied

    TankInfo(bool userBelongx = false):userBelong(userBelongx){

        // Initialize the collision box
        destroyed = false;
        isHit = false;
        BulletsNumber = TOTAL_BULLET_PER_TANK;
        mBox.w = TANK_WIDTH;
        mBox.h = TANK_HEIGHT;
        mVelX = 0;
        mVelY = 0;
        Bullets = new Bullet[TOTAL_BULLET_PER_TANK];
        // TODO: figure out how to effectively flag these bullets as user's
        if(userBelong){
            mBox.x = rand()%SCREEN_WIDTH;
            mBox.y = rand()%SCREEN_HEIGHT;
            for (int i = 0; i < TOTAL_BULLET_PER_TANK; i++){
                Bullets[i].type = userB;
            };
        }
        switch(rand()%3){
            case 0: face = UP;break;
            case 1: face = RIGHT;break;
            case 2: face = DOWN;break;
            case 3: face = LEFT;break;
        };
        // NOTE: Turn out I can not delete any single element on array without changing the others. Got to rewrite it
    };
};

// bool OtherReleased(SDL_KeyboardEvent key);

// Randomize Tank Positions that far enough
void InitializeTankPos(Position* RealTankPos);

TankInfo InitializeTankInfo(int x, int y);

//Takes key presses and adjusts the Tank's velocity
void handleEventForTank(KeyState* CurrentBut, TankInfo* Tank );

// Bot tanks auto find way using dijstra algorithm
void littleGuide(TankInfo* botTank, TankInfo* UserTank, bool collided);
    
//Moves the Tank and check collision against tiles
void move(bool touchesWall = false, bool collided = false, TankInfo* Tank = NULL);

// Fire a bullet
void fire(TankInfo* Tank);
void respawn(TankInfo* Tank);

void resetBullet(Bullet* bullet);
void resetTank(TankInfo* Tank);
void BiTankCheck(TankInfo* ATank, TankInfo* BTank);
//Centers the camera over the Tank
void setCamera( SDL_Rect& camera ,TankInfo* UserTank);
#define XTANK_H
#endif
