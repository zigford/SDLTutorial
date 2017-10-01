#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "string"

SDL_Renderer *gRenderer = NULL;
SDL_Texture *gTexture = NULL;
SDL_Window *gWindow = NULL;

SDL_Texture* loadTexture(std::string path);

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init();
bool close();

SDL_Texture* loadTexture(std::string path){
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL){
        printf("Unable to load image %s. SDL Error: %s\n", path.c_str(),SDL_GetError());
    } else {
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL){
            printf("Unable to create texture from surface: %s\n",SDL_GetError());
        }
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}

bool init(){
    bool success = true;
    if (SDL_Init(SDL_INIT_VIDEO) <0){
        printf("Failed to INIT video. SDL Error: %s\n",SDL_GetError());
        success = false;
    } else {
        int imgFlags = IMG_INIT_PNG;
        if (!(IMG_Init(imgFlags) & imgFlags)) {
            printf("Failed to initialize SDL2 Image. SDL Error: %s\n", SDL_GetError());
            success = false;
        } else {
            gWindow = SDL_CreateWindow("viewPort", 
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                SCREEN_WIDTH, SCREEN_HEIGHT,
                SDL_WINDOW_SHOWN );
            if (gWindow == NULL){
                printf("Unable to create window. SDL Error: %s\n",SDL_GetError());
                success = false;
            } else {
                gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
                if (gRenderer == NULL){
                    printf("Renderer could not be created. SDL Error: %s\n",SDL_GetError());
                    success = false;
                }
            }
        }
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

        gTexture = loadTexture("kid.png");
        SDL_SetRenderDrawColor(gRenderer,0xFF,0xFF,0xFF,0xFF);
        SDL_RenderClear(gRenderer);
        
        SDL_Rect topLeftViewport;
        topLeftViewport.x = 0;
        topLeftViewport.y = 0;
        topLeftViewport.w = SCREEN_WIDTH / 2;
        topLeftViewport.h = SCREEN_HEIGHT;
        SDL_RenderSetViewport(gRenderer, &topLeftViewport);

        SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
        gTexture = loadTexture("girl.png");

        SDL_Rect topRightViewport;
        topRightViewport.x = SCREEN_WIDTH / 2;
        topRightViewport.y = 0;
        topRightViewport.w = SCREEN_WIDTH / 2;
        topRightViewport.h = SCREEN_HEIGHT;
        SDL_RenderSetViewport(gRenderer, &topRightViewport);

        SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
        SDL_RenderPresent(gRenderer);
        SDL_Delay(20000);

    }
    close();

    return 0;
}