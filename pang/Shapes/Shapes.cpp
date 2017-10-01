#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

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

//Global SDL objects
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* gCurrentSurface = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Texture* gTexture = NULL;

//Media load functions
SDL_Surface* loadSurface(std::string path);
SDL_Texture* loadTexture(std::string path);

//The Images that correspond to a keypress
SDL_Texture* gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];

//The image we will load and show on the screen
SDL_Surface* gXOut = NULL;

void chooseMedia(SDL_Event keyDown){
    switch (keyDown.key.keysym.sym) {
        case SDLK_UP:
        gTexture = gKeyPressSurfaces[KEY_PRESS_SURFACE_UP];
        break;

        case SDLK_DOWN:
        gTexture = gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
        break;
        
        case SDLK_LEFT:
        gTexture = gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
        break;
        
        case SDLK_RIGHT:
        gTexture = gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
        break;
        
        default:
        gTexture = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
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

SDL_Texture* loadTexture(std::string path){
    //The final texture
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL){
        printf("Unable to load image %s! SDL_image Error: %s\n",
            path.c_str(),SDL_GetError());
    } else {
        newTexture = SDL_CreateTextureFromSurface (gRenderer, loadedSurface);
        if (newTexture == NULL) {
            printf("Unable to create texture from %s! SDL Error: %s\n", 
                path.c_str(),SDL_GetError());
        }
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
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
            gRenderer = SDL_CreateRenderer(gWindow, -1,
                SDL_RENDERER_ACCELERATED);
            if (gRenderer == NULL) {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            } else {
                SDL_SetRenderDrawColor(gRenderer,0xFF,0xFF,0xFF,0xFF);
                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)){
                    printf ("SDL_image could not be initialized! SDL_image error: %s\n",SDL_GetError());
                    success = false;
                } 
            }
        }
    }

    return success;
}

bool loadMedia() {
    //Loading success flag
    bool success = true;
    
    //Load default surface
    return success;
}

void close() {
    SDL_DestroyTexture(gTexture);
    gTexture = NULL;

    //Deallocate surface
    SDL_FreeSurface(gXOut);
    gXOut = NULL;

    //Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    IMG_Quit();
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

            //gTexture = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];

            while (! quit) {
                while (SDL_PollEvent(&e) !=0 ) {
                    if (e.type == SDL_QUIT) {
                        quit=true;
                    } else if (e.type == SDL_KEYDOWN){
                        chooseMedia(e);
                    }
                }

                //Apply the image
                SDL_SetRenderDrawColor(gRenderer, 0xFF,0xFF,0xFF,0xFF);
                SDL_RenderClear(gRenderer);
                SDL_Rect fillRect = {SCREEN_WIDTH/4, SCREEN_HEIGHT/4,
                    SCREEN_WIDTH/2, SCREEN_HEIGHT/2};
                SDL_SetRenderDrawColor(gRenderer, 0xFF,0x00,0x00,0xFF);
                SDL_RenderFillRect(gRenderer, &fillRect);

                SDL_Rect outlineRect = {
                    SCREEN_WIDTH / 6,
                    SCREEN_HEIGHT / 6,
                    SCREEN_WIDTH * 2/3,
                    SCREEN_HEIGHT * 2/3
                };
                SDL_SetRenderDrawColor(gRenderer, 0x00,0xFF,0x00,0xFFl);
                SDL_RenderDrawRect(gRenderer, &outlineRect);

                SDL_SetRenderDrawColor(gRenderer,0x00,0x00,0xFF,0xFF);
                SDL_RenderDrawLine(gRenderer,0,SCREEN_HEIGHT/2,SCREEN_WIDTH,
                    SCREEN_HEIGHT/2);

                    SDL_SetRenderDrawColor(gRenderer,0xFF,0xFF,0x00,0xFF);
                    for(int i =0; i < SCREEN_HEIGHT; i+=4){
                        SDL_RenderDrawPoint(gRenderer,SCREEN_WIDTH/2,i);
                    }
                SDL_RenderPresent(gRenderer);
            }
        }
    }
    close();

    return 0;
}