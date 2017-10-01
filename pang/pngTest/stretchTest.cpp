#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "stdio.h"
#include "string"

//Key press surface constants
enum KeyPressSurfaces {
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};

//sStarts up SDL and creates a window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

void chooseMedia(SDL_Event keyDown);

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* gCurrentSurface = NULL;

//Loads individual image
SDL_Surface* loadSurface(std::string path);

//The Images that correspond to a keypress
SDL_Surface* gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];

//The image we will load and show on the screen
SDL_Surface* gXOut = NULL;

void chooseMedia(SDL_Event keyDown){
    switch (keyDown.key.keysym.sym) {
        case SDLK_UP:
        gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_UP];
        break;

        case SDLK_DOWN:
        gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
        break;
        
        case SDLK_LEFT:
        gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
        break;
        
        case SDLK_RIGHT:
        gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
        break;
        
        default:
        gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
        break;
        
    }
}

SDL_Surface* loadSurface(std::string path){
    SDL_Surface* optimizedSurface = NULL;
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)){
        printf ("SDL_image could not be initialized! SDL_image error: %s\n",SDL_GetError());
    } else {
        SDL_Surface* loadedSurface = IMG_Load(path.c_str());
        if (loadedSurface == NULL){
            printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        } else {
            //Convert surface to screen format
            optimizedSurface = SDL_ConvertSurface(loadedSurface,
                gScreenSurface->format,NULL);
            if (optimizedSurface == NULL) {
                printf("Unable to optimize image %s! SDL Error: %s\n", path.c_str(),
                    SDL_GetError());
            }
            SDL_FreeSurface(loadedSurface);
        }
    }
    return optimizedSurface;
}

bool init () {
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        printf ("SDL could not initialize! SDL_ERROR: %s\n", SDL_GetError());
        success = false;
    } else {
        //Create window
        gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN);
        if ( gWindow ==NULL ) {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            success = false;
        } else {
            //Get window surface
            gScreenSurface = SDL_GetWindowSurface(gWindow);
        }
    }

    return success;
}

bool loadMedia() {
    //Loading success flag
    bool success = true;
    
    //Load default surface
    gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = loadSurface("press.bmp.png");
    if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] == NULL) {
        printf("Failed to load default image!\n");
        success = false;
    }
    gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] = loadSurface("up.bmp.png");
    if (gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] == NULL) {
        printf("Failed to load default image!\n");
        success = false;
    }
    gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] = loadSurface("down.bmp.png");
    if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] == NULL) {
        printf("Failed to load default image!\n");
        success = false;
    }
    gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] = loadSurface("left.bmp.png");
    if (gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] == NULL) {
        printf("Failed to load default image!\n");
        success = false;
    }
    gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] = loadSurface("right.bmp.png");
    if (gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] == NULL) {
        printf("Failed to load default image!\n");
        success = false;
    }

    return success;
}

void close() {
    //Deallocate surface
    SDL_FreeSurface(gXOut);
    gXOut = NULL;

    //Destroy window
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}

int main( int argc, char* args[] ) {

    //Start up SDL and create window
    if (!init()) {
        printf("Failed to initialize!\n");
    } else {
        //Load media
        if (! loadMedia()) {
            printf("Failed to load media!\n");
        } else {
            //Main loop flag
            bool quit = false;

            //Event handler
            SDL_Event e;

            gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];

            while (! quit) {
                while (SDL_PollEvent(&e) !=0 ) {
                    if (e.type == SDL_QUIT) {
                        quit=true;
                    } else if (e.type == SDL_KEYDOWN){
                        chooseMedia(e);
                    }
                }

                //Apply the image
                SDL_Rect stretchRect;
                stretchRect.x = 0;
                stretchRect.y = 0;
                stretchRect.w = SCREEN_WIDTH;
                stretchRect.h = SCREEN_HEIGHT;
                SDL_BlitScaled(gCurrentSurface, NULL, gScreenSurface, &stretchRect);
                //Update the surface
                SDL_UpdateWindowSurface(gWindow);
            }
        }
    }
    close();

    return 0;
}