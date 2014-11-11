/*This source code copyrighted by Lazy Foo' Productions (2004-2013)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, math, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image as texture
SDL_Texture* loadTexture( std::string path );

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Nothing to load
	return success;
}

void close()
{
	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* loadTexture( std::string path )
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	return newTexture;
}

int main( int argc, char* args[] )
{
    int counter = 1;

    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
    SDL_RenderClear( gRenderer );

    ifstream levelOneFile;

    string tempString = "";
    string fileString = "";

    fileString = "levelOne.txt";

    levelOneFile.open(fileString.c_str());

    if(levelOneFile.is_open())
    {
        while(getline(levelOneFile,tempString))
        {
            for(int i = 0; i < tempString.length(); i++)
            {
                if(tempString[i] == '~')
                {
                    cout << tempString[i];
                }
                else if(tempString[i] == '#')
                {
                    cout << tempString[i];
                    SDL_Rect fillRect = { SCREEN_WIDTH / counter, SCREEN_HEIGHT / counter, SCREEN_WIDTH / counter, SCREEN_HEIGHT / counter };
                    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );
                    SDL_RenderFillRect( gRenderer, &fillRect );
                }
                else if(tempString[i] == '+')
                {
                    cout << tempString[i];
                }
                else if(tempString[i] == '*')
                {
                    cout << tempString[i];
                }
                else if(tempString[i] == '@')
                {
                    cout << tempString[i];
                }
                else if(tempString[i] == '.')
                {
                    cout << tempString[i];
                }
                else if(tempString[i] == '$')
                {
                    cout << tempString[i];
                }

            }
            cout << endl;
        }
    }


	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
				}

				//Clear screen
				//SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				//SDL_RenderClear( gRenderer );

				int counter = 1;

				//Render red filled quad
				//SDL_Rect fillRect = { SCREEN_WIDTH / counter, SCREEN_HEIGHT / counter, SCREEN_WIDTH / counter, SCREEN_HEIGHT / counter };
				//SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );
				//SDL_RenderFillRect( gRenderer, &fillRect );

				//Render green outlined quad
				//SDL_Rect outlineRect = { SCREEN_WIDTH / 6, SCREEN_HEIGHT / 6, SCREEN_WIDTH * 2 / 3, SCREEN_HEIGHT * 2 / 3 };
				//SDL_SetRenderDrawColor( gRenderer, 0x00, 0xFF, 0x00, 0xFF );
				//SDL_RenderDrawRect( gRenderer, &outlineRect );
    /*
    ifstream levelOneFile;

    string tempString = "";
    string fileString = "";

    fileString = "levelOne.txt";

    levelOneFile.open(fileString.c_str());

    if(levelOneFile.is_open())
    {
        while(getline(levelOneFile,tempString))
        {
            for(int i = 0; i < tempString.length(); i++)
            {
                if(tempString[i] == '~')
                {
                    cout << tempString[i];
                }
                else if(tempString[i] == '#')
                {
                    cout << tempString[i];
                    SDL_Rect fillRect = { SCREEN_WIDTH / counter, SCREEN_HEIGHT / counter, SCREEN_WIDTH / counter, SCREEN_HEIGHT / counter };
                    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );
                    SDL_RenderFillRect( gRenderer, &fillRect );
                }
                else if(tempString[i] == '+')
                {
                    cout << tempString[i];
                    SDL_Rect outlineRect = { SCREEN_WIDTH / 6, SCREEN_HEIGHT / 6, SCREEN_WIDTH * 2 / 3, SCREEN_HEIGHT * 2 / 3 };
                    SDL_SetRenderDrawColor( gRenderer, 0x00, 0xFF, 0x00, 0xFF );
                    SDL_RenderDrawRect( gRenderer, &outlineRect );

                }
                else if(tempString[i] == '*')
                {
                    cout << tempString[i];
                }
                else if(tempString[i] == '@')
                {
                    cout << tempString[i];

                }
                else if(tempString[i] == '.')
                {
                    cout << tempString[i];
                }
                else if(tempString[i] == '$')
                {
                    cout << tempString[i];
                }

            }
            cout << endl;
            counter++;
        }
    }
    */


				//Draw blue horizontal line
				//SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0xFF, 0xFF );
				//SDL_RenderDrawLine( gRenderer, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2 );

				//Draw vertical line of yellow dots
				/*
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0x00, 0xFF );
				for( int i = 0; i < SCREEN_HEIGHT; i += 4 )
				{
					SDL_RenderDrawPoint( gRenderer, SCREEN_WIDTH / 2, i );
				}*/

				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
