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
#include "LTexture.h"
#include "xTank.h"
#include "Tile.h"

// TODO: Why the PlatformP won't recognize the Platform variable
// TODO: Try to figure out how the gDotTexture and gTileTexture is nullptr

#define real32 float
#define uint32 uint32_t


struct PlatformP {

   const char* menuChoices [] = {"NEW GAME","RESUME","OPTIONS","EXIT"}; 
    
   SDL_Rect gExplosionClips [ANIMATING_FRAMES];
   SDL_Rect gTileClips [TOTAL_TILE_SPRITES];

    LTexture* gMenuTexture;
    LTexture* gTileTexture;
    LTexture* gTextTexture;
    LTexture* gUserTankTexture;
    LTexture* gUserBulletTexture;
    LTexture* gEnemyTankTexture;
    LTexture* gEnemyBulletTexture;
    LTexture* gExplosionTexture;

//The window we'll be rendering to
    SDL_Window* gWindow;
    SDL_Renderer* gRenderer;
    SDL_Event e;
    
//The window renderer
    PlatformP(){

        gTileTexture = nullptr;
        gTileTexture = new LTexture;

        // NOTE: Menu texture using the image or text string as the main display
        gMenuTexture = nullptr;
        gMenuTexture = new LTexture;
        
        // gDotTexture = nullptr;
        // gDotTexture = new LTexture;
        gTextTexture = nullptr;
        gTextTexture = new LTexture;

        gUserTankTexture = nullptr;
        gUserTankTexture = new LTexture;

        gEnemyTankTexture = nullptr;
        gEnemyTankTexture = new LTexture;

        gEnemyBulletTexture = nullptr;
        gEnemyBulletTexture = new LTexture;

        gUserBulletTexture = nullptr;
        gUserBulletTexture = new LTexture;

        gTextTexture = nullptr;
        gTextTexture = new LTexture;

        gExplosionTexture = nullptr;
        gExplosionTexture = new LTexture;

        gWindow = NULL;
        gRenderer = NULL;};
};

// NOTE: BE CAREFUL this part (This can mess the whole thing up)
// #if !defined
// extern PlatformP Platform = {};
// #define PLATFORM
// #endif

//Frees media and shuts down SDL
bool init();    
bool LoadMedia(Tile* tiles[]);
void close( Tile* tiles[]);
bool IsArrow(SDL_Scancode KeyCode);
//Sets tiles from tile map
bool setTiles( Tile *tiles[]);
bool valid(SDL_Event e, KeyState* previousKey, KeyState* currentKey);
//Shows the Tank on the screen
void render(TankInfo* Tank, int frame, SDL_Rect& camera );
void renderText(real32 FPS, const TankInfo* userTank);

#define PLATFORMP_H
#endif
