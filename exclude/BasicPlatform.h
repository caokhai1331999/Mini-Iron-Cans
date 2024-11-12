#if !defined(BASICPLATFORM_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */

#include <fstream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "LTexture.h"
#include "Tile.h"
#include "Dot.h"

// NOTE: I done this using OOP styles class
// Next time I will try to using just the SIMD Procedure style on Platform to
// see whether the performance difference is that obvious

class BasicPlatform{

public:
    BasicPlatform();
    ~BasicPlatform();
    
//Frees media and shuts down SDL
    void close( Tile* tiles[]);
    bool LoadMedia(Tile* tiles[]);
    bool init();    
//Sets tiles from tile map
    bool setTiles( Tile *tiles[]);
    SDL_Renderer* GetRenderer();
    SDL_Window* GetWindow();
    LTexture* GetgTileTexture();
    LTexture* GetgDotTexture();
    LTexture* GetgTextTexture();
    SDL_Rect* GetgTileClips();
    SDL_Rect* GetgSpriteClips();
    
private :
    
   SDL_Rect gSpriteClips [ANIMATING_FRAMES];

   SDL_Rect gTileClips [TOTAL_TILE_SPRITES];

    LTexture* gTileTexture;
    LTexture* gDotTexture;
    LTexture* gTextTexture;

//The window we'll be rendering to
    SDL_Window* gWindow;

//The window renderer
    SDL_Renderer* gRenderer;
};

#define BASICPLATFORM_H
#endif
