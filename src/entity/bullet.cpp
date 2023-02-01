#include "bullet.h"
#include "stage/stage.h"
#include "GLOBALS.h"
#include "drawer.h"
#include "entity/enemy.h"
#include "entity/player.h"

entity bullet::bullet_head;
entity *bullet::bullet_tail = &bullet::bullet_head;

int collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
	return (MAX(x1, x2) < MIN(x1 + w1, x2 + w2)) && (MAX(y1, y2) < MIN(y1 + h1, y2 + h2));
}

void bullet::fire_bullet(entity* player)
{
	// stage *s = this;
	entity *bullet = entity::create_entity(player->x, player->y, SIDE_PLAYER, bullet_texture);

	bullet::bullet_tail->next = bullet;
	bullet::bullet_tail = bullet;

	bullet->dx = PLAYER_BULLET_SPEED;

	bullet->y += (player->h / 2) - (bullet->h / 2);

	player->reload = 8;
}


void bullet::fire_alien_bullet(entity *from, entity *to)
{
	// stage *s = this;
	entity *e = from;
	entity *player = to;

	entity *bullet = entity::create_entity(e->x, e->y, e->side, alien_bullet_texture);
	bullet::bullet_tail->next = bullet;
	bullet::bullet_tail = bullet;

	bullet->x += (e->w / 2) - (bullet->w / 2);
	bullet->y += (e->h / 2) - (bullet->h / 2);

	drawer::calc_slope(player->x + (player->w / 2), player->y + (player->h / 2), e->x, e->y, &bullet->dx, &bullet->dy);

	bullet->dx *= ALIEN_BULLET_SPEED;
	bullet->dy *= ALIEN_BULLET_SPEED;

	bullet->side = SIDE_ALIEN;

	e->reload = (rand() % FPS * 2);
}


void bullet::do_bullets()
{
	// stage *s = this;
	entity *b;
	entity *prev = &bullet::bullet_head;

	for (b = bullet::bullet_head.next; b != NULL; b = b->next) {
		b->x += b->dx;
		b->y += b->dy;

		if (bullet_hit_fighter(b) || b->x < -b->w || b->y < -b->h || b->x > SCREEN_WIDTH || b->y > SCREEN_HEIGHT) {
			if (b == bullet::bullet_tail)
				bullet::bullet_tail = prev;

			prev->next = b->next;
			free(b);
			b = prev;
		}
		prev = b;
	}
}


int bullet_hit_fighter(entity *b)
{
	entity *e;

	for(e = enemy::fighter_head.next; e != NULL; e = e->next) {
		if(e->side != b->side && collision(b->x, b->y, b->w, b->h, e->x, e->y, e->w, e->h)) {
			b->health = 0;
			e->health--;

			if(e->health == 0) {
				explosion::add_explosions(e->x, e->y, 32);
				debris::add_debris(e);
				if(e != player::player_ptr)
					stage::score++;
			}

			return 1;

		}
	}
	return 0;
}
