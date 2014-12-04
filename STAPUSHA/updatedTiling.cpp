/*This source code copyrighted by Lazy Foo' Productions (2004-2014)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, strings, and file streams
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>


using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 675;
const int SCREEN_HEIGHT = 616;

//The dimensions of the level
const int LEVEL_WIDTH = 675;
const int LEVEL_HEIGHT = 4950;

//Tile constants
const int TILE_WIDTH = 80;
const int TILE_HEIGHT = 80;
const int TOTAL_TILES = 693;
const int TOTAL_TILE_SPRITES = 12;

//The different tile sprites
const int TILE_RED = 0;
const int TILE_GREEN = 1;
const int TILE_BLUE = 2;
const int TILE_CENTER = 3;
const int TILE_TOP = 4;
const int TILE_TOPRIGHT = 5;
const int TILE_RIGHT = 6;
const int TILE_BOTTOMRIGHT = 7;
const int TILE_BOTTOM = 8;
const int TILE_BOTTOMLEFT = 9;
const int TILE_LEFT = 10;
const int TILE_TOPLEFT = 11;

//Texture wrapper class
class LTexture
{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( std::string path );

		#ifdef _SDL_TTF_H
		//Creates image from font string
		bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
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
		void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

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

//The tile
class Tile
{
    public:
		//Initializes position and type
		Tile( int x, int y, int tileType );

		//Shows the tile
		void render( SDL_Rect& camera );

		//Get the tile type
		int getType();

		//Get the collision box
		SDL_Rect getBox();

    private:
		//The attributes of the tile
		SDL_Rect mBox;

		//The tile type
		int mType;
};

//The dot that will move around on the screen
class Dot
{
    public:
		//The dimensions of the dot
		static const int DOT_WIDTH = 34;
		static const int DOT_HEIGHT = 36;

		//Maximum axis velocity of the dot
		//static const int DOT_VEL = 8;

		//Initializes the variables
		Dot();

		//Takes key presses and adjusts the dot's velocity
		int handleEvent( SDL_Event& e , Tile *tileSet[]);

		//Moves the dot and check collision against tiles
		void move( Tile *tiles[], int direction );

		//Centers the camera over the dot
		void setCamera( SDL_Rect& camera );

		//Shows the dot on the screen
		void render( SDL_Rect& camera );

		int getX ();

		int getY();

		void setPosition(int, int);

    private:
		//Collision box of the dot
		SDL_Rect mBox;

		//The velocity of the dot
		int mVelX, mVelY;
};

class Star
{
    public:
		//The dimensions of the star
		static const int DOT_WIDTH = 34;
		static const int DOT_HEIGHT = 36;

		//Initializes the variables
		Star();
		Star(int, int);

		//Moves the star and check collision against tiles
		int move( Tile *tiles[], int direction );

		//Shows the star on the screen
		void render( SDL_Rect& camera );

		int getX();

		int getY();

		void setPosition(int, int);

    private:
		//Collision box of the star
		SDL_Rect mBox;

		//The velocity of the star
		int mVelX, mVelY;
};

class Goal
{
    public:
		//The dimensions of the dot
		static const int DOT_WIDTH = 75;
		static const int DOT_HEIGHT = 56;

		//Maximum axis velocity of the dot
		//static const int DOT_VEL = 8;

		//Initializes the variables
		Goal();
		Goal(int, int);

		//Shows the goal on the screen
		void render( SDL_Rect& camera );

		int getX();

		int getY();

		void setActive(int, int);

		void setOff();

		bool getActive();

    private:
		//Collision box of the goal
		SDL_Rect mBox;

		bool isActive;
};

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* gHelloWorld = NULL;
SDL_Surface* gSolved  = NULL;

//Starts up SDL and creates window
bool init();

bool initMenu()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Get window surface
			gScreenSurface = SDL_GetWindowSurface( gWindow );
		}
	}

	return success;
}

//Loads media
bool loadMedia( Tile* tiles[] );

bool loadMediaMenu()
{
	//Loading success flag
	bool success = true;

	//Load splash image
	gHelloWorld = SDL_LoadBMP( "hello_world.bmp" );
	if( gHelloWorld == NULL )
	{
		printf( "Unable to load image %s! SDL Error: %s\n", "hello_world.bmp", SDL_GetError() );
		success = false;
	}
	gSolved = SDL_LoadBMP( "solvedscreen.bmp" );
	if( gSolved == NULL )
	{
		printf( "Unable to load image %s! SDL Error: %s\n", "hello_world.bmp", SDL_GetError() );
		success = false;
	}

	return success;
}


//Frees media and shuts down SDL
void close( Tile* tiles[] );

void close()
{
	//Deallocate surface
	SDL_FreeSurface( gHelloWorld );
	gHelloWorld = NULL;

	//Destroy window
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
}

//Box collision detector
bool checkCollision( SDL_Rect a, SDL_Rect b );

//Checks collision box against set of tiles
bool touchesWall( SDL_Rect box, Tile* tiles[] );

//Sets tiles from tile map
bool setTiles( Tile *tiles[] );

//The window we'll be rendering to
//SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Scene textures
LTexture gDotTexture;
LTexture gTileTexture;
LTexture gStarTexture;
LTexture gGoalOffTexture;
LTexture gGoalOnTexture;
SDL_Rect gTileClips[ TOTAL_TILE_SPRITES ];


SDL_Surface* gCurrentSurface = NULL;

//SDL_Surface* gScreenSurface = NULL;

SDL_Surface* loadSurface( std::string path );

LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile( std::string path )
{
	//Get rid of preexisting texture
	free();

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
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 255, 255 ) );

		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}


void LTexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	//Modulate texture rgb
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
	//Set blending function
	SDL_SetTextureBlendMode( mTexture, blending );
}

void LTexture::setAlpha( Uint8 alpha )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

SDL_Surface* loadSurface( std::string path )
{
	//Load image at specified path
	SDL_Surface* loadedSurface = SDL_LoadBMP( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
	}

	return loadedSurface;
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

Tile::Tile( int x, int y, int tileType )
{
    //Get the offsets
    mBox.x = x;
    mBox.y = y;

    //Set the collision box
    mBox.w = TILE_WIDTH;
    mBox.h = TILE_HEIGHT;

    //Get the tile type
    mType = tileType;
}

void Tile::render( SDL_Rect& camera )
{
    //If the tile is on screen
    if( checkCollision( camera, mBox ) )
    {
        //Show the tile
        gTileTexture.render( mBox.x - camera.x, mBox.y - camera.y, &gTileClips[ mType ] );
    }
}

int Tile::getType()
{
    return mType;
}

SDL_Rect Tile::getBox()
{
    return mBox;
}

Goal::Goal()
{

    mBox.x = 97;
    mBox.y = 67;
	mBox.w = DOT_WIDTH;
	mBox.h = DOT_HEIGHT;

}

Goal::Goal(int X, int Y)
{

    mBox.x = X;
    mBox.y = Y;
    mBox.w = DOT_WIDTH;
	mBox.h = DOT_HEIGHT;

}

void Goal::render( SDL_Rect& camera )
{

    if (isActive == false)
    {
        gGoalOffTexture.render( mBox.x - camera.x, mBox.y - camera.y );
    }

    else
    {
        gGoalOnTexture.render ( mBox.x - camera.x, mBox.y - camera.y );
    }

}

int Goal::getX()
{
    return mBox.x;
}

int Goal::getY()
{
    return mBox.y;
}


void Goal::setActive(int X, int Y)
{

    if (X == (mBox.x + 22) && Y == (mBox.y + 11 ))
    {
        isActive = true;
    }

}

void Goal::setOff()
{
    isActive = false;
}

bool Goal::getActive()
{
    return isActive;
}

Dot::Dot()
{
    //Initialize the collision box
    mBox.x = 97;
    mBox.y = 67;
	mBox.w = DOT_WIDTH;
	mBox.h = DOT_HEIGHT;

    //Initialize the velocity
    mVelX = 75;
    mVelY = 56;
}

Star::Star()
{

   //Initialize the collision box
    mBox.x = 97;
    mBox.y = 67;
	mBox.w = DOT_WIDTH;
	mBox.h = DOT_HEIGHT;

    //Initialize the velocity
    mVelX = 75;
    mVelY = 56;

}

Star::Star(int X, int Y)
{

    mBox.x = X;
    mBox.y = Y;
	mBox.w = DOT_WIDTH;
	mBox.h = DOT_HEIGHT;
	mVelX = 75;
	mVelY = 56;

}

int Dot::handleEvent( SDL_Event& e , Tile *tileSet[])
{
    //If a key was pressed
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: move(tileSet, 1); return 1;break;
            case SDLK_DOWN: move(tileSet, 2); return 2; break;
            case SDLK_LEFT: move(tileSet, 3); return 3; break;
            case SDLK_RIGHT: move(tileSet, 4); return 4; break;
            case SDLK_r: return 5; break;
            case SDLK_q: return 6; break;
        }
    }
}

void Dot::move( Tile *tiles[], int direction )
{
    switch(direction)
    {
    case 1:

            mBox.y -= mVelY;

            //If the dot went too far up or down or touched a wall
            if( ( mBox.y < 0 ) || ( mBox.y + DOT_HEIGHT > LEVEL_HEIGHT ) || touchesWall( mBox, tiles ) )
            {
                //move back
                mBox.y += mVelY;
            }
        break;

    case 2:
        {
            mBox.y += mVelY;

            //If the dot went too far up or down or touched a wall
            if( ( mBox.y < 0 ) || ( mBox.y + DOT_HEIGHT > LEVEL_HEIGHT ) || touchesWall( mBox, tiles ) )
            {
                //move back
                mBox.y -= mVelY;
            }
        }break;

    case 3:
        {
            mBox.x -= mVelX;

            //If the dot went too far to the left or right or touched a wall
            if( ( mBox.x < 0 ) || ( mBox.x + DOT_WIDTH > LEVEL_WIDTH ) || touchesWall( mBox, tiles ) )
            {
                //move back
                mBox.x += mVelX;
            }
        }break;

    case 4:
        {
            mBox.x += mVelX;

            //If the dot went too far to the left or right or touched a wall
            if( ( mBox.x < 0 ) || ( mBox.x + DOT_WIDTH > LEVEL_WIDTH ) || touchesWall( mBox, tiles ) )
            {
                //move back
                mBox.x -= mVelX;
            }
        }break;

    }

}

int Star::move( Tile *tiles[], int direction )
{
    switch(direction)
    {
    case 1:

            mBox.y -= mVelY;

            //If the dot went too far up or down or touched a wall
            if( ( mBox.y < 0 ) || ( mBox.y + DOT_HEIGHT > LEVEL_HEIGHT ) || touchesWall( mBox, tiles ) )
            {
                //move back
                mBox.y += mVelY;
                return 1;
            }
            return 0;
        break;

    case 2:
        {
            mBox.y += mVelY;

            //If the dot went too far up or down or touched a wall
            if( ( mBox.y < 0 ) || ( mBox.y + DOT_HEIGHT > LEVEL_HEIGHT ) || touchesWall( mBox, tiles ) )
            {
                //move back
                mBox.y -= mVelY;
                return 1;
            }
            return 0;
        }break;

    case 3:
        {
            mBox.x -= mVelX;

            //If the dot went too far to the left or right or touched a wall
            if( ( mBox.x < 0 ) || ( mBox.x + DOT_WIDTH > LEVEL_WIDTH ) || touchesWall( mBox, tiles ) )
            {
                //move back
                mBox.x += mVelX;
                return 1;
            }
            return 0;
        }break;

    case 4:
        {
            mBox.x += mVelX;

            //If the dot went too far to the left or right or touched a wall
            if( ( mBox.x < 0 ) || ( mBox.x + DOT_WIDTH > LEVEL_WIDTH ) || touchesWall( mBox, tiles ) )
            {
                //move back
                mBox.x -= mVelX;
                return 1;
            }
            return 0;
        }break;

    }

}

void Dot::setCamera( SDL_Rect& camera )
{
	//Center the camera over the dot
	camera.x = ( mBox.x + DOT_WIDTH / 2 ) - SCREEN_WIDTH / 2;
	camera.y = ( mBox.y + DOT_HEIGHT / 2 ) - SCREEN_HEIGHT / 2;

	//Keep the camera in bounds
	if( camera.x < 0 )
	{
		camera.x = 0;
	}
	if( camera.y < 0 )
	{
		camera.y = 0;
	}
	if( camera.x > LEVEL_WIDTH - camera.w )
	{
		camera.x = LEVEL_WIDTH - camera.w;
	}
	if( camera.y > LEVEL_HEIGHT - camera.h )
	{
		camera.y = LEVEL_HEIGHT - camera.h;
	}
}

void Dot::render( SDL_Rect& camera )
{
    //Show the dot
	gDotTexture.render( mBox.x - camera.x, mBox.y - camera.y );

}

void Star::render( SDL_Rect& camera )
{
    //Show the dot
	gStarTexture.render( mBox.x - camera.x, mBox.y - camera.y );

}

int Dot::getX()
{
    return mBox.x;
}

int Dot::getY()
{
    return mBox.y;
}

int Star::getX()
{
    return mBox.x;
}

int Star::getY()
{
    return mBox.y;
}

void Dot::setPosition(int X, int Y)
{
    mBox.x = X;
    mBox.y = Y;
}

void Star::setPosition(int X, int Y)
{
    mBox.x = X;
    mBox.y = Y;
}



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
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
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

bool loadMedia( Tile* tiles[] )
{
	//Loading success flag
	bool success = true;

	/*gKeyPressSurfaces[ KEY_PRESS_SURFACE_A ] = loadSurface( "04_key_presses/press.bmp" );
	if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_A ] == NULL )
	{
		printf( "Failed to load default image!\n" );
		success = false;
	}*/

	//Load dot texture
	if( !gDotTexture.loadFromFile( "39_tiling/dot.bmp" ) )
	{
		printf( "Failed to load dot texture!\n" );
		success = false;
	}

	if( !gStarTexture.loadFromFile( "39_tiling/star.png" ) )
	{
		printf( "Failed to load star texture!\n" );
		success = false;
	}

    //Load tile texture
	if( !gGoalOffTexture.loadFromFile( "39_tiling/goaloff.png" ) )
	{
		printf( "Failed to load tile set texture!\n" );
		success = false;
	}

    if( !gGoalOnTexture.loadFromFile( "39_tiling/goalon.png" ) )
	{
		printf( "Failed to load tile set texture!\n" );
		success = false;
	}


	//Load tile texture
	if( !gTileTexture.loadFromFile( "39_tiling/tiles.png" ) )
	{
		printf( "Failed to load tile set texture!\n" );
		success = false;
	}

	//Load tile map
	if( !setTiles( tiles ) )
	{
		printf( "Failed to load tile set!\n" );
		success = false;
	}

	return success;
}

