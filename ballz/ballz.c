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

enum {MENU, SETUP, WAITING, AIMING, SHOOTING, GAMEOVER} state;

int main(int argc, char *argv[]) {
	window win = graphinit(RES_WIDTH, RES_HEIGHT);
	srand(time(NULL));

	state = MENU;

	int i,j;

	bool menuDrew = false;
	bool canShoot = false;
	bool addedNewSquares = false;
	bool willAddNewBouncers = false;

	int launchInterval = 0;
	int launchIndex = 0;
	int arrivalCounter = 0;
	int newBouncersCount = 0;

	float offset=0;

	ALLEGRO_MOUSE_EVENT mouse_down;

	float squareSide = calcSquareSide(win.disp_data.width);
	float shooting_y = calcSquareYf(8, squareSide) - BOUNCER_RADIUS;

	game_t game;
	int squares[LINHAS_QUADRADO][COLUNAS_QUADRADO];
	bouncer_t **bouncers;
	

	while (1) {
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
				state = SETUP;
				setupGame(&game, win.disp_data.width);
				setupSquares(squares);
				setupBouncers(&bouncers, win.disp_data.width, shooting_y);
				menuDrew = false;
				canShoot = false;
				addedNewSquares = false;
				willAddNewBouncers = false;

				launchInterval = 0;
				launchIndex = 0;
				arrivalCounter = 0;
				newBouncersCount = 0;

				offset=0;
			}
			break;

		case SETUP:
			if (!addedNewSquares) {
				game.score++;
				for (j=0; j<COLUNAS_QUADRADO; j++) {
					squares[0][j] = (rand() % 3) * game.score;
				}
				if (game.score > 1) {
					int newBallPosition = rand() % COLUNAS_QUADRADO;
					squares[0][newBallPosition] = -1;
				}
				addedNewSquares = true;
			}
			if(ev.type == ALLEGRO_EVENT_TIMER) {
				if (offset<(1.1 * squareSide)) {
					draw_setup(&win, bouncers[0], squares, offset);
					offset += 2;
				} else {
					offset = 0;
					for (j=0; j<COLUNAS_QUADRADO; j++) {
						if (squares[LINHAS_QUADRADO-1][j] > 0) {
							state = GAMEOVER;
						}
					}
					if (state != GAMEOVER) {
						for (i=LINHAS_QUADRADO-1; i>0; i--) {
							for (j=0; j<COLUNAS_QUADRADO; j++) {
								squares[i][j] = squares[i-1][j];
							}
						}
						for (j=0; j<COLUNAS_QUADRADO; j++) {
							squares[0][j] = 0;
						};
						state = WAITING;
						draw_wait(&win, bouncers[0], squares);
					}  else {
						destroyBouncers(bouncers, &game);
						draw_gameover(&win, &game);
					};
					addedNewSquares = false;
				};
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
				if (canShoot) {
					game.dx = SPEED_FACTOR * (distX)/dist;
					game.dy = SPEED_FACTOR * (distY)/dist;

					state = SHOOTING;
					canShoot = false;
				} else {
					state = WAITING;
				}
			} else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES) {
				if((dist > BOUNCER_RADIUS) && (fabs((float) distY/distX) > 0.06) && (distY < 0)) {
					draw_aim(&win, bouncers[0], distX, distY, dist, squares);
					canShoot = true;
				} else {
					draw_wait(&win, bouncers[0], squares);
					canShoot = false;
				}
			}
			break;

		case SHOOTING:
			if (ev.type == ALLEGRO_EVENT_TIMER) {
				if (willAddNewBouncers) {
					game.bouncers += newBouncersCount;
					bouncer_t **newBouncers = realloc(bouncers, sizeof(bouncer_t) * game.bouncers);
					if (newBouncers == NULL) {
						perror("Error reallocking bouncers");
						exit(1);
					}
					bouncers = newBouncers;

					for (i=game.bouncers - newBouncersCount; i<game.bouncers; i++) {
						bouncers[i] = createBouncer(game.shooting_x, shooting_y);
					}

					newBouncersCount = 0;
					willAddNewBouncers = false;
				}
				
				launchInterval++;
				if (launchInterval >= 5 && launchIndex < game.bouncers) {
					launchInterval = 0;
					bouncers[launchIndex]->x = game.shooting_x;
					bouncers[launchIndex]->y = shooting_y;
					bouncers[launchIndex]->dx = game.dx;
					bouncers[launchIndex]->dy = game.dy;
					launchIndex++;
				}

				for (i=0; i<launchIndex; i++) {
					if (bouncers[i] != NULL) {
						if (bouncers[i]->x <= BOUNCER_RADIUS) {
							bouncers[i]->dx = -bouncers[i]->dx;
						}
							
						if (bouncers[i]->x >= win.disp_data.width - BOUNCER_RADIUS) {
							bouncers[i]->dx = -bouncers[i]->dx;
						}

						if (bouncers[i]->y <= BOUNCER_RADIUS) {
							bouncers[i]->dy = -bouncers[i]->dy;
						}


						for (int c=0; c<COLUNAS_QUADRADO; c++) {
							if(calcSquareXi(c, squareSide) <= bouncers[i]->x && calcSquareXf(c, squareSide) >= bouncers[i]->x) {
								for (int l=1; l<LINHAS_QUADRADO; l++) {
									if (squares[l][c] > 0
									&& calcSquareYi(l, squareSide) <= bouncers[i]->y + BOUNCER_RADIUS
									&& calcSquareYf(l, squareSide) >= bouncers[i]->y - BOUNCER_RADIUS) {
										bouncers[i]->dy = -bouncers[i]->dy;
										squares[l][c]--;
									}
									if (squares[l][c] < 0
									&& calcSquareMidY(l, squareSide) <= bouncers[i]->y + 2 * BOUNCER_RADIUS
									&& calcSquareMidY(l, squareSide) >= bouncers[i]->y - 2 * BOUNCER_RADIUS) {
										squares[l][c] = 0;
										newBouncersCount++;
									}
								}
							}
						}

						for (int l=0; l<LINHAS_QUADRADO; l++) {
							if(calcSquareYi(l, squareSide) <= bouncers[i]->y && calcSquareYf(l, squareSide) >= bouncers[i]->y) {
								for (int c=1; c<COLUNAS_QUADRADO; c++) {
									if (squares[l][c] > 0
										&& calcSquareXi(c, squareSide) <= bouncers[i]->x + BOUNCER_RADIUS 
										&& calcSquareXf(c, squareSide) >= bouncers[i]->x - BOUNCER_RADIUS) {
										bouncers[i]->dx = -bouncers[i]->dx;
										squares[l][c]--;
									}
									if (squares[l][c] < 0
										&& calcSquareMidX(c, squareSide) <= bouncers[i]->x + 2 * BOUNCER_RADIUS 
										&& calcSquareMidX(c, squareSide) >= bouncers[i]->x - 2 * BOUNCER_RADIUS) {
										squares[l][c] = 0;
										newBouncersCount++;
									}
								}
							}
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
							}

							if (arrivalCounter == game.bouncers) {
								bouncer_t *auxBouncer = bouncers[i];
								bouncers[i] = bouncers[game.bouncers - 1];
								bouncers[game.bouncers - 1] = auxBouncer;
							}
						}

						if (arrivalCounter > 0 && bouncers[i]->y >= shooting_y) {
							if (bouncers[i]->x > bouncers[0]->x) {
								bouncers[i]->x -= SPEED_FACTOR;
							} 
							if (bouncers[i]->x < bouncers[0]->x) {
								bouncers[i]->x += SPEED_FACTOR;
							}
							if (bouncers[i]->x <= bouncers[0]->x + SPEED_FACTOR && bouncers[i]->x >= bouncers[0]->x - SPEED_FACTOR) {
								bouncers[i]->x = bouncers[0]->x;

								if (arrivalCounter == game.bouncers && i == game.bouncers - 1) {
									state = SETUP;
									willAddNewBouncers = (newBouncersCount > 0);

									launchIndex = 0;
									game.shooting_x = bouncers[0]->x;
									arrivalCounter = 0;
								}
							}
						}
					}
				}
				draw_shoot(&win, bouncers, game.bouncers, squares);
			}
			break;

		case GAMEOVER:
			if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
				state = MENU;
			}
			break;

		default: break;
		}

		if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
		} 
	}

	graphdeinit(win);
	return 0;
}