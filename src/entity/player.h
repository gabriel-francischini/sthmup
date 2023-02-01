#ifndef __PLAYER__
#define __PLAYER__

#include "entity.h"

struct player {
    static entity *player_ptr;

    static void init_player();
    static void do_player(int *keyboard);
    static void clip_player();
};


#endif
