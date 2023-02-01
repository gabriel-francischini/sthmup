#include "drawer.h"
#include "entity/explosion.h"

#define MAX_LINE_LENGTH 1024
#define GLYPH_HEIGHT 28
#define GLYPH_WIDTH  18
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

static char draw_text_buffer[MAX_LINE_LENGTH];

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

void drawer::draw_background(SDL_Renderer *r)
{
	SDL_Rect dest;
	int x;
	
	for (x = background_x ; x < SCREEN_WIDTH ; x += SCREEN_WIDTH) {
		dest.x = x;
		dest.y = 0;
		dest.w = SCREEN_WIDTH;
		dest.h = SCREEN_HEIGHT;
		
		SDL_RenderCopy(r, background, NULL, &dest);
	}
}

void drawer::draw_text(int x, int y, int r, int g, int b, SDL_Renderer *renderer, char *format, ...)
{
	int i, len, c;
	SDL_Rect rect;
	va_list args;

	memset(&draw_text_buffer, '\0', sizeof(draw_text_buffer));

	va_start(args, format);
	vsprintf(draw_text_buffer, format, args);
	va_end(args);

	len = strlen(draw_text_buffer);

	rect.w = GLYPH_WIDTH;
	rect.h = GLYPH_HEIGHT;
	rect.y = 0;

	SDL_SetTextureColorMod(font_texture, r, g, b);

	for (i = 0 ; i < len ; i++) {
		c = draw_text_buffer[i];

		if (c >= ' ' && c <= 'Z') {
			rect.x = (c - ' ') * GLYPH_WIDTH;
			drawer::blit_rect(font_texture, &rect, x, y, renderer);
			//drawer::blit_rect(d->texture, &d->rect, d->x, d->y, r);
			x += GLYPH_WIDTH;
		}
	}
}

void drawer::blit(SDL_Texture *texture, int x, int y, SDL_Renderer *r)
{
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;

	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
	SDL_RenderCopy(r, texture, NULL, &dest);
}

void drawer::blit_rect(SDL_Texture *texture, SDL_Rect *src, int x, int y, SDL_Renderer *r)
{
	SDL_Rect dest;
	
	dest.x = x;
	dest.y = y;
	dest.w = src->w;
	dest.h = src->h;
	
	SDL_RenderCopy(r, texture, src, &dest);
}


SDL_Texture *drawer::load_texture(char *filename, SDL_Renderer *r)
{
	return IMG_LoadTexture(r, filename);
}

void drawer::calc_slope(int x1, int y1, int x2, int y2, float *dx, float *dy)
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
