#include "star.h"
#include "GLOBALS.h"


star star::stars[MAX_STARS];


void star::init_starfield()
{
	// stage *s = this;
	int i;
	for(i = 0; i < MAX_STARS; i++) {
		star::stars[i].x = rand() % SCREEN_WIDTH;
		star::stars[i].y = rand() % SCREEN_HEIGHT;
		star::stars[i].speed = 1 + rand() % 8;
	}
}


void star::do_starfield()
{
	// stage *s = this;
	int i;

	for(i = 0; i < MAX_STARS; i++) {
		star::stars[i].x -= star::stars[i].speed;

		if(star::stars[i].x < 0)
			star::stars[i].x += SCREEN_WIDTH;
	}
}


void star::draw_starfield(SDL_Renderer *r)
{
	// stage *s = this;
	int i, c;

	for (i = 0 ; i < MAX_STARS ; i++) {
		c = 32 * star::stars[i].speed;

		SDL_SetRenderDrawColor(r, c, c, c, 255);

		SDL_RenderDrawLine(r, star::stars[i].x, star::stars[i].y, star::stars[i].x + 3, star::stars[i].y);
	}
}
