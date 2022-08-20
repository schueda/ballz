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