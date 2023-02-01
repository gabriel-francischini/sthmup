#ifndef __EXPLOSION__
#define __EXPLOSION__

#include <SDL2/SDL.h>


struct explosion {
    float x;
    float y;
    float dx;
    float dy;
    int r, g, b, a;
    explosion *next;


    static SDL_Texture *explosion_texture;
    static explosion explosion_head, *explosion_tail;

    static void add_explosions(int x, int y, int num);
    static void do_explosions();
    static void draw_explosions(SDL_Renderer *r);
};


#endif
