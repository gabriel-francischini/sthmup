#ifndef __DRAWER__
#define __DRAWER__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "GLOBALS.h"

//void draw(stage *s, SDL_Renderer *r, entity *player);

void init_draw(SDL_Renderer *r);

void draw_background(SDL_Renderer *r);

void draw_text(int x, int y, int r, int g, int b, SDL_Renderer *renderer, char *format, ...);

void blit(SDL_Texture *texture, int x, int y, SDL_Renderer *r);

void blit_rect(SDL_Texture *texture, SDL_Rect *src, int x, int y, SDL_Renderer *r);

SDL_Texture *load_texture(char *filename, SDL_Renderer *r);

#endif
