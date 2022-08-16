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

	float radius;
};

typedef struct square square_t;
struct square {
	float xi;
	float yi;
	float xf;
	float yf;

	int hits;
};

typedef struct game game_t;
struct game {
	int score;
	int bouncers;

	float dx;
	float dy;

	float shooting_x;
};