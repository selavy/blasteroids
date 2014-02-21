#include <stdio.h>
#include "general.h"
#include "spaceship.h"

int main(int argc, char **argv) {
  ALLEGRO_DISPLAY *display = NULL;
  ALLEGRO_EVENT_QUEUE *event_queue = NULL;
  ALLEGRO_TIMER *timer = NULL;
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
  }

  spaceship = spaceship_create();

  /* al setup */
  al_register_event_source(event_queue, al_get_display_event_source(display));
  al_register_event_source(event_queue, al_get_timer_event_source(timer));
  al_clear_to_color(al_map_rgb(0,0,0));
  spaceship_draw(spaceship);
  al_flip_display();
  al_start_timer(timer);

  while(!doexit)
    {
      ALLEGRO_EVENT ev;
      al_wait_for_event(event_queue, &ev);
      
      if(ev.type == ALLEGRO_EVENT_TIMER) {
	redraw = true;
      }
      else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
	break;
      }

      if(redraw && al_is_event_queue_empty(event_queue)) {
	redraw = false;
	al_clear_to_color(al_map_rgb(0,0,0));
	spaceship_draw(spaceship);
	al_flip_display();
      }
    }

  spaceship_destroy(spaceship);
  al_destroy_timer(timer);
  al_destroy_display(display);
  al_destroy_event_queue(event_queue);
  
  return 0;
}


