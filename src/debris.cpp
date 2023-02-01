#include "debris.h"
#include "stage/stage.h"
#include "drawer.h"

debris debris::debris_head;
debris* debris::debris_tail = &debris::debris_head;

void debris::add_debris(entity *e)
{
	// stage *s = this->s;
	debris *d;
	int x, y, w, h;

	w = e->w / 2;
	h = e->h / 2;

	for(y = 0; y <= h; y += h) {
		for(x = 0; x <= w; x += w) {
			d = (debris*) calloc(1, sizeof(debris));
			debris::debris_tail->next = d;
			debris::debris_tail = d;

			d->x = e->x + e->w / 2;
			d->y = e->y + e->h / 2;

			if(e->side == SIDE_PLAYER)
				d->dx = (rand() % 9);
			else
				d->dx = -(rand() % 9);

			d->dy = rand() % 3 + (-2) + 1;

			d->life = FPS * 2;
			d->texture = e->texture;

			d->rect.x = x;
			d->rect.y = y;
			d->rect.w = w;
			d->rect.h = h;
		}
	}
}


void debris::do_debris() {
	// stage *s = this->s;
	debris *d;
	debris *prev = &debris::debris_head;

	for (d = debris::debris_head.next ; d != NULL ; d = d->next) {
		d->x += d->dx;
		d->y += d->dy;

		if (--d->life <= 0) {
			if (d == debris::debris_tail)
				debris::debris_tail = prev;

			prev->next = d->next;
			free(d);
			d = prev;
		}
		prev = d;
	}
}


void debris::draw_debris(SDL_Renderer *r)
{
	// stage *s = this->s;
	debris *d;

	for (d = debris::debris_head.next ; d != NULL ; d = d->next)
		blit_rect(d->texture, &d->rect, d->x, d->y, r);
}
