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
#define GRID 4

//-----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
  Window win;
  ALLEGRO_BITMAP *image, *image1, *image2, *imageaux;

  bool key[4] = { false, false, false, false },
    redraw = true, 
    sair = false;
  float bouncer_x, bouncer_y,
    bouncer_dx = -GRID, 
    bouncer_dy = GRID;

  win = graphinit(RES_WIDTH, RES_HEIGHT);

  image1 = imagemArq("Bug_01.png", BOUNCER_SIZE, BOUNCER_SIZE,win);
  image2 = imagemArq("Bug_02.png", BOUNCER_SIZE, BOUNCER_SIZE, win);
  image = image1;

  bouncer_x = win.disp_data.width / 2.0 - BOUNCER_SIZE / 2.0;
  bouncer_y = win.disp_data.height / 2.0 - BOUNCER_SIZE / 2.0;

  /* Enquanto  a tecla ESC  não for  pressionada, irá executar
     o código especificado.
  */
  while (!sair) {
    ALLEGRO_EVENT ev;
    al_wait_for_event(win.event_queue, &ev);
    
    if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
      break;
    }
    else if(ev.type == ALLEGRO_EVENT_TIMER) {
      /* Para mover imagem com teclas de Direção */
      if(key[KEY_UP] && bouncer_y >= GRID)
	bouncer_y -= GRID;
 
      if(key[KEY_DOWN] && bouncer_y <= win.disp_data.height - BOUNCER_SIZE - GRID)
	bouncer_y += GRID;
 
      if(key[KEY_LEFT] && bouncer_x >= GRID)
	bouncer_x -= GRID;
 
      if(key[KEY_RIGHT] && bouncer_x <= win.disp_data.width - BOUNCER_SIZE - GRID)
	bouncer_x += GRID;
 
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
    else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
      switch(ev.keyboard.keycode) {
      case ALLEGRO_KEY_UP:    key[KEY_UP] = true;  break;
      case ALLEGRO_KEY_DOWN:  key[KEY_DOWN] = true; break;
      case ALLEGRO_KEY_LEFT:  key[KEY_LEFT] = true; break;
      case ALLEGRO_KEY_RIGHT: key[KEY_RIGHT] = true; break;
      case ALLEGRO_KEY_ENTER: image1 = image;
	image = image2;
	image2 = image1;
	break;
      }
    }
    else if(ev.type == ALLEGRO_EVENT_KEY_UP) {
      switch(ev.keyboard.keycode) {
      case ALLEGRO_KEY_UP:     key[KEY_UP] = false; break;
      case ALLEGRO_KEY_DOWN:   key[KEY_DOWN] = false; break;
      case ALLEGRO_KEY_LEFT:   key[KEY_LEFT] = false; break;
      case ALLEGRO_KEY_RIGHT:  key[KEY_RIGHT] = false; break;
      case ALLEGRO_KEY_ESCAPE: sair = true; break;
      }
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

