#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "stage.h"
#include "entity.h"
#include "star.h"
#include "explosion.h"
#include "debris.h"
#include "drawer.h"
#include "GLOBALS.h"
#include "bullet.h"

entity stage::fighter_head;
entity *stage::fighter_tail = &stage::fighter_head;

int stage::score;

static void clip_player();
static void do_background();


entity *stage::player;

static int spawn_timer;
static int reset_timer;

//DEFINING GLOBAL from GLOBALS.H
int background_x;

//static void draw();

stage *stage::init_stage(SDL_Renderer *r)
{
	stage *s = (stage*) calloc(1, sizeof(stage));
	s->fighter_tail = &s->fighter_head;
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

	while(s->fighter_head.next) {
		e = s->fighter_head.next;
		s->fighter_head.next = e->next;
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
	s->fighter_tail = &s->fighter_head;
	bullet::bullet_tail = &bullet::bullet_head;
	explosion::explosion_tail = &explosion::explosion_head;
	debris::debris_tail = &debris::debris_head;

	s->init_player();
	star::init_starfield();

	spawn_timer = 0;
	reset_timer = FPS * 3;
	s->score = 0;
}

void stage::init_player()
{
	stage *s = this;
	player = entity::create_entity(100, 100, SIDE_PLAYER, player_texture);
	player->dx = 0;
	player->dy = 0;
	player->health = 3;
	s->fighter_tail->next = player;
	s->fighter_tail = player;
}

void stage::do_logic(int *keyboard)
{
	stage *s = this;
	do_background();
	star::do_starfield();
	s->do_player(keyboard);
	s->do_enemies();
	s->do_fighters();
	bullet::do_bullets();
	s->spawn_enemies();
	clip_player();
	explosion::do_explosions();
	debris::do_debris();

	if(player == NULL && --reset_timer <= 0)
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

void stage::do_enemies()
{
	stage *s = this;
	entity *e;
	for(e = s->fighter_head.next; e != NULL; e = e->next) {
		if(e != player && player != NULL && --e->reload <= 0)
			bullet::fire_alien_bullet(e, player);
	}
}

void stage::do_fighters()
{
	stage *s = this;
	entity *e;
	entity *prev = &s->fighter_head;

	for (e = s->fighter_head.next; e != NULL ; e = e->next) {
		
		e->x += e->dx;
		e->y += e->dy;
		
		if(e != player && e->x < -e->w)
			e->health = 0;
		
		if(e->health == 0) {

			if (e == player)
				player = NULL;
			
			if(e == s->fighter_tail)
				s->fighter_tail = prev;
			
			prev->next = e->next;
			free(e);
			e = prev;
		}
		prev = e;
	}
}

void stage::spawn_enemies()
{
	stage *s = this;
	entity *enemy;
	
	if (--spawn_timer <= 0) {
		enemy = entity::create_entity(SCREEN_WIDTH, rand() % SCREEN_HEIGHT, SIDE_ALIEN, enemy_texture);
		enemy->dx = -(2 + (rand() % 4));
		enemy->reload = FPS * (1 + (rand() % 3));
		spawn_timer = 30 + (rand() % FPS);
		
		s->fighter_tail->next = enemy;
		s->fighter_tail = enemy;
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
	
	for (e = s->fighter_head.next; e != NULL ; e = e->next)
		drawer::blit(e->texture, e->x, e->y, r);
	
	debris::draw_debris(r);
	explosion::draw_explosions(r);
	s->draw_hud(player, r);
}
