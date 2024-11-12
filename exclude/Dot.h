#ifndef DOT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "LTexture.h"
#include "Tile.h"


//The dot that will move around on the screen
class Dot
{
    public:
		//The dimensions of the dot
		static const int DOT_WIDTH = 20;
		static const int DOT_HEIGHT = 20;

		//Maximum axis velocity of the dot
		static const int DOT_VEL = 10;

		//Initializes the variables
		Dot();

		//Takes key presses and adjusts the dot's velocity
		void handleEvent( SDL_Event& e );

		//Moves the dot and check collision against tiles
    void move( Tile *tiles[], bool checkCollsion );

		//Centers the camera over the dot
		void setCamera( SDL_Rect& camera );

		//Shows the dot on the screen
    void render(LTexture* gDotTexture ,SDL_Rect& camera, SDL_Rect* CurrentClip, SDL_Renderer* gRenderer);

    SDL_Rect* GetmBox();

    private:

		//Collision box of the dot
		SDL_Rect mBox;

		//The velocity of the dot
		int mVelX, mVelY;
};

#define DOT_H
#endif
