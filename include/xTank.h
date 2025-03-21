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
#include "TileX.h"
#include "GamePhysics.h"

using namespace std;

#define TANKFACE double
#define UP 0.0
#define DOWN 180.0
#define RIGHT 90.0
#define LEFT 270.0


enum INDEX{
    _FIRST = 0,
    _SECOND = 1
};

#define INDEX_ENUM(DIRECTION) \
    DIRECTION ## _FIRST, \
    DIRECTION ## _SECOND,

enum {
    INDEX_ENUM(UP)
    INDEX_ENUM(DOWN)
    INDEX_ENUM(RIGHT)
    INDEX_ENUM(LEFT)
};

enum BulletType{enemyB, userB};

const global_variable int TANK_VEL = 10;
const global_variable int TANK_WIDTH = 20;
const global_variable int TANK_HEIGHT = 20;
const global_variable int TOTAL_ENEMY_TANK = 3 ;
const global_variable int TOTAL_BULLET_PER_TANK = 10 ;

const global_variable int BULLET_VEL = 20;
const global_variable int BULLET_WIDTH = 10 ;
const global_variable int BULLET_HEIGHT = 10 ;

struct Position{
    int x;
    int y;
    Position():x(0), y(0){
    };
};

struct Bullet{
    // TODO: How to properly decide which is bot and which is user bullet
    BulletType type;
    int BlVelX = 0;
    int BlVelY = 0;
    bool Launched = false;
    SDL_Point* BulletScaffold = nullptr;
    SDL_Rect blBox = {};

    Bullet(int TankX = 0, int TankY = 0, bool launched = false, bool userBelong = false): blBox({TankX, TankY, BULLET_WIDTH, BULLET_HEIGHT}), Launched(launched)
    {
        BulletScaffold = new SDL_Point[5];
        type = userBelong?userB:enemyB;
    }
    
};

struct TankInfo
{
    // int name;
    bool Belong = true;
    bool isMoving;
    bool destroyed = false;
    bool isHit = false;
    // NOTE: Don't know why this doesn't trigger default value
    SDL_Rect mBox;

    SDL_Point* TankScaffold = nullptr;

    int mVelX = 0;
    int mVelY = 0;
    TANKFACE face = (TANKFACE)(rand()%3) ;
    int BulletsNumber = TOTAL_BULLET_PER_TANK;
    Bullet* Bullets;

    TankInfo(int x = 0, int y = 0, bool userBelong = false):Belong(userBelong), mBox({x , y, TANK_WIDTH, TANK_HEIGHT}){

        TankScaffold = new SDL_Point[5];
        isMoving = false;
        Bullets = nullptr;
        Bullets = new Bullet[TOTAL_BULLET_PER_TANK]();
        // printf("Size of Bullet set is: %d\n", (int)(sizeof(Bullets)));

        for(int i = 0; i < TOTAL_ENEMY_TANK; i++){        
            Bullets[i].blBox = {mBox.x, mBox.y, BULLET_WIDTH, BULLET_HEIGHT};
            Belong?Bullets[i].type = userB:Bullets[i].type = enemyB;
        }

    };
    // NOTE: Bullet created when Tank is created, follow them until being fired
    // after fire, create the second bullet to follow it when the previous is sent
    //flied    
};

// bool OtherReleased(SDL_KeyboardEvent key);


void ConstructRectJoint(const SDL_Rect* rect, SDL_Point* Joint);
void ConstructTankScaffold(TankInfo* tank);
// Randomize Tank Positions that far enough
void InitializeTankPos(Position* RealTankPos);
void InitializeTankInfo(Position* Tankpos = nullptr, TankInfo* Tank = nullptr);

void GenerateSinglePosition(int* x = nullptr, int* y = nullptr);

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
bool BiTankCheck(TankInfo* ATank, TankInfo* BTank);
//Centers the camera over the Tank
void setCamera( SDL_Rect* camera ,TankInfo* UserTank);
#define XTANK_H
#endif
