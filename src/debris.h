#ifndef __DEBRIS__
#define __DEBRIS__

#include <SDL2/SDL.h>
struct stage;
struct entity;


struct debris {
    float x;
    float y;
    float dx;
    float dy;
    SDL_Rect rect;
    SDL_Texture *texture;
    int life;
    debris *next;

    static debris debris_head, *debris_tail;

    static void add_debris(entity *e);
    static void do_debris();
    static void draw_debris(SDL_Renderer *r);
};

#endif
