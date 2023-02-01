#ifndef __BULLET__
#define __BULLET__

#include "entity.h"
struct stage;

struct bullet {
    static entity bullet_head, *bullet_tail;

    static void fire_bullet(entity* player);
    static void fire_alien_bullet(entity *from, entity *to);
    static void do_bullets();
};


extern int bullet_hit_fighter(entity *b);
extern int collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);

#endif
