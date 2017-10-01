#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "string"

class LTexture{
    public:
        //Initializes textures
        LTexture();

        //Deallocates memory
        ~LTexture();

        //Loads image at specified path
        bool loadFromFile(std::string path);

        //Deallocates texture
        void free();

        //Renders texture at given point
        void render(int x, int y);

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

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
LTexture gKidTexture;
LTexture gBackgroundTexture;
const int SCREEN_HEIGHT = 240;
const int SCREEN_WIDTH = 320;

LTexture::LTexture(){
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

LTexture::~LTexture(){
    free();
}

bool LTexture::loadFromFile(std::string path){
    free();
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL){
        printf("Unable to load image %s! SDL Error: %s\n", path.c_str(),SDL_GetError());
    } else {
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format,0,0xFF,0xFF));
        newTexture = SDL_CreateTextureFromSurface(gRenderer,loadedSurface);
        if (newTexture == NULL){
            printf("Unable to create new texture from surface: %s\n",SDL_GetError());
        } else {
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }
        SDL_FreeSurface(loadedSurface);
    }
    mTexture = newTexture;
    return mTexture != NULL;
}

void LTexture::free(){
    if(mTexture != NULL){
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
        mWidth = NULL;
        mHeight = NULL;
    }
}

void LTexture::render(int x,int y) {
    SDL_Rect renderQuad = {x,y,mWidth, mHeight};
    SDL_RenderCopy(gRenderer, mTexture, NULL, &renderQuad);
}

int LTexture::getWidth(){
    return mWidth;
}

int LTexture::getHeight(){
    return mHeight;
}

bool init();
bool loadMedia();
void close();

bool loadMedia(){
    bool success = true;
    if (!gKidTexture.loadFromFile("kid-transparent.png")){
        printf("Unable to load kid texture image!\n");
        success = false;
    }

    if(!gBackgroundTexture.loadFromFile("background.png")) {
        success = false;
    }

    return success;
}

bool init(){
    bool success = true;
    if (SDL_Init(SDL_INIT_VIDEO)<0){
        printf("Failed to initialize video. SDL Error: %s\n",SDL_GetError());
        success = false;
    } else {
        int imgFlags = IMG_INIT_PNG;
        if(!(IMG_Init(imgFlags)& imgFlags)) {
            printf("Failed to initialize SDL2_image. Error: %s\n",SDL_GetError());
            success = false;
        } else {
            gWindow = SDL_CreateWindow("colorKey",
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                SCREEN_WIDTH,SCREEN_HEIGHT,
                SDL_WINDOW_SHOWN);
            if (gWindow == NULL) {
                printf("Error creating window. SDL Error: %s\n",SDL_GetError());
                success = false;
            } else {
                gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
                if (gRenderer == NULL){
                    printf("Unable to create renderer. Error: %s\n",SDL_GetError());
                    success = false;
                }
            }
        }
    }
    return success;
}

void close() {
    gKidTexture.free();
    gBackgroundTexture.free();
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    IMG_Quit();
    SDL_Quit();
}
int main(int argc,char* args[]){
    if (!init()) {
        printf("Failed to init SDL\n");
    } else {
        if (!loadMedia()){
            printf("Failed to load media!\n");
        } else {
            bool quit = false;

            SDL_Event e;

            while(!quit){
                while(SDL_PollEvent(&e) != 0) {
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    }
                }
                SDL_SetRenderDrawColor(gRenderer,0xFF,0xFF,0xFF,0xFF);
                SDL_RenderClear(gRenderer);

                gBackgroundTexture.render(0,0);
                gKidTexture.render(100,100);
                SDL_RenderPresent(gRenderer);
            }

        }
    }
    close();
    return 0;
}