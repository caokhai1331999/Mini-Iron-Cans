#if !defined(XTEXTURE_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */

#include <iostream>
#include <cstring>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define internal static
#define local_persist static
#define global_variable static

#define uint32 uint32_t
#define uint8 uint8_t
#define real32 float

//Screen dimension constants
global_variable int DEFAULT_SCREEN_WIDTH = 270;
global_variable int DEFAULT_SCREEN_HEIGHT = 180;

//The dimensions of the level
const global_variable int LEVEL_WIDTH = 1080;
const global_variable int LEVEL_HEIGHT = 720;

global_variable int TEXT_W = 120;
global_variable int TEXT_H = 120;

//Tile constants
const global_variable int TOTAL_TILES = 2400;
const global_variable int TOTAL_TILE_SPRITES = 13;

//Tile constants
const global_variable int BIG_TILE_WIDTH = 48;
const global_variable int BIG_TILE_HEIGHT = 48;

const global_variable int SMALL_TILE_WIDTH = 18;
const global_variable int SMALL_TILE_HEIGHT = 18;

//The different tile sprites(Different map)
// const global_variable int BIG_GRASS_TILE = 0;
const global_variable int SMALL_GRASS_TILE_1 = 0;
const global_variable int SMALL_GRASS_TILE_2 = 1;
const global_variable int SMALL_GRASS_TILE_3 = 2;
const global_variable int SMALL_GRASS_TILE_4 = 3;
const global_variable int SMALL_GRASS_TILE_5 = 4;
const global_variable int SMALL_GRASS_TILE_6 = 5;

const global_variable int SMALL_STUMP_TILE_1 = 6;
const global_variable int SMALL_STUMP_TILE_2 = 7;
const global_variable int SMALL_STUMP_TILE_3 = 8;
const global_variable int SMALL_STUMP_TILE_4 = 9;

const global_variable int SMALL_WATER_1 = 10;
const global_variable int SMALL_WATER_2 = 11;

const global_variable int SMALL_WOOD_PATH_1 = 12;

const int EXPLOSION_FRAMES = 10;
const int MOVING_FRAMES = 2;

struct KeyState{
    bool pressed;
    SDL_Scancode key;
    bool repeat;
    int init;
    uint32_t type;
    KeyState(){
        pressed = false;
        key = (SDL_Scancode)0;  // NOTE: This is unknown button
        init = 0;
    };
};

struct XTexture{
    //The actual hardware texture
    SDL_Texture* mTexture = nullptr;

    //Image dimensions
    int mWidth = 0;
    int mHeight = 0;

    XTexture(){
        if(mTexture != nullptr){
            mTexture = nullptr;
        }
        mWidth = 0;
        mHeight = 0;
    };
};

//Loads image at specified path
bool loadFromFile( std::string path, SDL_Renderer* gRenderer, int Width = 0, int Height = 0, XTexture* texture = nullptr);
		
#if defined(SDL_TTF_MAJOR_VERSION)
//Creates image from font string
bool loadFromRenderedText(char* textureText = nullptr, float scaleW = 1.0f, float scaleH = 1.0f, SDL_Color textColor = {0, 0, 0}, TTF_Font* gFont = nullptr, SDL_Renderer* gRenderer = nullptr, XTexture* texture = nullptr);
#endif

//Set color modulation
void setColor( Uint8 red, Uint8 green, Uint8 blue );

//Set blending
void setBlendMode( SDL_BlendMode blending );

//Set alpha modulation
void setAlpha( Uint8 alpha );
		
//Renders texture at given point
void render( SDL_Renderer* gRenderer = nullptr, int x = 0, int y = 0, XTexture* texture = nullptr, SDL_Rect* clip = nullptr, double angle = 0.0, SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE);

//Deallocates texture
void xfree(XTexture* texture = nullptr);

#define XTEXTURE_H
#endif
