#include "string"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

int const SCREEN_HEIGHT = 480;
int const SCREEN_WIDTH = 640;
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


bool init(char* title = "default");
void close();
bool loadMedia();
LTexture gModulatedTexture;

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

bool loadMedia(){
    bool success = true;
	gModulatedTexture.loadFromFile("background.png");
    return success;
}

void close() {
    //Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
	gModulatedTexture.free();
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

void changeColors(SDL_Event keyDown,Uint8 *r, Uint8 *g, Uint8 *b);

void changeColors(SDL_Event keyDown,Uint8 *r, Uint8 *g, Uint8 *b){
    switch (keyDown.key.keysym.sym) {
        case SDLK_q:
        *r += 32;
		printf("r now equals: %d\n",r);
        break;

        case SDLK_w:
        *g += 32;
        break;

        case SDLK_e:
        *b += 32;
        break;

        case SDLK_a:
        *r -= 32;
        break;

        case SDLK_s:
        *g -= 32;
        break;

        case SDLK_d:
        *b -= 32;
        break;
    }
}

int main(int args, char* argc[]){
    if(!init()){
        printf("Unable to initialize!\n");
    } else {
    	bool quit = false;
        loadMedia();

		Uint8 r = 255;
		Uint8 g = 255;
		Uint8 b = 255;
			
        SDL_Event(e);
        while (!quit){
        	while(SDL_PollEvent(&e) != 0){
            	if(e.type==SDL_QUIT){
                	quit = true;
				}else if (e.type == SDL_KEYDOWN){
					changeColors(e,&r,&g,&b);
					printf("Current R: %d\n", r);
				}
            }       

    	    SDL_SetRenderDrawColor(gRenderer,0xFF,0xFF,0xFF,0xFF);
        	SDL_RenderClear(gRenderer);
			// Code here
			gModulatedTexture.setColor(r,g,b);
			gModulatedTexture.render(0,0);	
			// end code here
        	SDL_RenderPresent(gRenderer);

    	}
	}

    close();
    return 0;
}

