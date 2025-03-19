/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */

#include <XTexture.h>
bool loadFromFile( std::string path, SDL_Renderer* gRenderer, int Width, int Height, XTexture* texture)
{
	//Get rid of preexisting texture
	xfree(texture);

	//The final texture
	SDL_Texture* newTexture = nullptr;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if (gRenderer == nullptr){
		printf( "gRenderer is nullptr %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );        
    } else {
	if( loadedSurface == nullptr )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );

		if( newTexture == nullptr )
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
	return texture->mTexture != nullptr;
}

#if defined(SDL_TTF_MAJOR_VERSION)
bool loadFromRenderedText(char* textureText, float ScaleW, float ScaleH, SDL_Color textColor, TTF_Font* gFont , SDL_Renderer* gRenderer, XTexture* texture)
{
	//Get rid of preexisting texture
	xfree(texture);

	//Render text surface
	SDL_Surface* textSurface = nullptr;
    textSurface = TTF_RenderText_Solid( gFont, textureText, textColor);
	if( textSurface != nullptr )
	{
		//Create texture from surface pixels
        texture->mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( texture->mTexture == nullptr )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
                    texture->mWidth = (int)(textSurface->w/1.5)* ScaleW;
                    // printf("Text Width is : %d\n", texture->mWidth);
                    texture->mHeight = (int)(textSurface->h/1.5)* ScaleH;
                    // printf("Text Height is : %d\n", texture->mHeight);
        }

		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	else
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}

	
	//Return success
	return texture->mTexture != nullptr;
}
#endif

void xfree(XTexture* texture)
{
	//Free texture if it exists
	if( texture->mTexture != nullptr )
	{
		texture->mWidth = 0;
		texture->mHeight = 0;

		SDL_DestroyTexture(texture->mTexture);
        texture->mTexture = nullptr;
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
	if( clip != nullptr )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	//Render to screen
	SDL_RenderCopyEx( gRenderer, texture->mTexture, clip, &renderQuad, angle, center, flip );
}
