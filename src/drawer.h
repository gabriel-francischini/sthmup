#ifndef __DRAWER__
#define __DRAWER__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "GLOBALS.h"

//void draw(stage *s, SDL_Renderer *r, entity *player);

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

// not really a struct, it's actually just a namespace
struct drawer {
    static void init_draw(SDL_Renderer *r);
    static void draw_background(SDL_Renderer *r);
    static void draw_text(int x, int y, int r, int g, int b, SDL_Renderer *renderer, char *format, ...);

    static void blit(SDL_Texture *texture, int x, int y, SDL_Renderer *r);
    static void blit_rect(SDL_Texture *texture, SDL_Rect *src, int x, int y, SDL_Renderer *r);

    static SDL_Texture *load_texture(char *filename, SDL_Renderer *r);

    static void calc_slope(int x1, int y1, int x2, int y2, float *dx, float *dy);
};

#endif
