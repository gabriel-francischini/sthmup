#include "enemy.h"
#include "GLOBALS.h"
#include "bullet.h"
#include "stage/stage.h"
#include "entity/player.h"

entity enemy::fighter_head;
entity *enemy::fighter_tail = &enemy::fighter_head;

int enemy::spawn_timer;
int enemy::reset_timer;


void enemy::do_enemies()
{
	// stage *s = this;
	entity *e;
	for(e = enemy::fighter_head.next; e != NULL; e = e->next) {
		if(e != player::player_ptr && player::player_ptr != NULL && --e->reload <= 0)
			bullet::fire_alien_bullet(e, player::player_ptr);
	}
}

void enemy::do_fighters()
{
	// stage *s = this;
	entity *e;
	entity *prev = &enemy::fighter_head;

	for (e = enemy::fighter_head.next; e != NULL ; e = e->next) {

		e->x += e->dx;
		e->y += e->dy;

		if(e != player::player_ptr && e->x < -e->w)
			e->health = 0;

		if(e->health == 0) {

			if (e == player::player_ptr)
				player::player_ptr = NULL;

			if(e == enemy::fighter_tail)
				enemy::fighter_tail = prev;

			prev->next = e->next;
			free(e);
			e = prev;
		}
		prev = e;
	}
}

void enemy::spawn_enemies()
{
	// stage *s = this;
	entity *enemy;

	if (--spawn_timer <= 0) {
		enemy = entity::create_entity(SCREEN_WIDTH, rand() % SCREEN_HEIGHT, SIDE_ALIEN, enemy_texture);
		enemy->dx = -(2 + (rand() % 4));
		enemy->reload = FPS * (1 + (rand() % 3));
		spawn_timer = 30 + (rand() % FPS);

		enemy::fighter_tail->next = enemy;
		enemy::fighter_tail = enemy;
	}
}
