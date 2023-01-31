#include <stdio.h>
#include "game.h"

int main(void)
{
	game *g = game::init_game();
	while (g->is_running) {
		g->handle_input();
		g->update();
		g->render();
		SDL_Delay(16);
	}

	g->cleanup();

	return 0;
}