void close( Tile* tiles[] )
{
	//Deallocate tiles
	for( int i = 0; i < TOTAL_TILES; ++i )
	{
		 if( tiles[ i ] == NULL )
		 {
			delete tiles[ i ];
			tiles[ i ] = NULL;
		 }
	}

	//Free loaded images
	gDotTexture.free();
	gTileTexture.free();
	gStarTexture.free();
	gGoalOffTexture.free();
	gGoalOnTexture.free();

	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

bool checkCollision( SDL_Rect a, SDL_Rect b )
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = (a.x + a.w);
    topA = a.y+24;
    bottomA = (a.y + a.h)+24;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = (b.x + b.w);
    topB = b.y+24;
    bottomB = (b.y + b.h)-24;

    //If any of the sides from A are outside of B
    if( bottomA <= topB )
    {
        return false;
    }

    if( topA >= bottomB )
    {
        return false;
    }

    if( rightA <= leftB )
    {
        return false;
    }

    if( leftA >= rightB )
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}

bool setTiles( Tile* tiles[] )
{
	//Success flag
	bool tilesLoaded = true;

    //The tile offsets
    int x = 0, y = 0;

    //Open the map
    std::ifstream map( "39_tiling/levelOne.map" );

    //If the map couldn't be loaded
    if( map == NULL )
    {
		printf( "Unable to load map file!\n" );
		tilesLoaded = false;
    }
	else
	{
		//Initialize the tiles
		for( int i = 0; i < TOTAL_TILES; ++i )
		{
			//Determines what kind of tile will be made
			int tileType = -1;

			//Read tile from map file
			map >> tileType;

			//If the was a problem in reading the map
			if( map.fail() )
			{
				//Stop loading map
				printf( "Error loading map: Unexpected end of file!\n" );
				tilesLoaded = false;
				break;
			}

			//If the number is a valid tile number
			if( ( tileType >= 0 ) && ( tileType < TOTAL_TILE_SPRITES ) )
			{
				tiles[ i ] = new Tile( x, y, tileType );
			}
			//If we don't recognize the tile type
			else
			{
				//Stop loading map
				printf( "Error loading map: Invalid tile type at %d!\n", i );
				tilesLoaded = false;
				break;
			}

			//Move to next tile spot
			x += TILE_WIDTH;
			x = x - 5;

			//If we've gone too far
			if( x >= LEVEL_WIDTH )
			{
				//Move back
				x = 0;

				//Move to the next row
				y += TILE_HEIGHT;
				y = y-24;
			}
		}

		//Clip the sprite sheet
		if( tilesLoaded )
		{
			gTileClips[ TILE_RED ].x = 0;
			gTileClips[ TILE_RED ].y = 0;
			gTileClips[ TILE_RED ].w = TILE_WIDTH;
			gTileClips[ TILE_RED ].h = TILE_HEIGHT;

			gTileClips[ TILE_GREEN ].x = 0;
			gTileClips[ TILE_GREEN ].y = 80;
			gTileClips[ TILE_GREEN ].w = TILE_WIDTH;
			gTileClips[ TILE_GREEN ].h = TILE_HEIGHT;

			gTileClips[ TILE_BLUE ].x = 0;
			gTileClips[ TILE_BLUE ].y = 160;
			gTileClips[ TILE_BLUE ].w = TILE_WIDTH;
			gTileClips[ TILE_BLUE ].h = TILE_HEIGHT;

			gTileClips[ TILE_TOPLEFT ].x = 80;
			gTileClips[ TILE_TOPLEFT ].y = 0;
			gTileClips[ TILE_TOPLEFT ].w = TILE_WIDTH;
			gTileClips[ TILE_TOPLEFT ].h = TILE_HEIGHT;

			gTileClips[ TILE_LEFT ].x = 80;
			gTileClips[ TILE_LEFT ].y = 80;
			gTileClips[ TILE_LEFT ].w = TILE_WIDTH;
			gTileClips[ TILE_LEFT ].h = TILE_HEIGHT;

			gTileClips[ TILE_BOTTOMLEFT ].x = 80;
			gTileClips[ TILE_BOTTOMLEFT ].y = 160;
			gTileClips[ TILE_BOTTOMLEFT ].w = TILE_WIDTH;
			gTileClips[ TILE_BOTTOMLEFT ].h = TILE_HEIGHT;

			gTileClips[ TILE_TOP ].x = 160;
			gTileClips[ TILE_TOP ].y = 0;
			gTileClips[ TILE_TOP ].w = TILE_WIDTH;
			gTileClips[ TILE_TOP ].h = TILE_HEIGHT;

			gTileClips[ TILE_CENTER ].x = 160;
			gTileClips[ TILE_CENTER ].y = 80;
			gTileClips[ TILE_CENTER ].w = TILE_WIDTH;
			gTileClips[ TILE_CENTER ].h = TILE_HEIGHT;

			gTileClips[ TILE_BOTTOM ].x = 160;
			gTileClips[ TILE_BOTTOM ].y = 160;
			gTileClips[ TILE_BOTTOM ].w = TILE_WIDTH;
			gTileClips[ TILE_BOTTOM ].h = TILE_HEIGHT;

			gTileClips[ TILE_TOPRIGHT ].x = 240;
			gTileClips[ TILE_TOPRIGHT ].y = 0;
			gTileClips[ TILE_TOPRIGHT ].w = TILE_WIDTH;
			gTileClips[ TILE_TOPRIGHT ].h = TILE_HEIGHT;

			gTileClips[ TILE_RIGHT ].x = 240;
			gTileClips[ TILE_RIGHT ].y = 80;
			gTileClips[ TILE_RIGHT ].w = TILE_WIDTH;
			gTileClips[ TILE_RIGHT ].h = TILE_HEIGHT;

			gTileClips[ TILE_BOTTOMRIGHT ].x = 240;
			gTileClips[ TILE_BOTTOMRIGHT ].y = 160;
			gTileClips[ TILE_BOTTOMRIGHT ].w = TILE_WIDTH;
			gTileClips[ TILE_BOTTOMRIGHT ].h = TILE_HEIGHT;
		}
	}

    //Close the file
    map.close();

    //If the map was loaded fine
    return tilesLoaded;
}

