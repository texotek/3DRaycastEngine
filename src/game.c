#include <stdbool.h>
#include <math.h>

#include <SDL2/SDL.h>
#include "SDL2/SDL_events.h"

#include <sys/time.h>

#include "types.h"
#include "sdl.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define FOV M_PI/4
#define DEPTH 12.0

#define SPEED 0.10

int map[] = {
    1,1,1,1,1,1,1,1,1,1,1,1,
    1,0,0,0,0,0,0,0,0,0,0,1,
    1,0,1,1,0,0,0,0,0,0,0,1,
    1,0,1,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,1,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,1,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,1,
    1,0,1,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,1,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,1,1,1,1,1
};

typedef struct {
    double x,y;
    double angle;
} Player;

int main(int argc, char *argv[])
{
    struct timeval t1;
    struct timeval t2;
    gettimeofday(&t1, 0);
    gettimeofday(&t2, 0);

    bool quit = false;
    Screen screen = createScreen(SCREEN_WIDTH, SCREEN_HEIGHT);
    Player p = {9,8,0};

    while(!quit) {
        gettimeofday(&t2, 0);
        double deltaTime = (double)(t2.tv_usec - t1.tv_usec) / 1000000 + (double)(t2.tv_sec - t1.tv_sec);;
        t1 = t2;

        printf("deltaTime = %lf\n", deltaTime);


        SDL_Event event;
        SDL_PollEvent(&event); 

        switch (event.type) {
            case SDL_KEYDOWN:
                printf("Key pressed: %c\n", event.key.keysym.sym);
                char pressed = event.key.keysym.sym;
                if(pressed == 'w') {
                    double dirX = sinf(p.angle);
                    double dirY = cosf(p.angle);
                    p.x = p.x + dirX * SPEED * deltaTime;
                    p.y = p.y + dirY * SPEED * deltaTime;
                }
                else if(pressed == 's') {
                    double dirX = sinf(p.angle);
                    double dirY = cosf(p.angle);
                    p.x = p.x - dirX * SPEED * deltaTime;
                    p.y = p.y - dirY * SPEED * deltaTime;
                }
                else if(pressed == 'a') {
                    p.angle -= 1 * deltaTime;
                }
                else if(pressed == 'd') {
                    p.angle += 1 * deltaTime;
                }
                break;
            case SDL_QUIT:
                quit = true;
                break;
        }

        for (int x = 0; x < screen.width; x++) {
            double rayAngle = (p.angle - FOV / 2.0) + ((double)x / (double)screen.width) * FOV;
            double dirX = sin(rayAngle);
            double dirY = cos(rayAngle);

            double distanceToWall = 0;
            while (1) {
                distanceToWall+= 0.01;
                int x = (int) p.x+dirX*distanceToWall;
                int y = (int) p.y+dirY*distanceToWall;
                if(x < 0 || x >= 12 || y < 0 || y >= 12) {
                    break;
                    distanceToWall = DEPTH;
                }
                else if(map[y * 12 + x] == 1) break;
            }

            int ceiling = screen.height/2.0 - screen.height/distanceToWall;
            int floor = screen.height- ceiling;

            for (int y = 0; y < screen.height; y++) {
                if(y<ceiling) screen.pixels[y*screen.width+x] = 0x00;
                else if(y>=ceiling && y<=floor) screen.pixels[y*screen.width+x] = 0xFFFFFF;// << 2 | (int)(0x44);
                else screen.pixels[y*screen.width+x] = 0x00;
            }
        }
        renderPixels(screen);
        SDL_Delay(8);
    }    
    destroyScreen(screen);
    return 0;
}
