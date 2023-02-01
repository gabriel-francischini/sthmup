#ifndef __STAGE__
#define __STAGE__

#include <SDL2/SDL.h>
#include "entity.h"
#include "explosion.h"
#include "debris.h"
#include "star.h"

#define FPS 60
#define SIDE_PLAYER 0

struct stage {
    entity fighter_head, *fighter_tail;
    entity bullet_head, *bullet_tail;

    int score;

    static stage *init_stage(SDL_Renderer *renderer);
    void do_logic(int *keyboard);
    void extracted(SDL_Renderer *&r);
    void draw(SDL_Renderer *r);

private:
    void init_player();
    void fire_bullet();
    void fire_alien_bullet(entity *e);
    void do_player(int *keyboard);
    void do_bullets();
    void do_enemies();
    void do_fighters();
    void spawn_enemies();
    void reset_stage();

    // These were previously drawer.c methods
    void draw_hud(entity *player, SDL_Renderer *renderer);
};


#endif
