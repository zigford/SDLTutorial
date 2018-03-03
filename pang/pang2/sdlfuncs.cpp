#include "SDL2/SDL.h"

bool verbose = true;
SDL_Window *window = NULL;
SDL_Surface *screenSurface = NULL;
const int SCREEN_HEIGHT = 480;
const int SCREEN_WIDTH = 640;

bool initSDL(){
    int result = true;

    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
        printf( "SDL could not initialize! SDL_Error %s\n", SDL_GetError() );
        result = false;
    }
    return result;
}

void logMsg(char *logMessage,int level){
    switch (level){
        case 0 :
            if (verbose) {
                printf( "info: %s\n", logMessage);
            }
            break;
        case 1 : printf( "error: %s, SDL: %s\n", logMessage,
                         SDL_GetError() );
            break;
    }
}

bool createWindow( char *windowTitle ){
    bool result = true;
    logMsg( "Creating SDL Window", 0 );
    window = SDL_CreateWindow( windowTitle,
          SDL_WINDOWPOS_UNDEFINED,
          SDL_WINDOWPOS_UNDEFINED,
          SCREEN_WIDTH,
          SCREEN_HEIGHT,
          SDL_WINDOW_SHOWN);
    if ( window == NULL ){
        result = false;
    }
    return result;
}

