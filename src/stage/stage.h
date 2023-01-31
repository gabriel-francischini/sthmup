#ifndef __STAGE__
#define __STAGE__

#include <SDL2/SDL.h>
#include "entity.h"
#include "explosion.h"
#include "debris.h"
#include "star.h"

#define MAX_STARS 500

struct stage {
    entity fighter_head, *fighter_tail;
    entity bullet_head, *bullet_tail;

    explosion explosion_head, *explosion_tail;
    debris debris_head, *debris_tail;
    star stars[MAX_STARS];
    int score;

    static stage *init_stage(SDL_Renderer *renderer);
    void do_logic(int *keyboard);
    void draw(SDL_Renderer *r);
};

#endif
