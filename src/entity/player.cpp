#include "player.h"
#include "GLOBALS.h"
#include "stage/stage.h"
#include "entity/enemy.h"
#include "entity/bullet.h"


void player::clip_player()
{
	entity *p = player::player_ptr;
	if(p != NULL) {
		if(p->x < 0)
			p->x = 0;

		if(p->y < 0)
			p->y = 0;

		if(p->x > SCREEN_WIDTH / 2)
			p->x = SCREEN_WIDTH / 2;

		if(p->y > SCREEN_HEIGHT - p->h)
			p->y = SCREEN_HEIGHT - p->h;
	}
}

void player::init_player()
{
	// stage *s = this;
	player_ptr = entity::create_entity(100, 100, SIDE_PLAYER, player_texture);
	player_ptr->dx = 0;
	player_ptr->dy = 0;
	player_ptr->health = 3;
	enemy::fighter_tail->next = player::player_ptr;
	enemy::fighter_tail = player::player_ptr;
}


void player::do_player(int *keyboard)
{
	// stage *s = this;
	if(player_ptr != NULL) {

		player_ptr->dx = 0;
		player_ptr->dy = 0;

		if (player_ptr->reload > 0)
			player_ptr->reload--;

		if (keyboard[SDL_SCANCODE_UP])
			player_ptr->dy = -PLAYER_SPEED;

		if (keyboard[SDL_SCANCODE_DOWN])
			player_ptr->dy = PLAYER_SPEED;

		if (keyboard[SDL_SCANCODE_LEFT])
			player_ptr->dx = -PLAYER_SPEED;

		if (keyboard[SDL_SCANCODE_RIGHT])
			player_ptr->dx = PLAYER_SPEED;

		if (keyboard[SDL_SCANCODE_RCTRL] && player_ptr->reload <= 0)
			bullet::fire_bullet(player::player_ptr);

		player_ptr->x += player_ptr->dx;
		player_ptr->y += player_ptr->dy;
	}
}
