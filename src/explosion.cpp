#include "explosion.h"
#include "drawer.h"
#include "stage/stage.h"

SDL_Texture *explosion::explosion_texture;
explosion explosion::explosion_head;
explosion *explosion::explosion_tail;


void explosion::add_explosions(int x, int y, int num)
{
	/* stage *s = this; */
	explosion *ex;
	int i;
	for(i = 0; i < num; i++) {
		ex = (explosion*) calloc(1, sizeof(explosion));
		explosion::explosion_tail->next = ex;
		explosion::explosion_tail = ex;

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


void explosion::do_explosions()
{
	/* stage *s = this; */
	explosion *e;
	explosion *prev = &explosion::explosion_head;

	for(e = explosion::explosion_head.next; e != NULL; e = e->next) {
		e->x += e->dx;
		e->y += e->dy;

		if(--e->a <= 0) {
			if(e == explosion::explosion_tail)
				explosion::explosion_tail = prev;

			prev->next = e->next;
			free(e);
			e = prev;
		}
		prev = e;
	}
}


void explosion::draw_explosions(SDL_Renderer *r)
{
	/* stage *s = this; */
	explosion *ex;

	SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_ADD);
	SDL_SetTextureBlendMode(explosion_texture, SDL_BLENDMODE_ADD);

	for (ex = explosion::explosion_head.next ; ex != NULL ; ex = ex->next) {
		SDL_SetTextureColorMod(explosion_texture, ex->r, ex->g, ex->b);
		SDL_SetTextureAlphaMod(explosion_texture, ex->a);

		drawer::blit(explosion_texture, ex->x, ex->y, r);
	}

	SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);
}
