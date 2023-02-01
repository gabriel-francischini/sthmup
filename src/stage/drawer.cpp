#include "stage/stage.h"
#include "drawer.h"

/*
	DEFINING TEXTURES FROM GLOBALS.H
*/
SDL_Texture *player_texture;
SDL_Texture *bullet_texture;
SDL_Texture *font_texture;
SDL_Texture *enemy_texture;
SDL_Texture *alien_bullet_texture;
SDL_Texture *background;

void drawer::init_draw(SDL_Renderer *r)
{
	bullet_texture = drawer::load_texture("gfx/playerBullet.png", r);
	enemy_texture = drawer::load_texture("gfx/enemy.png", r);
	alien_bullet_texture = drawer::load_texture("gfx/alienBullet.png", r);
	player_texture = drawer::load_texture("gfx/player.png", r);
	background = drawer::load_texture("gfx/background.png", r);
	explosion::explosion_texture = drawer::load_texture("gfx/explosion.png", r);
	font_texture = drawer::load_texture("gfx/font.png", r);
}

void stage::draw_starfield(SDL_Renderer *r)
{
	stage *s = this;
	int i, c;

	for (i = 0 ; i < MAX_STARS ; i++) {
		c = 32 * s->stars[i].speed;

		SDL_SetRenderDrawColor(r, c, c, c, 255);

		SDL_RenderDrawLine(r, s->stars[i].x, s->stars[i].y, s->stars[i].x + 3, s->stars[i].y);
	}
}


void stage::draw_hud(entity *player, SDL_Renderer *renderer)
{
	stage *s = this;
	drawer::draw_text(1080, 10, 255, 255, 255, renderer, "SCORE: %03d", s->score);
	if(player != NULL) {
		drawer::draw_text(1080, 50, 255, 255, 255, renderer, "HEALTH: %d", player->health);
	} else {
		drawer::draw_text(1080, 50, 255, 255, 255, renderer, "HEALTH: %d", 0);
		drawer::draw_text(400, 400, 255, 255, 255, renderer, "YOU GOT DESTROYED!");
	}
}
