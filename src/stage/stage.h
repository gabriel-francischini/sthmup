#ifndef __STAGE__
#define __STAGE__

#include <SDL2/SDL.h>
#include "entity.h"
#include "explosion.h"
#include "debris.h"
#include "star.h"

#define MAX_STARS 500

#define FPS 60
#define SIDE_PLAYER 0

struct stage {
    entity fighter_head, *fighter_tail;
    entity bullet_head, *bullet_tail;

    explosion explosion_head, *explosion_tail;
    star stars[MAX_STARS];
    int score;

    static stage *init_stage(SDL_Renderer *renderer);
    void do_logic(int *keyboard);
    void extracted(SDL_Renderer *&r);
    void draw(SDL_Renderer *r);

    void add_explosions(int x, int y, int num);
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
    void init_starfield();
    void do_starfield();
    void do_explosions();

    // These were previously drawer.c methods
    void draw_explosions(SDL_Renderer *r);
    void draw_starfield(SDL_Renderer *r);
    void draw_hud(entity *player, SDL_Renderer *renderer);
};


#endif
