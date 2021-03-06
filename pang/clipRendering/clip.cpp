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
        printf("Unable to load image %s! SDL Error: %s\n", 
				path.c_str(),SDL_GetError());
    } else {
        SDL_SetColorKey(loadedSurface, SDL_TRUE,
				SDL_MapRGB(loadedSurface->format,0,0xFF,0xFF));
        newTexture = SDL_CreateTextureFromSurface(gRenderer,loadedSurface);
        if (newTexture == NULL){
            printf("Unable to create new texture from surface: %s\n",
					SDL_GetError());
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

SDL_Rect gSpriteClips[4];
LTexture gSpriteSheetTexture;

bool init();
void close();
bool loadMedia();
int const SCREEN_WIDTH = 640;
int const SCREEN_HEIGHT = 480;

bool loadMedia(){
	bool success = true;
	if (!gSpriteSheetTexture.loadFromFile("dots.png")){
		printf("Failed to load sprite sheet texture!\n");
		success = false;
	} else {
		// Split the spritelocations by 100 pixels
		gSpriteClips[0].x = 0;
		gSpriteClips[0].y = 0;
		gSpriteClips[0].w = 100;
		gSpriteClips[0].h = 100;

		gSpriteClips[1].x = 100;
		gSpriteClips[1].y = 0;
		gSpriteClips[1].w = 100;
		gSpriteClips[1].h = 100;

		gSpriteClips[2].x = 0;
		gSpriteClips[2].y = 100;
		gSpriteClips[2].w = 100;
		gSpriteClips[2].h = 100;

		gSpriteClips[3].x = 100;
		gSpriteClips[3].y = 100;
		gSpriteClips[3].w = 100;
		gSpriteClips[3].h = 100;
	}
	return success;

}

bool init(){
    bool success = true;
    if (SDL_Init(SDL_INIT_VIDEO)< 0){
        printf("SDL could not initialize video! SDL Error: %s\n",
				SDL_GetError());
        success = false;
    } else {
        int imgFlags = IMG_INIT_PNG;
		int initted = IMG_Init(imgFlags);
        if (!(initted & imgFlags)) {
            printf("Unable to initialize image library. Error: %s\n",
					SDL_GetError());
            success = false;
        } else {
            gWindow = SDL_CreateWindow("clipTest",SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT,
                SDL_WINDOW_SHOWN);
            if (gWindow == NULL){
                printf("Unable to create window. Error: %s\n",
						SDL_GetError());
                success = false;
			} else {
				gRenderer = SDL_CreateRenderer(gWindow, -1,
					   SDL_RENDERER_ACCELERATED);
				if (gRenderer == NULL) {
					printf("Unable to create renderer: %s\n",
							SDL_GetError());
					success = false;
				}
			}
        }

    }
    return success;
}

void close(){
	SDL_DestroyWindow(gWindow);
	SDL_DestroyRenderer(gRenderer);
	gSpriteSheetTexture.free();

	SDL_QUIT;
	//IMG_QUIT;
}

int main(int args, char* argc[]){
    if(!init()){
        printf("Unable to initialize!\n");
	} else {
		bool quit = false;
		loadMedia();
		SDL_Event(e);
		while (!quit){
			while(SDL_PollEvent(&e) != 0){
				if(e.type==SDL_QUIT){
					quit = true;
				}
			}	

			SDL_SetRenderDrawColor(gRenderer,0xFF,0xFF,0xFF,0xFF);
			SDL_RenderClear(gRenderer);
			
			gSpriteSheetTexture.render(0,0, &gSpriteClips[0]);
			gSpriteSheetTexture.render(SCREEN_WIDTH - gSpriteClips[1].w,0,
					&gSpriteClips[1]);
			gSpriteSheetTexture.render(0, 
					SCREEN_HEIGHT - gSpriteClips[2].h, &gSpriteClips[2] );
			gSpriteSheetTexture.render(SCREEN_WIDTH - gSpriteClips[3].w,
					SCREEN_HEIGHT - gSpriteClips[3].h,
					&gSpriteClips[3]);


			SDL_RenderPresent(gRenderer);

		}
	}
	close();
    return 0;
}
