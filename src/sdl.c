#include "sdl.h"

#include <SDL2/SDL.h>

#define assert(_e, ...) if(!(_e)) { fprintf(stderr, __VA_ARGS__); exit(1); }

Screen createScreen(int SCREEN_WIDTH, int SCREEN_HEIGHT) {
    Screen app;
    memset(&app, 0, sizeof(app));
    app.width = SCREEN_WIDTH;
    app.height = SCREEN_HEIGHT;

    assert(!SDL_Init(SDL_INIT_VIDEO),
           "Could not initialize window: %s",
           SDL_GetError());

    app.window = SDL_CreateWindow(
        "Window", 100, 100,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        0
    );
    assert(app.window, "Could not Create Window: %s\n", SDL_GetError());
    app.renderer = SDL_CreateRenderer(app.window, -1, 0);
    assert(app.renderer, "Could not Create Window: %s\n", SDL_GetError());
    app.texture = SDL_CreateTexture(
        app.renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        SCREEN_WIDTH,
        SCREEN_HEIGHT);
    assert(app.texture, "Could not Create Texture: %s\n", SDL_GetError());
    app.pixels = (u32*)calloc(SCREEN_WIDTH * SCREEN_HEIGHT, sizeof(u32));
    memset(app.pixels, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(u32));
    return app;
}

void renderPixels(Screen screen) {
        SDL_UpdateTexture(screen.texture,
                          NULL,
                          screen.pixels,
                          screen.width * sizeof(u32));

        SDL_RenderCopyEx(screen.renderer,
                         screen.texture,
                         NULL, NULL,
                         0.0, NULL,
                         SDL_FLIP_NONE);
        SDL_RenderPresent(screen.renderer);
}

void destroyScreen(Screen screen) {
    free(screen.pixels);
    SDL_DestroyTexture(screen.texture);
    SDL_DestroyRenderer(screen.renderer);
    SDL_DestroyWindow(screen.window);
    SDL_Quit();
}
