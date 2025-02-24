/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */

#include "XTexture.h"
bool loadFromFile( std::string path, SDL_Renderer* gRenderer, int Width, int Height, XTexture* texture)
{
	//Get rid of preexisting texture
	free(texture);

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if (gRenderer == NULL){
		printf( "gRenderer is NULL %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );        
    } else {
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			texture->mWidth = Width;
			texture->mHeight = Height;
		}
 
		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}
        
    }

	//Return success
	texture->mTexture = newTexture;
	return texture->mTexture != NULL;
}

#if defined(SDL_TTF_MAJOR_VERSION)
bool loadFromRenderedText(char* textureText, float Scale, SDL_Color textColor, TTF_Font* gFont , SDL_Renderer* gRenderer, XTexture* texture)
{
	//Get rid of preexisting texture
	free(texture);

	//Render text surface
	SDL_Surface* textSurface = nullptr;
    textSurface = TTF_RenderText_Solid( gFont, textureText, textColor);
	if( textSurface != NULL )
	{
		//Create texture from surface pixels
        texture->mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( texture->mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
                if(Scale != 0.0f){
                    texture->mWidth = (int)(textSurface->w)* Scale;
                    texture->mHeight = (int)(textSurface->h)* Scale;                
                } else {
                    texture->mWidth = (int)(textSurface->w);
                    texture->mHeight = (int)(textSurface->h);                
                }
		}

		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	else
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}

	
	//Return success
	return texture->mTexture != NULL;
}
#endif

void free(XTexture* texture)
{
	//Free texture if it exists
	if( texture->mTexture != NULL )
	{
		texture->mWidth = 0;
		texture->mHeight = 0;
		SDL_DestroyTexture(texture->mTexture);
		texture->mTexture = NULL;
	}
}

void setColor( Uint8 red, Uint8 green, Uint8 blue, XTexture* texture )
{
	//Modulate texture rgb
	SDL_SetTextureColorMod( texture->mTexture, red, green, blue );
}

void setBlendMode( SDL_BlendMode blending, XTexture* texture)
{
	//Set blending function
	SDL_SetTextureBlendMode( texture->mTexture, blending );
}
		
void setAlpha( Uint8 alpha, XTexture* texture )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( texture->mTexture, alpha );
}

void render( SDL_Renderer* gRenderer, int x, int y, XTexture* texture, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	//Set rendering space and render to screen

	SDL_Rect renderQuad = { x, y, texture->mWidth, texture->mHeight };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx( gRenderer, texture->mTexture, clip, &renderQuad, angle, center, flip );
}
