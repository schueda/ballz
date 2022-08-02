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
  ALLEGRO_BITMAP *image, *image1, *image2, *imageaux;

  bool redraw = true, 
       sair = false;
  float bouncer_x, bouncer_y,
        bouncer_dx = -4.0, 
        bouncer_dy = 4.0;

  win = graphinit(RES_WIDTH, RES_HEIGHT);

  image1 = imagemArq("Bug_01.png", BOUNCER_SIZE, BOUNCER_SIZE,win);
  image2 = imagemArq("Bug_02.png", BOUNCER_SIZE, BOUNCER_SIZE, win);
  image = image1;

  bouncer_x = win.disp_data.width / 2.0 - BOUNCER_SIZE / 2.0;
  bouncer_y = win.disp_data.height / 2.0 - BOUNCER_SIZE / 2.0;

  /* Enquanto  a tecla ESC  n�o for  pressionada, ir� executar
     o c�digo especificado.
  */
  while (!sair) {
    ALLEGRO_EVENT ev;
    al_wait_for_event(win.event_queue, &ev);
    
    if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
      break;
    }
    else if(ev.type == ALLEGRO_EVENT_TIMER) {
 
      /* Bouncing image */
      if(bouncer_x < 0 || bouncer_x > win.disp_data.width - BOUNCER_SIZE)
	      bouncer_dx = -bouncer_dx;
      
      if(bouncer_y < 0 || bouncer_y > win.disp_data.height - BOUNCER_SIZE)
	      bouncer_dy = -bouncer_dy;
      
      bouncer_x += bouncer_dx;
      bouncer_y += bouncer_dy;

      redraw = true;
    }
    else if(ev.type == ALLEGRO_EVENT_MOUSE_AXES ||
	    ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY) {
      
      bouncer_x = ev.mouse.x;
      bouncer_y = ev.mouse.y;
    }
    else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
      image1 = image;
      image = image2;
      image2 = image1;
    }

    if(redraw && al_is_event_queue_empty(win.event_queue)) {
      redraw = false;
      al_clear_to_color(VERDE);
      al_draw_bitmap(image, bouncer_x, bouncer_y, 0);
      al_flip_display();
    }
}

  al_destroy_bitmap(image);

  graphdeinit(win);
}

