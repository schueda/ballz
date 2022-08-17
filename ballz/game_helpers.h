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

float calcSquareXi(float i, float l);
float calcSquareYi(float j, float l);
float calcSquareXf(float i, float l);
float calcSquareYf(float j, float l);