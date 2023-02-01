#ifndef __STAR__
#define __STAR__

#include <SDL2/SDL.h>

#define MAX_STARS 500


struct star {
    int x;
    int y;
    int speed;

    static star stars[MAX_STARS];

    static void init_starfield();
    static void do_starfield();
    static void draw_starfield(SDL_Renderer *r);
};

#endif