#include "sdlfuncs.h"
#include "stdio.h"

// Set some globals

int main( int argc, char *args[] ){
    char *gameName = "Dumdie Dums first adventure";
    if ( !initSDL ) {
        logMsg( "SDL not working\n", 1 );
        return 1;
    } else {
        logMsg( "SDL working", 0 );
    }
    if ( !createWindow( gameName ) ) {
        logMsg ( "Couldn't create window", 1);
        return 1;
    } else {
        logMsg ( "Succesfully created window", 0 );
    }
    return 0;
}
