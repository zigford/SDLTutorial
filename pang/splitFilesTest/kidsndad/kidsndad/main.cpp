#include <init.h>
#include <iostream>
#include <SDL2/SDL.h>

bool gDebug = true;
const int SCREEN_HEIGHT = 768;
const int SCREEN_WIDTH = 1024;
SDL_Window* gWindow;
SDL_Renderer* gRenderer;

int main(int argc, char* args[]) {

	if (!init(gDebug)) {
		errorMessage("Failed to init", gDebug);
	} else {
		gWindow = SDL_CreateWindow("Game On!",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH,
			SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL) {
			errorMessage(stringconcat("Failed to Create Window SDL. Error: ", SDL_GetError()), gDebug);
		} else {
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL) {
				errorMessage(stringconcat("Failed to Create Renderer SDL. Error: ", SDL_GetError()), gDebug);
			}
		}
	}
	// errorMessage("Debug mode test!",gDebug);

	return 0;
}