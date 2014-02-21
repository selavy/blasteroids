#include <stdio.h>
#include "general.h"
#include "spaceship.h"
#include "blast.h"

enum MYKEYS {
  KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_SPACE
};

int main(int argc, char **argv) {
  ALLEGRO_DISPLAY *display = NULL;
  ALLEGRO_EVENT_QUEUE *event_queue = NULL;
  ALLEGRO_TIMER *timer = NULL;
  bool key[5] = {false,false,false,false,false};
  bool doexit = false;
  bool redraw = false;
  Spaceship * spaceship;

  if(!al_init()) {
    fputs("failed to initialize allegro!\n", stderr);
    exit(0);
  }

  if(!al_init_primitives_addon()) {
    fputs("failed to initialize allegro primitives!\n", stderr);
    exit(0);
  }

  if(!al_install_keyboard()) {
    fputs("failed to initialize the keyboard!\n", stderr);
    exit(0);
  }

  timer = al_create_timer(1.0 / FPS);
  if(!timer) {
    fputs("failed to create timer!\n", stderr);
    exit(0);
  }

  display = al_create_display(SCREEN_W, SCREEN_H);
  if(!display) {
    fputs("failed to create display!\n", stderr);
    al_destroy_timer(timer);
    exit(0);
  }

  event_queue = al_create_event_queue();
  if(!event_queue) {
    fputs("failed to create event queue!\n", stderr);
    al_destroy_display(display);
    al_destroy_timer(timer);
    exit(0);
  }

  spaceship = spaceship_create();

  /* al setup */
  al_register_event_source(event_queue, al_get_display_event_source(display));
  al_register_event_source(event_queue, al_get_timer_event_source(timer));
  al_register_event_source(event_queue, al_get_keyboard_event_source());
  al_clear_to_color(al_map_rgb(0,0,0));
  spaceship_draw(spaceship);
  al_flip_display();
  al_start_timer(timer);

  while(!doexit)
    {
      ALLEGRO_EVENT ev;
      al_wait_for_event(event_queue, &ev);
      
      if(ev.type == ALLEGRO_EVENT_TIMER) {
	if(key[KEY_UP]) {
	  spaceship_move(spaceship, KEY_UP);
	}
	if(key[KEY_DOWN]) {
	  spaceship_move(spaceship, KEY_DOWN);
	}
	if(key[KEY_LEFT]) {
	  spaceship_move(spaceship, KEY_LEFT);
	}
	if(key[KEY_RIGHT]) {
	  spaceship_move(spaceship, KEY_RIGHT);
	}
	if(key[KEY_SPACE]) {
	  blast_fire(spaceship->sx, spaceship->sy, spaceship->heading);
	}
	blast_move();
	redraw = true;
      }
      else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
	break;
      }
      else if((ev.type == ALLEGRO_EVENT_KEY_DOWN) ||
	      (ev.type == ALLEGRO_EVENT_KEY_UP)) {
	switch(ev.keyboard.keycode) {
	case ALLEGRO_KEY_UP:
	  key[KEY_UP] ^= 1;
	  break;
	case ALLEGRO_KEY_DOWN:
	  key[KEY_DOWN] ^= 1;
	  break;
	case ALLEGRO_KEY_LEFT:
	  key[KEY_LEFT] ^= 1;
	  break;
	case ALLEGRO_KEY_RIGHT:
	  key[KEY_RIGHT] ^= 1;
	  break;
	case ALLEGRO_KEY_SPACE:
	  key[KEY_SPACE] ^= 1;
	  break;
	default: break;
	}
      }

      if(redraw && al_is_event_queue_empty(event_queue)) {
	redraw = false;
	al_clear_to_color(al_map_rgb(0,0,0));
	spaceship_draw(spaceship);
	blast_draw();
	al_flip_display();
      }
    }

  spaceship_destroy(spaceship);
  blast_destroy();
  al_destroy_timer(timer);
  al_destroy_display(display);
  al_destroy_event_queue(event_queue);
  
  return 0;
}
