#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "string"

SDL_Renderer* gRenderer = NULL;
SDL_Window* gWindow = NULL;

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
        void render(int x, int y, SDL_Rect* clip = NULL);

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

void LTexture::render(int x,int y, SDL_Rect* clip) {
    SDL_Rect renderQuad = {x,y,mWidth, mHeight};

    if (clip != NULL){
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }
    SDL_RenderCopy(gRenderer, mTexture, clip, &renderQuad);
}

int LTexture::getWidth(){
    return mWidth;
}

int LTexture::getHeight(){
    return mHeight;
}

bool init();
bool close();
bool loadMedia();
int const SCREEN_WIDTH = 640;
int const SCREEN_HEIGHT = 480;

bool init(){
    bool success = true;
    if (SDL_Init(SDL_INIT_VIDEO)< 0){
        printf("SDL could not initialize video! SDL Error: %s\n", SDL_GetError());
        success = false;
    } else {
        int imgFlags = IMG_INIT_PNG;
        if (!(IMG_Init(imgFlags)& imgFlags)) {
            printf("Unable to initialize image library. Error: %s\n",SDL_GetError());
            success = false;
        } else {
            gWindow = SDL_CreateWindow("clipTest",SCREEN_WINDOWPOS_UNDEFINED,
                SCREEN_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT,
                SCREEN_WINDOW_SHOWN)
            if (gWindown == NULL){
                printf("Unable to create window. Error: %s\n",SDL_GetError());
                success = false;
            }
        }

    }
    return success;
}

int main(int args, char* argc[]){
    if(!init()){
        printf("Unable to initialize!\n");
    }
    return 0;
}