bool touchesWall( SDL_Rect box, Tile* tiles[] )
{
    //Go through the tiles
    for( int i = 0; i < TOTAL_TILES; ++i )
    {
        //If the tile is a wall type tile
        if( ( tiles[ i ]->getType() >= TILE_CENTER ) && ( tiles[ i ]->getType() <= TILE_TOPLEFT ) || ( tiles[ i ]->getType() == TILE_RED ) || ( tiles[ i ]->getType() == TILE_BLUE ) )
        {
            //If the collision box touches the wall tile
            if( checkCollision( box, tiles[ i ]->getBox() ) )
            {
                return true;
            }
        }
    }

    //If no wall tiles were touched
    return false;
}

void DotOnStar(Dot *dot, Star *star, Tile *tileSet[], int movement );
void starOnStar(Dot *dot, Star *star, Star *star2, Tile *tileSet[], int movement );
void solve();

int main( int argc, char* args[] )
{

    goto LOLBOWEBEMAD;





    LOLBOWEBEMAD:
	//Start up SDL and create window
	if( !initMenu() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMediaMenu() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
			//Apply the image
			SDL_BlitSurface( gHelloWorld, NULL, gScreenSurface, NULL );

			//Update the surface
			SDL_UpdateWindowSurface( gWindow );

			SDL_Delay( 5000) ;
			close();

		}
	}
    if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
	    SDL_Event e;
		//The level tiles
		Tile* tileSet[ TOTAL_TILES ];

		//Load media
		if( !loadMedia( tileSet ) )
		{
			printf( "Failed to load media!\n" );
			cout << "failed to load media" << endl;
		}
		else
		{
			//Main loop flag
			bool quit = false;

			int num = 0;

			//Event handler
			SDL_Event e;

			int level = 1;

			//The dot that will be moving around on the screen
			Dot dot;

			//Level 1 Code Block
			Star level1Star1 (172, 235);
			Star level1Star2 (172,123);
            Goal level1Goal1 (150,168);
			Goal level1Goal2 (150, 336);

			//Level 2 Code Block
			Star level2Star1 (322, 1019);
			Star level2Star2 (322, 963);
			Goal level2Goal1 (375, 1680);
			Goal level2Goal2 (375, 1736);

			//Level 3 Code Block
			Star level3Star1 (172, 2139);
			Goal level3Goal1 (450, 2520);

			//Level 4 Code Block
			Star level4Star1 (322, 2979);
			Star level4Star2 (247, 2923);
			Star level4Star3 (397, 2923);
			Goal level4Goal1 (75, 2912);
			Goal level4Goal2 (525, 2912);
			Goal level4Goal3 (300, 3248);

			//Level 5 Code Block
			Star level5Star1 (322, 3539);
			Star level5Star2 (547, 3707);
			Goal level5Goal1 (300, 3472);
			Goal level5Goal2 (525, 3976);



			//Level camera
			SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

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

					//Handle input for the dot
					int movement = dot.handleEvent( e, tileSet );

					if( movement == 5 )
					{
					    if (level == 1)
                        {
                            dot.setPosition(97,67);
                            level1Star1.setPosition(172, 235);
                            level1Star2.setPosition(172,123);
                        }
                        else if (level == 2)
                        {
                            dot.setPosition(172, 1019);
                            level2Star1.setPosition(322, 1019);
                            level2Star2.setPosition(322, 963);
                        }
                        else if (level == 3)
                        {
                            dot.setPosition(172, 2083);
                            level3Star1.setPosition (172, 2139);
                        }
                        else if (level == 4)
                        {
                            dot.setPosition(322, 2867);
                            level4Star1.setPosition (322, 2979);
                            level4Star2.setPosition (247, 2923);
                            level4Star3.setPosition (397, 2923);
                        }
                        else if (level == 5)
                        {
                           dot.setPosition(97, 3595);
                           level5Star1.setPosition (322, 3539);
                           level5Star2.setPosition (547, 3707);
                        }
					}
					else if (movement == 6)
                    {
                        quit = true;
                    }

                    //Level 1
                    DotOnStar(&dot, &level1Star1, tileSet, movement);
					DotOnStar(&dot, &level1Star2, tileSet, movement);
					starOnStar(&dot, &level1Star1, &level1Star2, tileSet, movement);

					//Level 2
					DotOnStar(&dot, &level2Star1, tileSet, movement);
					DotOnStar(&dot, &level2Star2, tileSet, movement);
					starOnStar(&dot, &level2Star1, &level2Star2, tileSet, movement);

					//Level 3
					DotOnStar(&dot, &level3Star1, tileSet, movement);

					// Level 4
					DotOnStar(&dot, &level4Star1, tileSet, movement);
					DotOnStar(&dot, &level4Star2, tileSet, movement);
					DotOnStar(&dot, &level4Star3, tileSet, movement);
					starOnStar(&dot, &level4Star1, &level4Star2, tileSet, movement);
					starOnStar(&dot, &level4Star1, &level4Star3, tileSet, movement);
					starOnStar(&dot, &level4Star2, &level4Star3, tileSet, movement);

					// level 5
					DotOnStar(&dot, &level5Star1, tileSet, movement);
					DotOnStar(&dot, &level5Star2, tileSet, movement);
					starOnStar(&dot, &level5Star1, &level5Star2, tileSet, movement);

				}

				//Move the dot
				dot.setCamera( camera );

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				//Render level
				for( int i = 0; i < TOTAL_TILES; ++i )
				{
					tileSet[ i ]->render( camera );
				}

				//Level 1
				level1Goal1.setOff();
				level1Goal2.setOff();
                level1Goal1.setActive(level1Star1.getX(), level1Star1.getY());
                level1Goal1.setActive(level1Star2.getX(), level1Star2.getY());
                level1Goal2.setActive(level1Star1.getX(), level1Star1.getY());
                level1Goal2.setActive(level1Star2.getX(), level1Star2.getY());

                //Level 2
                level2Goal1.setOff();
				level2Goal2.setOff();
                level2Goal1.setActive(level2Star1.getX(), level2Star1.getY());
                level2Goal1.setActive(level2Star2.getX(), level2Star2.getY());
                level2Goal2.setActive(level2Star1.getX(), level2Star1.getY());
                level2Goal2.setActive(level2Star2.getX(), level2Star2.getY());

                //Level 3
                level3Goal1.setOff();
                level3Goal1.setActive(level3Star1.getX(), level3Star1.getY());

                //Level 4
                level4Goal1.setOff();
				level4Goal2.setOff();
				level4Goal3.setOff();
                level4Goal1.setActive(level4Star1.getX(), level4Star1.getY());
                level4Goal1.setActive(level4Star2.getX(), level4Star2.getY());
                level4Goal1.setActive(level4Star3.getX(), level4Star3.getY());
                level4Goal2.setActive(level4Star1.getX(), level4Star1.getY());
                level4Goal2.setActive(level4Star2.getX(), level4Star2.getY());
                level4Goal2.setActive(level4Star3.getX(), level4Star3.getY());
                level4Goal3.setActive(level4Star1.getX(), level4Star1.getY());
                level4Goal3.setActive(level4Star2.getX(), level4Star2.getY());
                level4Goal3.setActive(level4Star3.getX(), level4Star3.getY());

                //Level 5
                level5Goal1.setOff();
				level5Goal2.setOff();
                level5Goal1.setActive(level5Star1.getX(), level5Star1.getY());
                level5Goal1.setActive(level5Star2.getX(), level5Star2.getY());
                level5Goal2.setActive(level5Star1.getX(), level5Star1.getY());
                level5Goal2.setActive(level5Star2.getX(), level5Star2.getY());



				//Render dot
				level1Goal1.render( camera );
				level1Goal2.render( camera );
				level2Goal1.render( camera );
				level2Goal2.render( camera );
				level3Goal1.render( camera );
				level4Goal1.render( camera );
				level4Goal2.render( camera );
				level4Goal3.render( camera );
				level5Goal1.render( camera );
				level5Goal2.render( camera );

				dot.render( camera );
				level1Star1.render( camera );
				level1Star2.render( camera );
				level2Star1.render( camera );
				level2Star2.render( camera );
				level3Star1.render( camera );
				level4Star1.render( camera );
				level4Star2.render( camera );
				level4Star3.render( camera );
				level5Star1.render( camera );
				level5Star2.render( camera );


				//Update screen
				SDL_RenderPresent( gRenderer );

				if (level == 1 && level1Goal1.getActive() == true && level1Goal2.getActive() == true)
                {
                    solve();
                    level++;
                    dot.setPosition(172, 1019);
                }
                else if (level == 2 && level2Goal1.getActive() == true && level2Goal2.getActive() == true)
                {
                    solve();
                    level ++;
                    dot.setPosition(172, 2083);
                }
                else if (level == 3 && level3Goal1.getActive() == true)
                {
                    solve();
                    level++;
                    dot.setPosition(322, 2867);
                }
                else if (level == 4 && level4Goal1.getActive() == true && level4Goal2.getActive() == true && level4Goal3.getActive() == true)
                {
                    solve();
                    level++;
                    dot.setPosition(97, 3595);
                }
                else if (level == 5 && level5Goal1.getActive() == true && level5Goal2.getActive() == true)
                {
                    solve();
                    level++;
                }


			}
		}

		//Free resources and close SDL
		close( tileSet );
	}

	return 0;
}

