#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Allegro */
#include <allegro5/allegro.h>
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_native_dialog.h"

/* Graficos */
#include "grafico.h"

#define RES_WIDTH 600
#define RES_HEIGHT 800

#define BOUNCER_SIZE 20
#define SPEED_FACTOR 10

//-----------------------------------------------------------------------------

enum {MENU, SHOOTING, WATCHING, GAMEOVER} state;

int main(int argc, char *argv[]) {
	Window win;
	ALLEGRO_BITMAP *image, *image1, *image2;

	state = MENU;

	bool redraw = true;
	bool sair = false;

	float bouncer_x;
	float bouncer_y;
	float shooting_y;
	float bouncer_dx = 0;
	float bouncer_dy = 0;

	win = graphinit(RES_WIDTH, RES_HEIGHT);

	image1 = imagemArq("Ball.png", BOUNCER_SIZE, BOUNCER_SIZE, win);
	image2 = imagemArq("Ball.png", BOUNCER_SIZE, BOUNCER_SIZE, win);
	image = image1;

	bouncer_x = win.disp_data.width / 2.0 - BOUNCER_SIZE / 2.0;
	shooting_y = win.disp_data.height * 0.8;
	bouncer_y = shooting_y;

	while (!sair) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(win.event_queue, &ev);

		switch (state) {
		case MENU:
			if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
				
				state = SHOOTING;
			}
			break;

		case SHOOTING:
			if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
				break;
			} else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
				image1 = image;
				image = image2;
				image2 = image1;
				redraw = true;
			} else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
				int distX = bouncer_x - ev.mouse.x;
				int distY = bouncer_y - ev.mouse.y;
				int dist = sqrt(pow(distX, 2) + pow(distY, 2));
				bouncer_dx = SPEED_FACTOR * (distX)/dist;
				bouncer_dy = SPEED_FACTOR * (distY)/dist;
				image1 = image;
				image = image2;
				image2 = image1;
				state = WATCHING;
				redraw = true;
			}
			break;


		case WATCHING:
			if (ev.type == ALLEGRO_EVENT_TIMER) {
				if (bouncer_x < 0 || bouncer_x > win.disp_data.width - BOUNCER_SIZE)
					bouncer_dx = -bouncer_dx;

				if (bouncer_y < 0)
					bouncer_dy = -bouncer_dy;
				
				if ((bouncer_dy > 0) && (bouncer_y > shooting_y)) {
					state = SHOOTING;
					bouncer_dx = 0;
					bouncer_dy = 0;
				}

				bouncer_x += bouncer_dx;
				bouncer_y += bouncer_dy;

				redraw = true;
			}
			break;

		case GAMEOVER:
			break;

		default: break;
		}
		if (redraw && al_is_event_queue_empty(win.event_queue)) {
			redraw = false;
			al_clear_to_color(PRETO);
			al_draw_bitmap(image, bouncer_x, bouncer_y, 0);
			al_convert_mask_to_alpha(image, al_map_rgb(106, 76, 48));

			al_flip_display();
		}
	}

	al_destroy_bitmap(image);

	graphdeinit(win);
	return 0;
}