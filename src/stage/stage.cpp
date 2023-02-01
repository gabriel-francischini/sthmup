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

int stage::score;

static void clip_player();
static void do_background();


entity *stage::player;

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

	s->init_player();
	star::init_starfield();

	enemy::spawn_timer = 0;
	enemy::reset_timer = FPS * 3;
	s->score = 0;
}

void stage::init_player()
{
	// stage *s = this;
	player = entity::create_entity(100, 100, SIDE_PLAYER, player_texture);
	player->dx = 0;
	player->dy = 0;
	player->health = 3;
	enemy::fighter_tail->next = player;
	enemy::fighter_tail = player;
}

void stage::do_logic(int *keyboard)
{
	stage *s = this;
	do_background();
	star::do_starfield();
	s->do_player(keyboard);
	enemy::do_enemies();
	enemy::do_fighters();
	bullet::do_bullets();
	enemy::spawn_enemies();
	clip_player();
	explosion::do_explosions();
	debris::do_debris();

	if(player == NULL && --enemy::reset_timer <= 0)
		s->reset_stage();
}


static void do_background()
{
	if(--background_x < -SCREEN_WIDTH)
		background_x = 0;
}

void stage::do_player(int *keyboard)
{
	// stage *s = this;
	if(player != NULL) {
	
		player->dx = 0;
		player->dy = 0;
		
		if (player->reload > 0)
			player->reload--;
		
		if (keyboard[SDL_SCANCODE_UP])
			player->dy = -PLAYER_SPEED;
		
		if (keyboard[SDL_SCANCODE_DOWN])
			player->dy = PLAYER_SPEED;
		
		if (keyboard[SDL_SCANCODE_LEFT])
			player->dx = -PLAYER_SPEED;
		
		if (keyboard[SDL_SCANCODE_RIGHT])
			player->dx = PLAYER_SPEED;
		
		if (keyboard[SDL_SCANCODE_RCTRL] && player->reload <= 0)
			bullet::fire_bullet(player);
		
		player->x += player->dx;
		player->y += player->dy;
	}
}

static void clip_player()
{
	entity *p = stage::player;
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

void stage::draw(SDL_Renderer *r)
{
	stage *s = this;
	drawer::draw_background(r);
	star::draw_starfield(r);

	if(player != NULL)
		drawer::blit(player->texture, player->x, player->y, r);
	
	entity *b;
	
	for (b = bullet::bullet_head.next; b != NULL; b = b->next)
		drawer::blit(b->texture, b->x, b->y, r);

	entity *e;
	
	for (e = enemy::fighter_head.next; e != NULL ; e = e->next)
		drawer::blit(e->texture, e->x, e->y, r);
	
	debris::draw_debris(r);
	explosion::draw_explosions(r);
	s->draw_hud(player, r);
}
