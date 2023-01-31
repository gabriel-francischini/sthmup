#include "drawer.h"

#define MAX_LINE_LENGTH 1024
#define GLYPH_HEIGHT 28
#define GLYPH_WIDTH  18
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

static char draw_text_buffer[MAX_LINE_LENGTH];

void draw_background(SDL_Renderer *r)
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

void draw_text(int x, int y, int r, int g, int b, SDL_Renderer *renderer, char *format, ...)
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
			blit_rect(font_texture, &rect, x, y, renderer);
			//blit_rect(d->texture, &d->rect, d->x, d->y, r);
			x += GLYPH_WIDTH;
		}
	}
}

void blit(SDL_Texture *texture, int x, int y, SDL_Renderer *r)
{
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;

	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
	SDL_RenderCopy(r, texture, NULL, &dest);
}

void blit_rect(SDL_Texture *texture, SDL_Rect *src, int x, int y, SDL_Renderer *r)
{
	SDL_Rect dest;
	
	dest.x = x;
	dest.y = y;
	dest.w = src->w;
	dest.h = src->h;
	
	SDL_RenderCopy(r, texture, src, &dest);
}


SDL_Texture *load_texture(char *filename, SDL_Renderer *r)
{
	return IMG_LoadTexture(r, filename);
}
