#ifndef __GAME__
#define __GAME__

#include <SDL2/SDL.h>
#include "entity.h"
#include "stage/stage.h"

#define MAX_KEYBOARD_KEYS 350

struct game {
	SDL_Window *window;
	SDL_Renderer *renderer;
	stage *s;
	int is_running;
	int keyboard[MAX_KEYBOARD_KEYS];

	static game *init_game();
	void handle_input();
	void update();
	void render();
	void cleanup();
};

#endif
