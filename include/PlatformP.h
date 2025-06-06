#if !defined(PLATFORMP_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */

#include <fstream>
#include <SDL2/SDL.h>
#include "XTexture.h"
#include "TileX.h"
#include "particle.h"

// TODO: Why the PlatformP won't recognize the Platform variable
// TODO: Try to figure out how the gDotTexture and gTileTexture is nullptr

#define real32 float
#define uint32 uint32_t

static uint8_t rendercountt = 0;
struct PlatformP {
    
    SDL_Rect gExplosionClips [EXPLOSION_FRAMES];
    SDL_Rect gMovingClips [MOVING_FRAMES];
    SDL_Rect gTileClips [TOTAL_TILE_SPRITES];

    XTexture* gTileTexture;
    XTexture* gMenuTexture;
    XTexture* gTextTexture;
    XTexture* gUserTankTexture;
    XTexture* gUserBulletTexture;
    XTexture* gEnemyTankTexture;
    XTexture* gEnemyBulletTexture;
    XTexture* gExplosionTexture;

    ParticlesTexture* particlestexture;

    int screen_w ;
    int screen_h ;
    
//The window we'll be rendering to
    SDL_Window* gWindow = nullptr;
    SDL_Renderer* gRenderer = nullptr;
    TTF_Font* gFont = nullptr;    
    SDL_Color TextColor = {0 , 0 , 0};
//The window renderer
    PlatformP(){
        screen_h = 0;
        screen_w = 0;
        
        gTileTexture = nullptr;
        gTileTexture = new XTexture();

        // NOTE: Menu texture using the image or text string as the main display
        gMenuTexture = nullptr;
        gMenuTexture = new XTexture();
        
        gTextTexture = nullptr;
        gTextTexture = new XTexture();

        gUserTankTexture = nullptr;
        gUserTankTexture = new XTexture();

        gEnemyTankTexture = nullptr;
        gEnemyTankTexture = new XTexture();

        gEnemyBulletTexture = nullptr;
        gEnemyBulletTexture = new XTexture();

        gUserBulletTexture = nullptr;
        gUserBulletTexture = new XTexture();

        gExplosionTexture = nullptr;
        gExplosionTexture = new XTexture();

        particlestexture = nullptr;
        particlestexture = new ParticlesTexture();
        
        if(gWindow!=nullptr){
            gWindow = nullptr;
        }
        if(gRenderer = nullptr){
            gRenderer = nullptr;
        }        
        };
};

// NOTE: BE CAREFUL this part (This can mess the whole thing up)
// #if !defined
// extern PlatformP Platform = {};
// #define PLATFORM
// #endif

//Frees media and shuts down SDL
bool init(PlatformP* Platform);    
SDL_Rect CreateFrame(int VerticalIndex, int HorizontalIndex);
bool LoadMedia(Tile* tiles, PlatformP* Platform);
void close(PlatformP* Platform);
bool IsArrow(SDL_Scancode KeyCode);
//Sets tiles from tile map
bool setTiles( Tile *tiles, PlatformP* Platform);
bool valid(SDL_Event e, KeyState* previousKey, KeyState* currentKey);
//Shows the Tank on the screen
void ConstructRectJoint(const SDL_Rect* rect, SDL_Point* Joint);
void DrawScaffold(SDL_Point(*Scaffold)[5], const SDL_Rect* Camera);
void renderText(real32 FPS, const TankInfo* userTank, PlatformP* Platform);
void renderExplosionFrame(TankInfo* Tank, PlatformP* Platform, SDL_Rect* camera,  uint8_t* frame);
void renderTank(TankInfo* Tank, int index, uint8_t* MovingFrame, SDL_Rect* camera, PlatformP* Platform);
void RenderParticle(Particle* pa, PlatformP* platform, TankInfo* userTank);

#define PLATFORMP_H
#endif
