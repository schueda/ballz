#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Allegro */
#include <allegro5/allegro.h>
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro_native_dialog.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"

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

enum {MENU, WAITING, AIMING, SHOOTING, GAMEOVER} state;

void draw_menu(window *win) {
	al_clear_to_color(PRETO);

	char *path = al_get_current_directory();
	strcat(path, "/dimitri/dimitri.ttf");
	ALLEGRO_FONT *font = al_load_ttf_font(path, 100, 0);

	al_draw_text(font, BRANCO, win->disp_data.width * 0.5, win->disp_data.height * 0.2, ALLEGRO_ALIGN_CENTRE, "BALLz");
	
	// ALLEGRO_BITMAP *button = al_load_bitmap("button.png");
	
	// al_draw_bitmap(button, (win->disp_data.width - al_get_bitmap_width(button)) * 0.5 , win->disp_data.height * 0.6, 0);

	al_flip_display();
}

void draw_aim(window *win, bouncer_t *bouncer, square_t *square, float distX, float distY, float dist) {
	if(al_is_event_queue_empty(win->event_queue)) {
		al_clear_to_color(PRETO);
		al_draw_filled_circle(bouncer->x, bouncer->y, BOUNCER_SIZE, BRANCO);
		al_draw_filled_rectangle(10, 10, 100, 100, CINZA);

		float size = min(win->disp_data.height * 0.33 + dist, win->disp_data.height * 0.7);
		float spacing = (size - 80)/16;
		float position = size;

		int i;
		for(i = 0; i < 16; i++) {
			al_draw_filled_circle(bouncer->x + position * distX/dist, bouncer->y + position * distY/dist, BOUNCER_SIZE * 0.6 * size/(win->disp_data.height * 0.7), BRANCO);
			position -= spacing;
		}

		al_draw_filled_triangle(bouncer->x + (BOUNCER_SIZE + 70) * distX/dist, 
								bouncer->y + (BOUNCER_SIZE + 70) * distY/dist,

								bouncer->x + (BOUNCER_SIZE + 5) * distX/dist + 7 * distY/dist, 
								bouncer->y + (BOUNCER_SIZE + 5) * distY/dist - 7 * distX/dist, 

								bouncer->x + (BOUNCER_SIZE + 5) * distX/dist - 7 * distY/dist, 
								bouncer->y + (BOUNCER_SIZE + 5) * distY/dist + 7 * distX/dist, BRANCO);
		al_flip_display();
		
	}
}

void draw_shoot(window *win, bouncer_t *bouncer, square_t *square) {
	if(al_is_event_queue_empty(win->event_queue)) {
		al_clear_to_color(PRETO);
		al_draw_filled_circle(bouncer->x, bouncer->y, BOUNCER_SIZE, BRANCO);
		al_draw_filled_rectangle(10, 10, 100, 100, CINZA);
		al_flip_display();
	}
}

int main(int argc, char *argv[]) {
	window win;

	state = MENU;

	bool sair = false;
	bool menuDrew = false;
	bool can_shoot = false;


	ALLEGRO_MOUSE_EVENT mouse_down;
	bouncer_t *bouncer = malloc(sizeof(bouncer_t));

	win = graphinit(RES_WIDTH, RES_HEIGHT);

	float shooting_y = win.disp_data.height * 0.8;

	bouncer->x = win.disp_data.width / 2.0 - BOUNCER_SIZE / 2.0;
	bouncer->y = shooting_y;


	while (!sair) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(win.event_queue, &ev);

		int distX;
		int distY;
		float dist;

		switch (state) {
		case MENU:
			if (!menuDrew) {
				draw_menu(&win);
				menuDrew = true;	
			}
			if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
				state = WAITING;
				draw_shoot(&win, bouncer, NULL);
			}
			break;

		case WAITING:
			if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
				mouse_down = ev.mouse;
				
				state = AIMING;
			} 
			break;
		
		case AIMING:
			distX = MOUSE_SENSIBILITY * (mouse_down.x - ev.mouse.x);
			distY = MOUSE_SENSIBILITY * (mouse_down.y - ev.mouse.y);
			dist = sqrt(pow(distX, 2) + pow(distY, 2));

			if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
				if (can_shoot) {
					bouncer->dx = SPEED_FACTOR * (distX)/dist;
					bouncer->dy = SPEED_FACTOR * (distY)/dist;
					state = SHOOTING;
				} else {
					state = WAITING;
				}
			} else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES) {
				if((dist > BOUNCER_SIZE) && (fabs((float) distY/distX) > 0.06) && (distY < 0)) {
					draw_aim(&win, bouncer, NULL, distX, distY, dist);
					can_shoot = true;
				} else {
					draw_shoot(&win, bouncer, NULL);
					can_shoot = false;
				}
			}
			break;

		case SHOOTING:
			if (ev.type == ALLEGRO_EVENT_TIMER) {
				if (bouncer->x < 0 + BOUNCER_SIZE || bouncer->x > win.disp_data.width - BOUNCER_SIZE) {
					bouncer->dx = -bouncer->dx;
				}

				if (bouncer->y < 0 + BOUNCER_SIZE) {
					bouncer->dy = -bouncer->dy;
				}
				
				if ((bouncer->dy > 0) && (bouncer->y > shooting_y - BOUNCER_SIZE)) {
					state = WAITING;
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