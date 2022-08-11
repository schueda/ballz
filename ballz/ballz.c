#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Allegro */
#include <allegro5/allegro.h>
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro_native_dialog.h"

/* Graficos */
#include "grafico.h"

#define RES_WIDTH 600
#define RES_HEIGHT 800

#define BOUNCER_SIZE 15
#define SPEED_FACTOR 20

#define MOUSE_SENSIBILITY 2

typedef struct mask mask_t;
struct mask {
	int width;
	int height;
	int *bits;
};

typedef struct bouncer bouncer_t;
struct bouncer {
	float x;
	float y;

	float dx;
	float dy;
};

typedef struct square square_t;
struct square {
	float x;
	float y;

	int hits;
};

typedef struct game game_t;
struct game {
	int score;
	int balls;
	float shooting_x;
};

#define min(x, y) (((x) < (y)) ? (x) : (y))

enum {MENU, SHOOTING, WATCHING, GAMEOVER} state;

void draw_menu() {
	al_clear_to_color(PRETO);
	al_flip_display();
}

void draw_aim(Window *win, bouncer_t *bouncer, square_t *square, ALLEGRO_MOUSE_EVENT *mouse_down, ALLEGRO_MOUSE_EVENT *mouse_pos) {
	if(al_is_event_queue_empty(win->event_queue)) {
		al_clear_to_color(PRETO);
		al_draw_filled_circle(bouncer->x, bouncer->y, BOUNCER_SIZE, BRANCO);
		al_draw_filled_rectangle(10, 10, 100, 100, CINZA);

		float distX = MOUSE_SENSIBILITY * (mouse_pos->x - mouse_down->x);
		float distY = MOUSE_SENSIBILITY * (mouse_pos->y - mouse_down->y);
		float dist = sqrt(pow(distX, 2) + pow(distY, 2));
		float mult = min(win->disp_data.height * 0.25 + dist, win->disp_data.height * 0.7);

		al_draw_filled_circle(bouncer->x - mult * distX/dist, bouncer->y - mult * distY/dist, BOUNCER_SIZE * 0.7, BRANCO);
		al_draw_filled_triangle(bouncer->x - (BOUNCER_SIZE + 70) * distX/dist, 
								bouncer->y - (BOUNCER_SIZE + 70) * distY/dist,

								bouncer->x - (BOUNCER_SIZE + 5) * distX/dist + 7 * distY/dist, 
								bouncer->y - (BOUNCER_SIZE + 5) * distY/dist - 7 * distX/dist, 

								bouncer->x - (BOUNCER_SIZE + 5) * distX/dist - 7 * distY/dist, 
								bouncer->y - (BOUNCER_SIZE + 5) * distY/dist + 7 * distX/dist, BRANCO);
		al_flip_display();
		
	}
}

void draw_shoot(Window *win, bouncer_t *bouncer, square_t *square) {
	if(al_is_event_queue_empty(win->event_queue)) {
		al_clear_to_color(PRETO);
		al_draw_filled_circle(bouncer->x, bouncer->y, BOUNCER_SIZE, BRANCO);
		al_draw_filled_rectangle(10, 10, 100, 100, CINZA);
		al_flip_display();
	}
}

int main(int argc, char *argv[]) {
	Window win;

	state = MENU;

	bool holding = false;
	bool menuDrew = false;

	bool sair = false;

	ALLEGRO_MOUSE_EVENT mouse_down;
	bouncer_t *bouncer = malloc(sizeof(bouncer_t));

	win = graphinit(RES_WIDTH, RES_HEIGHT);

	float shooting_y = win.disp_data.height * 0.8;

	bouncer->x = win.disp_data.width / 2.0 - BOUNCER_SIZE / 2.0;
	bouncer->y = shooting_y;

	while (!sair) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(win.event_queue, &ev);

		switch (state) {
		case MENU:
			if (!menuDrew) {
				draw_menu();
				menuDrew = true;	
			}
			if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
				state = SHOOTING;
				draw_shoot(&win, bouncer, NULL);
			}
			break;

		case SHOOTING:
			if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
				mouse_down = ev.mouse;
				holding = true;
			} else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
				holding = false;
				draw_shoot(&win, bouncer, NULL);
				int distX = mouse_down.x - ev.mouse.x;
				int distY = mouse_down.y - ev.mouse.y;
				float dist = sqrt(pow(distX, 2) + pow(distY, 2));
				if((dist > BOUNCER_SIZE) && (fabs((float) distY/distX) > 0.1) && (distY < 0))  {
					bouncer->dx = SPEED_FACTOR * (distX)/dist;
					bouncer->dy = SPEED_FACTOR * (distY)/dist;
					state = WATCHING;
				}
			} else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES && holding) {
				int distX = MOUSE_SENSIBILITY * (mouse_down.x - ev.mouse.x);
				int distY = MOUSE_SENSIBILITY * (mouse_down.y - ev.mouse.y);
				float dist = sqrt(pow(distX, 2) + pow(distY, 2));
				if((dist > BOUNCER_SIZE) && (fabs((float) distY/distX) > 0.1) && (distY < 0)) {
					draw_aim(&win, bouncer, NULL, &mouse_down, &ev.mouse);
				} else {
					draw_shoot(&win, bouncer, NULL);
				}
			}
			break;


		case WATCHING:
			if (ev.type == ALLEGRO_EVENT_TIMER) {
				if (bouncer->x < 0 + BOUNCER_SIZE || bouncer->x > win.disp_data.width - BOUNCER_SIZE) {
					bouncer->dx = -bouncer->dx;
				}

				if (bouncer->y < 0 + BOUNCER_SIZE) {
					bouncer->dy = -bouncer->dy;
				}
				
				if ((bouncer->dy > 0) && (bouncer->y > shooting_y - BOUNCER_SIZE)) {
					state = SHOOTING;
					bouncer->dx = 0;
					bouncer->dy = 0;
				}

				bouncer->x += bouncer->dx;
				bouncer->y += bouncer->dy;

				draw_shoot(&win, bouncer, NULL);
			}
			break;

		case GAMEOVER:
			break;

		default: break;
		}

		if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
		} 
	}

	free(bouncer);
	graphdeinit(win);
	return 0;
}