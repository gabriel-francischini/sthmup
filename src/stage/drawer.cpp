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
SDL_Texture *explosion_texture;
SDL_Texture *background;

void init_draw(SDL_Renderer *r)
{
	bullet_texture = load_texture("gfx/playerBullet.png", r);
	enemy_texture = load_texture("gfx/enemy.png", r);
	alien_bullet_texture = load_texture("gfx/alienBullet.png", r);
	player_texture = load_texture("gfx/player.png", r);
	background = load_texture("gfx/background.png", r);
	explosion_texture = load_texture("gfx/explosion.png", r);
	font_texture = load_texture("gfx/font.png", r);
}

void stage::draw_explosions(SDL_Renderer *r)
{
	stage *s = this;
	explosion *ex;

	SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_ADD);
	SDL_SetTextureBlendMode(explosion_texture, SDL_BLENDMODE_ADD);

	for (ex = s->explosion_head.next ; ex != NULL ; ex = ex->next) {
		SDL_SetTextureColorMod(explosion_texture, ex->r, ex->g, ex->b);
		SDL_SetTextureAlphaMod(explosion_texture, ex->a);

		blit(explosion_texture, ex->x, ex->y, r);
	}

	SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);
}


void stage::draw_debris(SDL_Renderer *r)
{
	stage *s = this;
	debris *d;

	for (d = s->debris_head.next ; d != NULL ; d = d->next)
		blit_rect(d->texture, &d->rect, d->x, d->y, r);
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
	draw_text(1080, 10, 255, 255, 255, renderer, "SCORE: %03d", s->score);
	if(player != NULL) {
		draw_text(1080, 50, 255, 255, 255, renderer, "HEALTH: %d", player->health);
	} else {
		draw_text(1080, 50, 255, 255, 255, renderer, "HEALTH: %d", 0);
		draw_text(400, 400, 255, 255, 255, renderer, "YOU GOT DESTROYED!");
	}
}
