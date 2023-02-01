#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "stage.h"
#include "entity.h"
#include "entity/star.h"
#include "entity/explosion.h"
#include "entity/debris.h"
#include "drawer.h"
#include "GLOBALS.h"
#include "entity/bullet.h"
#include "entity/enemy.h"
#include "entity/player.h"

int stage::score;

static void do_background();


entity *player::player_ptr;

//DEFINING GLOBAL from GLOBALS.H
int background_x;

//static void draw();

stage *stage::init_stage(SDL_Renderer *r)
{
	stage *s = (stage*) calloc(1, sizeof(stage));
	enemy::fighter_tail = &enemy::fighter_head;
	bullet::bullet_tail = &bullet::bullet_head;
	s->score = 0;

	drawer::init_draw(r);
	
	s->reset_stage();

	return s;
}

void stage::reset_stage()
{
	stage *s = this;
	entity *e;
	explosion *ex;
	debris *d;

	while(enemy::fighter_head.next) {
		e = enemy::fighter_head.next;
		enemy::fighter_head.next = e->next;
		free(e);
	}

	while (bullet::bullet_head.next) {
		e = bullet::bullet_head.next;
		bullet::bullet_head.next = e->next;
		free(e);
	}

	while (explosion::explosion_head.next) {
		ex = explosion::explosion_head.next;
		explosion::explosion_head.next = ex->next;
		free(ex);
	}

	while(debris::debris_head.next) {
		d = debris::debris_head.next;
		debris::debris_head.next = d->next;
		free(d);
	}

	memset(s, 0, sizeof(stage));
	enemy::fighter_tail = &enemy::fighter_head;
	bullet::bullet_tail = &bullet::bullet_head;
	explosion::explosion_tail = &explosion::explosion_head;
	debris::debris_tail = &debris::debris_head;

	player::init_player();
	star::init_starfield();

	enemy::spawn_timer = 0;
	enemy::reset_timer = FPS * 3;
	s->score = 0;
}

void stage::do_logic(int *keyboard)
{
	stage *s = this;
	do_background();
	star::do_starfield();
	player::do_player(keyboard);
	enemy::do_enemies();
	enemy::do_fighters();
	bullet::do_bullets();
	enemy::spawn_enemies();
	player::clip_player();
	explosion::do_explosions();
	debris::do_debris();

	if(player::player_ptr == NULL && --enemy::reset_timer <= 0)
		s->reset_stage();
}


static void do_background()
{
	if(--background_x < -SCREEN_WIDTH)
		background_x = 0;
}

void stage::draw(SDL_Renderer *r)
{
	stage *s = this;
	drawer::draw_background(r);
	star::draw_starfield(r);

	if(player::player_ptr != NULL)
		drawer::blit(player::player_ptr->texture, player::player_ptr->x, player::player_ptr->y, r);
	
	entity *b;
	
	for (b = bullet::bullet_head.next; b != NULL; b = b->next)
		drawer::blit(b->texture, b->x, b->y, r);

	entity *e;
	
	for (e = enemy::fighter_head.next; e != NULL ; e = e->next)
		drawer::blit(e->texture, e->x, e->y, r);
	
	debris::draw_debris(r);
	explosion::draw_explosions(r);
	s->draw_hud(player::player_ptr, r);
}
