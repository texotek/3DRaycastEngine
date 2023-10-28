#ifndef SDL_H
#define SDL_H

#include "types.h"
Screen createScreen(int screenWidth, int screenHeight);
void renderPixels(Screen screen);
void destroyScreen(Screen screen);
#endif // !SDL_H
