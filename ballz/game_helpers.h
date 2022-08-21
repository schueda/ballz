#include <stdio.h>
#include <stdlib.h>

#define BOUNCER_RADIUS 12

#define SPEED_FACTOR 20

#define MOUSE_SENSIBILITY 2

#define LINHAS_QUADRADO 8
#define COLUNAS_QUADRADO 7

typedef struct bouncer bouncer_t;
struct bouncer {
	float x;
	float y;

	float dx;
	float dy;
};

typedef struct game game_t;
struct game {
	int score;
	int bouncers;

	float dx;
	float dy;

	float shooting_x;
};

float calcSquareSide(float dispWidth);

float calcSquareXi(float j, float l);
float calcSquareYi(float i, float l);
float calcSquareMidX(float j, float l);
float calcSquareMidY(float i, float l);
float calcSquareXf(float j, float l);
float calcSquareYf(float i, float l);

int calcjMatrixPositioni(float xi, float l);
int calciMatrixPositioni(float yi, float l);
int calcjMatrixPositionf(float xf, float l);
int calciMatrixPositionf(float yf, float l);

bouncer_t *createBouncer(float x, float y);

void setupGame(game_t *game, float dispWidth);
void setupSquares(int squares[][COLUNAS_QUADRADO]);
void setupBouncers(bouncer_t ***bouncers, float dispWidth, float shooting_y);
void destroyBouncers(bouncer_t **bouncers, game_t *game);
