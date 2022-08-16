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

#define BOUNCER_RADIUS 15

#define SPEED_FACTOR 20

#define MOUSE_SENSIBILITY 2

enum {MENU, WAITING, AIMING, SHOOTING, GAMEOVER} state;

int main(int argc, char *argv[]) {
	window win = graphinit(RES_WIDTH, RES_HEIGHT);

	state = MENU;

	bool sair = false;
	bool menuDrew = false;
	bool can_shoot = false;

	int launchInterval = 0;
	int launchIndex = 1;
	int arrivalCounter = 0;



	game_t game = {
		.bouncers = 20,
		.dx = 0,
		.dy = 0,
		.score = 1,
		.shooting_x = (win.disp_data.width - BOUNCER_RADIUS) / 2.0
	};

	ALLEGRO_MOUSE_EVENT mouse_down;


	bouncer_t **bouncers;
	bouncers = calloc(sizeof(bouncer_t) * game.bouncers, game.bouncers);
	bouncers[0] = malloc(sizeof(bouncer_t));

	float shooting_y = win.disp_data.height * 0.8;

	bouncers[0]->radius = BOUNCER_RADIUS;
	bouncers[0]->dx = 0;
	bouncers[0]->dy = 0;
	bouncers[0]->x = game.shooting_x;
	bouncers[0]->y = shooting_y;

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
				draw_wait(&win, bouncers[0]);
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
					game.dx = SPEED_FACTOR * (distX)/dist;
					game.dy = SPEED_FACTOR * (distY)/dist;

					bouncers[0]->dx = game.dx;
					bouncers[0]->dy = game.dy;

					state = SHOOTING;
				} else {
					state = WAITING;
				}
			} else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES) {
				if((dist > bouncers[0]->radius) && (fabs((float) distY/distX) > 0.06) && (distY < 0)) {
					draw_aim(&win, bouncers[0], distX, distY, dist);
					can_shoot = true;
				} else {
					draw_wait(&win, bouncers[0]);
					can_shoot = false;
				}
			}
			break;

		case SHOOTING:
			if (ev.type == ALLEGRO_EVENT_TIMER) {
				launchInterval++;
				if (launchInterval >= 5 && launchIndex < game.bouncers) {
					launchInterval = 0;
					bouncers[launchIndex] = malloc(sizeof(bouncer_t));
					bouncers[launchIndex]->dx = game.dx;
					bouncers[launchIndex]->dy = game.dy;
					bouncers[launchIndex]->x = game.shooting_x;
					bouncers[launchIndex]->y = shooting_y;
					bouncers[launchIndex]->radius = BOUNCER_RADIUS;
					launchIndex++;
				}

				for (int i=0; i<game.bouncers; i++) {
					if (bouncers[i]) {
						if (bouncers[i]->x <= bouncers[i]->radius || bouncers[i]->x >= win.disp_data.width - bouncers[i]->radius) {
							bouncers[i]->dx = -bouncers[i]->dx;
						}

						if (bouncers[i]->y <= bouncers[i]->radius) {
							bouncers[i]->dy = -bouncers[i]->dy;
						}

						bouncers[i]->x += bouncers[i]->dx;
						bouncers[i]->y += bouncers[i]->dy;
						
						if ((bouncers[i]->dy > 0) && (bouncers[i]->y >= shooting_y)) {
							arrivalCounter++;
							bouncers[i]->dx = 0;
							bouncers[i]->dy = 0;
								
							if (arrivalCounter == 1) {
								bouncer_t *auxBouncer = bouncers[i];
								bouncers[i] = bouncers[0];
								bouncers[0] = auxBouncer;
							} else {
								free(bouncers[i]);
								bouncers[i] = NULL;
							}

							if (arrivalCounter == game.bouncers) {
								state = WAITING;
								launchIndex = 1;
								game.shooting_x = bouncers[0]->x;
								arrivalCounter = 0;
							}
							
						}

					}
				}
				draw_shoot(&win, bouncers, game.bouncers);
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

	free(bouncers);
	graphdeinit(win);
	return 0;
}