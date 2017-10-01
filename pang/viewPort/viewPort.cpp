#include "SDL2/SDL.h"
#include "string"

SDL_Renderer *gRenderer = NULL;
SDL_Texture *gTexture = NULL;
SDL_Window *gWindow = NULL;

bool loadTexture(std::string path);

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init();
bool close();

loadTexture(std::string path){
    bool success = true;
    SDL_Surface *loadedSurface = NULL;

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("Failed to initialize SDL2 Image. SDL Error: %s\n", SDL_GetError());
        success = false;
    } else {
        
    }
}

bool init(){
    bool success = true;
    if (SDL_Init(SDL_INIT_VIDEO) <0){
        printf("Failed to INIT video. SDL Error: %s\n",SDL_GetError());
        success = false;
    }
    return success;
}

bool close(){
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyTexture(gTexture);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;
    gTexture = NULL;
    SDL_Quit();
}

int main (int argc,char * args[]){
    if (!init()) {
        printf("Failed to run init\n");
    } else {
        printf("Succesfull init\n");
        gWindow = SDL_CreateWindow("viewPort", 
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH, SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN );

        loadTexture("right.bmp")
        
        SDL_Rect topLeftViewport;
        topLeftViewport.x = 0;
        topLeftViewport.y = 0;
        topLeftViewport.w = SCREEN_WIDTH / 2;
        topLeftViewport.h = SCREEN_HEIGHT /2;
        SDL_RenderSetViewport(gRenderer, &topLeftViewport);

        SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

        SDL_Rect topRightViewport;
        topRightViewport.x = 0;
        topRightViewport.y = 0;
        topRightViewport.w = SCREEN_WIDTH / 2;
        topRightViewport.h = SCREEN_HEIGHT /2;
        SDL_RenderSetViewport(gRenderer, &topRightViewport);

        SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

        SDL_Rect bottomViewport;
        bottomViewport.x = 0;
        bottomViewport.y = 0;
        bottomViewport.w = SCREEN_WIDTH / 2;
        bottomViewport.h = SCREEN_HEIGHT /2;
        SDL_RenderSetViewport(gRenderer, &bottomViewport);

        SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

    }
    close();

    return 0;
}