void DotOnStar (Dot *dot, Star *star, Tile *tileSet[], int movement )
{
    if (dot->getX() == star->getX() && dot->getY() == star->getY())
                    {
                        int check = star->move(tileSet, movement);
                        if (check == 1)
                        {
                            if (movement == 1)
                            {
                                dot->move(tileSet, 2);
                            }
                            else if (movement == 2)
                            {
                                dot->move(tileSet, 1);
                            }
                            else if (movement == 3)
                            {
                                dot->move(tileSet, 4);
                            }
                            else if (movement == 4)
                            {
                                dot->move(tileSet, 3);
                            }

                        }
                    }
}

void starOnStar(Dot *dot, Star *star, Star *star2, Tile *tileSet[], int movement )
{
    if (star2->getX() == star->getX() && star2->getY() == star->getY())
        {
            if (movement == 1)
            {
                dot->move(tileSet, 2);
                star2->move(tileSet, 2);
            }
            else if (movement == 2)
            {
                dot->move(tileSet, 1);
                star2->move(tileSet, 1);
            }
            else if (movement == 3)
            {
                dot->move(tileSet, 4);
                star2->move(tileSet, 4);
            }
            else if (movement == 4)
            {
                dot->move(tileSet, 3);
                star2->move(tileSet, 3);
            }

            }
}

void solve()
{
    SDL_Delay(500);
    if( !initMenu() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMediaMenu() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
			//Apply the image
			SDL_BlitSurface( gSolved, NULL, gScreenSurface, NULL );

			//Update the surface
			SDL_UpdateWindowSurface( gWindow );

			SDL_Delay(3000);

		}
	}
	close();
}

