#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <string.h>

#include "game.h"
#include "entity.h"
#include "stage/stage.h"
#include "channels.h"
#include "sounds.h"

#define PLAYER_SPEED 4
#define PLAYER_BULLET_SPEED 16
#define CODE event->keysym.scancode

game *game::init_game()
{
	
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	game *g = (game*) malloc(sizeof(game));
	g->is_running = 1;
	memset(g->keyboard, 0, sizeof g->keyboard);
	g->window = SDL_CreateWindow("Shooter 01", 0, 0, 1280, 720, 0);
	g->renderer = SDL_CreateRenderer(g->window, -1, SDL_RENDERER_ACCELERATED);
	g->s = stage::init_stage(g->renderer);

	SDL_SetRenderDrawColor(g->renderer, 96, 128, 255, 255);
	
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

	return g;
}

void game::handle_input()
{
	game *g = this;
	SDL_Event event;
	
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				g->is_running = 0;
				break;
			case SDL_KEYDOWN:
				g->doKeyDown(&event.key);
				break;
			case SDL_KEYUP:
				g->doKeyUp(&event.key);
			default:
				break;
		}
	}
		
}

void game::update()
{
	game *g = this;
	g->s->do_logic(g->keyboard);
}

void game::render()
{
	game *g = this;
	SDL_RenderClear(g->renderer);
	g->s->draw(g->renderer);
	SDL_RenderPresent(g->renderer);
}

void game::cleanup()
{
	game *g = this;
	SDL_DestroyRenderer(g->renderer);

	
	SDL_DestroyWindow(g->window);
	
	SDL_Quit();
}



void game::doKeyDown(SDL_KeyboardEvent *event)
{
	game *g = this;
	if (event->repeat == 0 && CODE < MAX_KEYBOARD_KEYS)
		g->keyboard[CODE] = 1;
	
}
void game::doKeyUp(SDL_KeyboardEvent *event)
{
	game *g = this;
	if (event->repeat == 0 && CODE < MAX_KEYBOARD_KEYS)
		g->keyboard[CODE] = 0;
}
