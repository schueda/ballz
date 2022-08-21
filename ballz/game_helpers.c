#include "game_helpers.h"

float calcSquareSide(float dispWidth) {
    return dispWidth / 7.8;
}

float calcSquareXi(float j, float l) {
    return (j * 1.1*l) + 0.1*l;
}

float calcSquareYi(float i, float l) {
    return i * 1.1*l;
}

float calcSquareMidX(float j, float l) {
    return (calcSquareXi(j, l) + calcSquareXf(j, l))/2;
};

float calcSquareMidY(float i, float l) {
    return (calcSquareYi(i, l) + calcSquareYf(i, l))/2;
}

float calcSquareXf(float j, float l) {
    return (j+1) * 1.1*l;
}

float calcSquareYf(float i, float l) {
    return i * 1.1*l + l;
}


int calcjMatrixPositioni(float xi, float l) {
    return (xi/(1.1*l)) - 1/11;
}

int calciMatrixPositioni(float yi, float l) {
    return yi/(1.1*l);
}

int calcjMatrixPositionf(float xf, float l) {
    return (xf/(1.1*l)) - 1;
}

int calciMatrixPositionf(float yf, float l) {
    return (yf/(1.1*l)) - 10/11;
}


bouncer_t *createBouncer(float x, float y) {
	bouncer_t *bouncer = malloc(sizeof(bouncer_t));
	bouncer->dx = 0;
	bouncer->dy = 0;
	bouncer->x = x;
	bouncer->y = y;

	return bouncer;
}

void setupGame(game_t *game, float dispWidth) {
	game->score = 0;
	game->bouncers = 1;
	game->dx = 0;
	game->dy = 0;
	game->shooting_x = (dispWidth - BOUNCER_RADIUS) / 2.0;
}

void setupSquares(int squares[][COLUNAS_QUADRADO]) {
	for (int i = 0; i < LINHAS_QUADRADO; ++i) {
		for (int j = 0; j < COLUNAS_QUADRADO; ++j) {
			squares[i][j] = 0;
		}
	}
}

void setupBouncers(bouncer_t ***bouncers, float dispWidth, float shooting_y) {
	*bouncers = calloc(sizeof(bouncer_t), 1);
	*bouncers[0] = createBouncer(dispWidth * 0.5, shooting_y);
}

void destroyBouncers(bouncer_t **bouncers, game_t *game) {
	for(int i = 0; i < game->bouncers; ++i) {
		free(bouncers[i]);
	}
	free(bouncers);
}