#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "grafico.h"

#define min(x, y) (((x) < (y)) ? (x) : (y))

window graphinit(int res_width, int res_height) {
	int i;
	window win = {NULL, NULL, NULL, {0, 0, 0, 0}};

	/* Inicializa a Allegro */
	al_init();
	al_init_image_addon();
	al_init_native_dialog_addon();
	al_init_font_addon();
	al_init_ttf_addon();

	/* Define Janela */
	#ifdef __DEBUG__
		puts("Modos display a 60Hz disponíveis:\n");
	#endif

	for (i = 0; i < al_get_num_display_modes(); ++i) {
		al_get_display_mode(i, &win.disp_data);

		if (win.disp_data.refresh_rate == 60) { // 60 Hz
			#ifdef __DEBUG__
				printf("(%d): %d %d\n", i, win.disp_data.width, win.disp_data.height);
			#endif
			if (win.disp_data.width == res_width && win.disp_data.height == res_height)
				break;
		}
	}

	#ifdef __DEBUG__
		printf("\n\tModo selecionado --> (%d): %d %d\n", i, win.disp_data.width, win.disp_data.height);
	#endif

	al_get_display_mode(i, &win.disp_data);
	al_set_new_display_flags(ALLEGRO_WINDOWED);
	win.display = al_create_display(600,800/*win.disp_data.width, win.disp_data.height*/);
	win.disp_data.width = 600; //HARDCODED
	win.disp_data.height = 800;//HARDCODED

	al_install_mouse();
	al_install_keyboard();
	win.timer = al_create_timer(1.0 / win.disp_data.refresh_rate);

	/* Define Eventos */
	win.event_queue = al_create_event_queue();

	al_register_event_source(win.event_queue, al_get_display_event_source(win.display));
	al_register_event_source(win.event_queue, al_get_mouse_event_source());
	al_register_event_source(win.event_queue, al_get_keyboard_event_source());
	al_register_event_source(win.event_queue, al_get_timer_event_source(win.timer));

	al_clear_to_color(PRETO);
	al_flip_display();
	al_start_timer(win.timer);

	return (win);
}

void draw_menu(window *win) {
	// al_clear_to_color(PRETO);

	// char *path = al_get_current_directory();
	// strcat(path, "/dimitri/dimitri.ttf");
	// ALLEGRO_FONT *font = al_load_ttf_font(path, 100, 0);
	// printf("%p", font);
	// fflush(stdout);

	// al_draw_text(font, BRANCO, win->disp_data.width * 0.5, win->disp_data.height * 0.2, ALLEGRO_ALIGN_CENTRE, "BALLz");
	
	// ALLEGRO_BITMAP *button = al_load_bitmap("button.png");
	// al_draw_bitmap(button, (win->disp_data.width - al_get_bitmap_width(button)) * 0.5, win->disp_data.height * 0.6, 0);
	
	// free(path);
	// free(font);
	// free(button);

	// al_flip_display();
}

void draw_squares(window *win, int squares[][7]) {
	float l = win->disp_data.width / 7.8;
	int i, j;
	for (i = 0; i < 8; ++i) {
		for (j = 0; j < 7; ++j) {
			if (squares[i][j] == 1) {
				al_draw_filled_rectangle(i * 1.1*l + 0.1*l, j * 1.1*l, (i+1) * 1.1*l, j * 1.1*l + l, VERMELHO);
			}
		}
	}
}

void draw_ground(window *win, bouncer_t *bouncer) {
	al_draw_filled_rectangle(0, 8.8 * (win->disp_data.width)/7.8, win->disp_data.width, win->disp_data.height, CINZA_ESCURO);
}

void draw_wait(window *win, bouncer_t *bouncer, int squares[][7]) {
	if(al_event_queue_is_empty(win->event_queue)) {
		al_clear_to_color(PRETO);
		al_draw_filled_circle(bouncer->x, bouncer->y, bouncer->radius, BRANCO);
		draw_ground(win, bouncer);
		draw_squares(win, squares);
		al_flip_display();
	}
}

void draw_aim(window *win, bouncer_t *bouncer, float distX, float distY, float dist) {
	if(al_is_event_queue_empty(win->event_queue)) {
		al_clear_to_color(PRETO);
		al_draw_filled_circle(bouncer->x, bouncer->y, bouncer->radius, BRANCO);
		draw_ground(win, bouncer);

		float size = min(win->disp_data.height * 0.33 + dist, win->disp_data.height * 0.7);
		float spacing = (size - 80)/16;
		float position = size;

		int i;
		for(i = 0; i < 16; i++) {
			al_draw_filled_circle(bouncer->x + position * distX/dist, bouncer->y + position * distY/dist, bouncer->radius * 0.6 * size/(win->disp_data.height * 0.7), BRANCO);
			position -= spacing;
		}

		al_draw_filled_triangle(bouncer->x + (bouncer->radius + 70) * distX/dist, 
								bouncer->y + (bouncer->radius + 70) * distY/dist,

								bouncer->x + (bouncer->radius + 5) * distX/dist + 7 * distY/dist, 
								bouncer->y + (bouncer->radius + 5) * distY/dist - 7 * distX/dist, 

								bouncer->x + (bouncer->radius + 5) * distX/dist - 7 * distY/dist, 
								bouncer->y + (bouncer->radius + 5) * distY/dist + 7 * distX/dist, BRANCO);
		al_flip_display();
	}
}

void draw_shoot(window *win, bouncer_t **bouncers, int bouncersCount) {
	if(al_is_event_queue_empty(win->event_queue)) {
		al_clear_to_color(PRETO);
		for(int i=0; i<bouncersCount; i++) {
			if (bouncers[i]) {
				al_draw_filled_circle(bouncers[i]->x, bouncers[i]->y, bouncers[i]->radius, BRANCO);
			}
		}
		draw_ground(win, bouncers[0]);
		al_flip_display();
	}
}


void graphdeinit(window win) {
	al_destroy_timer(win.timer);
	al_destroy_event_queue(win.event_queue);
	al_destroy_display(win.display);
}

