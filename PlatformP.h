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
#include "xTank.h"

// TODO: Why the PlatformP won't recognize the Platform variable
// TODO: Try to figure out how the gDotTexture and gTileTexture is nullptr

#define real32 float
#define uint32 uint32_t

struct PlatformP {
    
   SDL_Rect gExplosionClips [ANIMATING_FRAMES];
   SDL_Rect gTileClips [TOTAL_TILE_SPRITES];

    XTexture* RedTexture;
    XTexture* GreenTexture;
    XTexture* BlueTexture;
    XTexture* ShimmerTexture;
    
    XTexture* gTileTexture;
    XTexture* gMenuTexture;
    XTexture* gTextTexture;
    XTexture* gUserTankTexture;
    XTexture* gUserBulletTexture;
    XTexture* gEnemyTankTexture;
    XTexture* gEnemyBulletTexture;
    XTexture* gExplosionTexture;

//The window we'll be rendering to
    SDL_Window* gWindow;
    SDL_Renderer* gRenderer;
    SDL_Event e;
    
//The window renderer
    PlatformP(){
        e = {};

        gTileTexture = nullptr;
        gTileTexture = new XTexture;

        // NOTE: Menu texture using the image or text string as the main display
        gMenuTexture = nullptr;
        gMenuTexture = new XTexture;
        
        gTextTexture = nullptr;
        gTextTexture = new XTexture;

        gUserTankTexture = nullptr;
        gUserTankTexture = new XTexture;

        gEnemyTankTexture = nullptr;
        gEnemyTankTexture = new XTexture;

        gEnemyBulletTexture = nullptr;
        gEnemyBulletTexture = new XTexture;

        gUserBulletTexture = nullptr;
        gUserBulletTexture = new XTexture;

        gTextTexture = nullptr;
        gTextTexture = new XTexture;

        gExplosionTexture = nullptr;
        gExplosionTexture = new XTexture;

        gWindow = NULL;
        gRenderer = NULL;};
};

// NOTE: BE CAREFUL this part (This can mess the whole thing up)
// #if !defined
// extern PlatformP Platform = {};
// #define PLATFORM
// #endif

//Frees media and shuts down SDL
bool init(PlatformP* Platform);    
bool LoadMedia(Tile* tiles, PlatformP* Platform);
void close(PlatformP* Platform);
bool IsArrow(SDL_Scancode KeyCode);
//Sets tiles from tile map
bool setTiles( Tile *tiles, PlatformP* Platform);
bool valid(SDL_Event e, KeyState* previousKey, KeyState* currentKey);
//Shows the Tank on the screen
void renderText(real32 FPS, const TankInfo* userTank, PlatformP* Platform);
void renderExplosionFrame(TankInfo* Tank, PlatformP* Platform, SDL_Rect* camera, int frame);
void renderTank(TankInfo* Tank, int frame, SDL_Rect& camera, PlatformP* Platform);

#define PLATFORMP_H
#endif
