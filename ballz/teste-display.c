#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Allegro */
#include <allegro5/allegro.h>
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_native_dialog.h"

/* Graficos */
#include "grafico.h"

#define RES_WIDTH 800
#define RES_HEIGHT 600

#define BOUNCER_SIZE 80

//-----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
  Window win;
  ALLEGRO_BITMAP *image;

  win = graphinit(RES_WIDTH, RES_HEIGHT);

  image = imagemArq("Bug_01.png", BOUNCER_SIZE, BOUNCER_SIZE,win);

  al_clear_to_color(VERDE);
  al_draw_bitmap(image, 200, 200, 0);
  al_flip_display();

  printf("\n\nENTER para terminar: ");
  getchar();

  al_destroy_bitmap(image);

  graphdeinit(win);
}

