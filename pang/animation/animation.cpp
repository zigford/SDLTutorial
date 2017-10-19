#include "string"
#include "stdio.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

const int SCREEN_HEIGHT = 768;
const int SCREEN_WIDTH = 1024;
const int WALKING_ANIMATION_FRAMES = 8;

const int xCharWidth = 260;
SDL_Rect gSpriteClips[WALKING_ANIMATION_FRAMES];
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

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

		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode(SDL_BlendMode blending);

		//Set alpha modulation
		void setAlpha(Uint8 alpha);

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

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue){
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending){
	//Set blending function
	SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha){
	//Modulte texture alpha
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

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

		mWidth = NULL;
		mHeight = NULL;
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
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

bool init(char* title = "default");
void close();
bool loadMedia();
LTexture gSpriteSheetTexture;

bool init(char* title){
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
            gWindow = SDL_CreateWindow(title,SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT,
                SDL_WINDOW_SHOWN);
            if (gWindow == NULL){
                printf("Unable to create window. Error: %s\n",
                    SDL_GetError());
                success = false;
            } else {
                gRenderer = SDL_CreateRenderer(gWindow, -1,
                    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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

bool loadMedia(){
    bool success = true;
	if (!gSpriteSheetTexture.loadFromFile("spritesheet.png")) {
		printf("Failed to load spriteclip sheet\n");
		success = false;
	} else {
		// Set sprite clips

		int hCharHeight = gSpriteSheetTexture.getHeight();
		for (int x = 0; x <= WALKING_ANIMATION_FRAMES; x++) {
			gSpriteClips[x].x = x * xCharWidth;
			gSpriteClips[x].y = 0;
			gSpriteClips[x].w = xCharWidth;
			gSpriteClips[x].h = hCharHeight;
		}
	}
    return success;
}

void close() {
    //Destroy window
	gSpriteSheetTexture.free();	
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
	gWindow = NULL;
    gRenderer = NULL;
    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

int main(int args, char* argc[]){
	printf("Initializing\n");
    if(!init()){
        printf("Unable to initialize!\n");
    } else {
    	bool quit = false;
        loadMedia();

		int frame = 0;
        SDL_Event(e);
        while (!quit){
			while (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					quit = true;
				}
			}
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(gRenderer);
			SDL_Rect* currentClip = &gSpriteClips[frame /4];
			gSpriteSheetTexture.render((
				SCREEN_WIDTH - currentClip->w) / 2,
				(SCREEN_HEIGHT - currentClip->h) / 2
				, currentClip);
			//Update screen SDL_RenderPresent( gRenderer );
			SDL_RenderPresent(gRenderer);
			++frame;
			//printf("Current frame is: %d", frame);
			if (frame / 4 >= WALKING_ANIMATION_FRAMES) {
				frame = 0;
			}
    	}
	}

    close();
	printf("boo\n");
    return 0;
}

