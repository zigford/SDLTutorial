#include "SDL2/SDL.h"
#include "string"

clas LTexture{
    public:
        LTexture();
}
bool init();


bool init(){
    bool success = true;
    return success;
}

int main(int argc,char* args[]){
    if (!init()) {
        printf("Failed to init SDL\n");
    }
    return 0;
}