#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "stage.h"
#include "entity.h"
#include "star.h"
#include "explosion.h"
#include "debris.h"
#include "drawer.h"
#include "GLOBALS.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define PLAYER_SPEED 4
#define PLAYER_BULLET_SPEED 16
#define ALIEN_BULLET_SPEED 8
#define SIDE_ALIEN 1
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

static void clip_player();
static int collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
static int bullet_hit_fighter(entity *b, stage *s);
static void calc_slope(int x1, int y1, int x2, int y2, float *dx, float *dy);
static void do_background();


static entity *player;

static int spawn_timer;
static int reset_timer;

//DEFINING GLOBAL from GLOBALS.H
int background_x;

//static void draw();

stage *stage::init_stage(SDL_Renderer *r)
{
	stage *s = (stage*) calloc(1, sizeof(stage));
	s->fighter_tail = &s->fighter_head;
	s->bullet_tail = &s->bullet_head;
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

	while (s->bullet_head.next) {
		e = s->bullet_head.next;
		s->bullet_head.next = e->next;
		free(e);
	}

	while (s->explosion_head.next) {
		ex = s->explosion_head.next;
		s->explosion_head.next = ex->next;
		free(ex);
	}

	while(debris::debris_head.next) {
		d = debris::debris_head.next;
		debris::debris_head.next = d->next;
		free(d);
	}

	memset(s, 0, sizeof(stage));
	s->fighter_tail = &s->fighter_head;
	s->bullet_tail = &s->bullet_head;
	s->explosion_tail = &s->explosion_head;
	debris::debris_tail = &debris::debris_head;

	s->init_player();
	s->init_starfield();

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

void stage::init_starfield()
{
	stage *s = this;
	int i;
	for(i = 0; i < MAX_STARS; i++) {
		s->stars[i].x = rand() % SCREEN_WIDTH;
		s->stars[i].y = rand() % SCREEN_HEIGHT;
		s->stars[i].speed = 1 + rand() % 8;
	}
}

void stage::do_logic(int *keyboard)
{
	stage *s = this;
	do_background();
	s->do_starfield();
	s->do_player(keyboard);
	s->do_enemies();
	s->do_fighters();
	s->do_bullets();
	s->spawn_enemies();
	clip_player();
	s->do_explosions();
	debris::do_debris();

	if(player == NULL && --reset_timer <= 0)
		s->reset_stage();
}


static void do_background()
{
	if(--background_x < -SCREEN_WIDTH)
		background_x = 0;
}

void stage::do_starfield()
{
	stage *s = this;
	int i;

	for(i = 0; i < MAX_STARS; i++) {
		s->stars[i].x -= s->stars[i].speed;

		if(s->stars[i].x < 0)
			s->stars[i].x += SCREEN_WIDTH;
	}
}

void stage::do_explosions()
{
	stage *s = this;
	explosion *e;
	explosion *prev = &s->explosion_head;

	for(e = s->explosion_head.next; e != NULL; e = e->next) {
		e->x += e->dx;
		e->y += e->dy;

		if(--e->a <= 0) {
			if(e == s->explosion_tail)
				s->explosion_tail = prev;

			prev->next = e->next;
			free(e);
			e = prev;
		}
		prev = e;
	}
}

void stage::add_explosions(int x, int y, int num)
{
	stage *s = this;
	explosion *ex;
	int i;
	for(i = 0; i < num; i++) {
		ex = (explosion*) calloc(1, sizeof(explosion));
		s->explosion_tail->next = ex;
		s->explosion_tail = ex;

		ex->x = x + (rand() % 32) - (rand() % 32);
		ex->y = y + (rand() % 32) - (rand() % 32);
		ex->dx = (rand() % 10) - (rand() % 10);
		ex->dy = (rand() % 10) - (rand() % 10);
		
		ex->dx /= 10;
		ex->dy /= 10;

		switch (rand() % 4) {
			case 0:
				ex->r = 255;
				break;
			case 1:
				ex->r = 255;
				ex->g = 128;
				break;
			case 2:
				ex->r = 255;
				ex->g = 255;
				break;
			default:
				ex->r = 255;
				ex->g = 255;
				ex->b = 255;
				break;
		}
		ex->a = rand() % FPS * 3;
	}
}

void stage::do_player(int *keyboard)
{
	stage *s = this;
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
			s->fire_bullet();
		
		player->x += player->dx;
		player->y += player->dy;
	}
}

