#ifndef SDLFUNCS
#define SDLFUCN

bool initSDL();
const int SCREEN_HEIGHT = 480;
const int SCREEN_WIDTH = 640;
bool createWindow( char *windowTitle );
void logMsg(char *logMessage,int level);

#endif
