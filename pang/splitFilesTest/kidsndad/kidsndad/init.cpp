#ifdef _WIN32
#include <windows.h>
#endif
#include <SDL2\SDL.h>
#include <SDL2\SDL_image.h>
#include <iostream>

using namespace std;

char* stringconcat(char* str1, const char* str2) {
	while (*str1 != '\0') {
		str1++;
	}
	while (*str2 != '\0') {
		*str1 = *str2;
		str1++;
		str2++;
	}
	return str1;
}

void errorMessage(char* message, bool debug) {
	if (debug) {
#ifdef _WIN32
		MessageBox(NULL, message,
			"Debug Mode on", MB_OK | MB_ICONEXCLAMATION);
#else
		cout << message << endl;
#endif
	}
}

bool init(bool debug) {
	bool success = true;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		errorMessage(stringconcat("Failed to initialize SDL. Error: ", SDL_GetError()), debug);
		success = false;
	}
	return success;
}