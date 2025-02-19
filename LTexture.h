#ifndef LTexture_h

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
global_variable int SCREEN_WIDTH = 450;
global_variable int SCREEN_HEIGHT = 300;
global_variable TTF_Font* gFont = NULL;

//The dimensions of the level
const global_variable int LEVEL_WIDTH = 1800;
const global_variable int LEVEL_HEIGHT = 1200;

global_variable int TEXT_W = 200;
global_variable int TEXT_H = 200;

//Tile constants
const global_variable int TOTAL_TILES = 2400;
const global_variable int TOTAL_TILE_SPRITES = 13;

//Tile constants
const global_variable int BIG_TILE_WIDTH = 80;
const global_variable int BIG_TILE_HEIGHT = 80;

const global_variable int SMALL_TILE_WIDTH = 30;
const global_variable int SMALL_TILE_HEIGHT = 30;

//The different tile sprites(LAZYFOO's map)
// const global_variable int TILE_RED = 0;
// const global_variable int TILE_GREEN = 1;
// const global_variable int TILE_BLUE = 2;
// const global_variable int TILE_CENTER = 3;
// const global_variable int TILE_TOP = 4;
// const global_variable int TILE_TOPRIGHT = 5;
// const global_variable int TILE_RIGHT = 6;
// const global_variable int TILE_BOTTOMRIGHT = 7;
// const global_variable int TILE_BOTTOM = 8;
// const global_variable int TILE_BOTTOMLEFT = 9;
// const global_variable int TILE_LEFT = 10;
// const global_variable int TILE_TOPLEFT = 11;


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

// const global_variable int TILE_GREEN = 1;
// const global_variable int TILE_BLUE = 2;
// const global_variable int TILE_CENTER = 3;
// const global_variable int TILE_TOP = 4;
// const global_variable int TILE_TOPRIGHT = 5;
// const global_variable int TILE_RIGHT = 6;
// const global_variable int TILE_BOTTOMRIGHT = 7;
// const global_variable int TILE_BOTTOM = 8;
// const global_variable int TILE_BOTTOMLEFT = 9;
// const global_variable int TILE_LEFT = 10;
// const global_variable int TILE_TOPLEFT = 11;

const int ANIMATING_FRAMES = 10;

struct KeyState{
    bool pressed;
    SDL_Scancode key;
    bool repeat;
    int init;
    uint8_t type;
    KeyState(){
        pressed = false;
        key = (SDL_Scancode)0;  // NOTE: This is unknown button
        init = 0;
    };
};


//Texture wrapper class
class LTexture
{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
    bool loadFromFile( std::string path, SDL_Renderer* gRenderer, int Width = 0, int Height = 0);
		
		#if defined(SDL_TTF_MAJOR_VERSION)
		//Creates image from font string
    bool loadFromRenderedText(char* textureText, int Scale, SDL_Color textColor, TTF_Font* gFont , SDL_Renderer* gRenderer);
		#endif

		//Deallocates texture
		void free();

		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );
		
    //Renders texture at given point
    void render( SDL_Renderer* gRenderer = NULL, int x = 0, int y = 0, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

		//Gets image dimensions
		int getWidth();
		int getHeight();

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;

		//Image dimensions
		int mWidth;
		int mHeight;
};

#define LTexture_h
#endif