void stage::fire_bullet()
{
	stage *s = this;
	entity *bullet = entity::create_entity(player->x, player->y, SIDE_PLAYER, bullet_texture);
	
	s->bullet_tail->next = bullet;
	s->bullet_tail = bullet;
	
	bullet->dx = PLAYER_BULLET_SPEED;
	
	bullet->y += (player->h / 2) - (bullet->h / 2);
	
	player->reload = 8;
}

void stage::do_enemies()
{
	stage *s = this;
	entity *e;
	for(e = s->fighter_head.next; e != NULL; e = e->next) {
		if(e != player && player != NULL && --e->reload <= 0)
			s->fire_alien_bullet(e);
	}
}

void stage::fire_alien_bullet(entity *e)
{
	stage *s = this;
	entity *bullet = entity::create_entity(e->x, e->y, e->side, alien_bullet_texture);
	s->bullet_tail->next = bullet;
	s->bullet_tail = bullet;

	bullet->x += (e->w / 2) - (bullet->w / 2);
	bullet->y += (e->h / 2) - (bullet->h / 2);

	calc_slope(player->x + (player->w / 2), player->y + (player->h / 2), e->x, e->y, &bullet->dx, &bullet->dy);

	bullet->dx *= ALIEN_BULLET_SPEED;
	bullet->dy *= ALIEN_BULLET_SPEED;

	bullet->side = SIDE_ALIEN;

	e->reload = (rand() % FPS * 2);
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
	entity *p = player;
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

void stage::do_bullets()
{
	stage *s = this;
	entity *b;
	entity *prev = &s->bullet_head;
		
	for (b = s->bullet_head.next; b != NULL; b = b->next) {
		b->x += b->dx;
		b->y += b->dy;
		
		if (bullet_hit_fighter(b, s) || b->x < -b->w || b->y < -b->h || b->x > SCREEN_WIDTH || b->y > SCREEN_HEIGHT) {
			if (b == s->bullet_tail)
				s->bullet_tail = prev;
			
			prev->next = b->next;
			free(b);
			b = prev;
		}
		prev = b;
	}
}

static int collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
	return (MAX(x1, x2) < MIN(x1 + w1, x2 + w2)) && (MAX(y1, y2) < MIN(y1 + h1, y2 + h2));
}

static int bullet_hit_fighter(entity *b, stage *s)
{
	entity *e;

	for(e = s->fighter_head.next; e != NULL; e = e->next) {
		if(e->side != b->side && collision(b->x, b->y, b->w, b->h, e->x, e->y, e->w, e->h)) {
			b->health = 0;
			e->health--;

			if(e->health == 0) {
				s->add_explosions(e->x, e->y, 32);
				debris::add_debris(e);
				if(e != player)
					s->score++;
			}
			
			return 1;

		}
	}
	return 0;
}

void calc_slope(int x1, int y1, int x2, int y2, float *dx, float *dy)
{
	int steps = MAX(abs(x1 - x2), abs(y1 - y2));

	if (steps == 0) {
		*dx = *dy = 0;
		return;
	}

	*dx = (x1 - x2);
	*dx /= steps;

	*dy = (y1 - y2);
	*dy /= steps;
}

void stage::draw(SDL_Renderer *r)
{
	stage *s = this;
	drawer::draw_background(r);
	s->draw_starfield(r);

	if(player != NULL)
		drawer::blit(player->texture, player->x, player->y, r);
	
	entity *b;
	
	for (b = s->bullet_head.next; b != NULL; b = b->next)
		drawer::blit(b->texture, b->x, b->y, r);

	entity *e;
	
	for (e = s->fighter_head.next; e != NULL ; e = e->next)
		drawer::blit(e->texture, e->x, e->y, r);
	
	debris::draw_debris(r);
	s->draw_explosions(r);
	s->draw_hud(player, r);
}
