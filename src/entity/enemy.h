#ifndef  __ENEMY__
#define  __ENEMY__

#include "entity.h"

struct enemy {
    static entity fighter_head, *fighter_tail;

    static int spawn_timer;
    static int reset_timer;

    static void do_enemies();
    static void do_fighters();
    static void spawn_enemies();
};

#endif
