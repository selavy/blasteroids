#include <stdio.h>
#include "general.h"
#include "spaceship.h"
#include "asteroid.h"
#include "blast.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#define MAX_STR_LEN 256
#define MAX_LIVES 3

enum MYKEYS {
  KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_SPACE
};

int score = 0;
Spaceship * ships_left[MAX_LIVES] = {NULL, NULL, NULL};
int lives = MAX_LIVES;
char score_str[MAX_STR_LEN];
bool gameover = false;

void score_draw(ALLEGRO_FONT * font);
void lives_init();
void lives_destroy();
void lives_draw();
void lose_life();
void draw_gameover_message(ALLEGRO_FONT * font);
void check_blast_hits();

int main(int argc, char **argv) {
  ALLEGRO_DISPLAY *display = NULL;
  ALLEGRO_EVENT_QUEUE *event_queue = NULL;
  ALLEGRO_TIMER *timer = NULL;
  ALLEGRO_FONT * font = NULL;
  bool key[5] = {false,false,false,false,false};
  bool doexit = false;
  bool redraw = false;
  bool immortal = false;
  int immortal_counter = 0;
  Spaceship * spaceship;

  if(!al_init()) {
    fputs("failed to initialize allegro!\n", stderr);
    exit(0);
  }

  if(!al_init_primitives_addon()) {
    fputs("failed to initialize allegro primitives!\n", stderr);
    exit(0);
  }

  al_init_font_addon();
  if(!al_init_ttf_addon()) {
    fputs("failed to initialize allegro ttf font addon!\n", stderr);
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

  font = al_load_ttf_font("Outlier.ttf",36,0);
  if(!font) {
    fputs("Unable to load font!\n", stderr);
    al_destroy_display(display);
    al_destroy_timer(timer);
    exit(0);
  }

  event_queue = al_create_event_queue();
  if(!event_queue) {
    fputs("failed to create event queue!\n", stderr);
    al_destroy_font(font);
    al_destroy_display(display);
    al_destroy_timer(timer);
    exit(0);
  }

  spaceship = spaceship_create();
  lives_init();
  al_register_event_source(event_queue, al_get_display_event_source(display));
  al_register_event_source(event_queue, al_get_timer_event_source(timer));
  al_register_event_source(event_queue, al_get_keyboard_event_source());
  al_clear_to_color(al_map_rgb(0,0,0));
  spaceship_draw(spaceship);
  score_draw(font);
  al_flip_display();
  al_start_timer(timer);

  while(!doexit)
    {
      ALLEGRO_EVENT ev;
      al_wait_for_event(event_queue, &ev);
      
      if(ev.type == ALLEGRO_EVENT_TIMER) {
	if(!gameover) {
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
	    score += 10;
	  }
	  
	  if(immortal) {
	    immortal_counter++;
	    if(immortal_counter >= FPS*5) {
	      immortal = false;
	    }
	  }
	  blast_move();
	  asteroid_move();
	}
	redraw = true;
      }
      else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
	break;
      }
      else if((ev.type == ALLEGRO_EVENT_KEY_DOWN) ||
	      (ev.type == ALLEGRO_EVENT_KEY_UP)) {
	asteroid_add();
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
	case ALLEGRO_KEY_ENTER:
	  if(gameover) doexit = true;
	  break;
	default: break;
	}
      }

      if(redraw && al_is_event_queue_empty(event_queue)) {
	redraw = false;
	al_clear_to_color(al_map_rgb(0,0,0));
	if(gameover) {
	  draw_gameover_message(font);
	  al_flip_display();
	}
	else { /* game not over */
	  if(!immortal) {
	    if(asteroid_check_collision(spaceship->sx, spaceship->sy) == HIT) {
	      lose_life();
	      immortal = true;
	      immortal_counter = 0;
	      spaceship_destroy(spaceship);
	      spaceship = spaceship_create();
	    }
	  }

	  check_blast_hits();
	  spaceship_draw(spaceship);
	  lives_draw();
	  blast_draw();
	  asteroid_draw();
	  score_draw(font);
	  al_flip_display();
	}
      }
    }

  lives_destroy();
  spaceship_destroy(spaceship);
  blast_destroy();
  asteroid_destroy();
  al_destroy_font(font);
  al_destroy_timer(timer);
  al_destroy_display(display);
  al_destroy_event_queue(event_queue);
  
  return 0;
}

void score_draw(ALLEGRO_FONT * font) {
  ALLEGRO_TRANSFORM transform, old;
  al_copy_transform(&old,al_get_current_transform());
  al_identity_transform(&transform);
  al_use_transform(&transform);
  sprintf(score_str,"Score: %i",score);
  al_draw_text(font, al_map_rgb(255,255,255), SCREEN_W / 6, 0, ALLEGRO_ALIGN_CENTRE,score_str);
  al_use_transform(&old);
}

void lives_init() {
  int i;
  for(i = 0; i < MAX_LIVES; ++i) {
    ships_left[i] = spaceship_create();
    if(!ships_left[i]) continue;
    ships_left[i]->sy = 50;
    ships_left[i]->sx = i * 30 + 20;
  }    
}

void lives_destroy() {
  int i;
  for(i = 0; i < MAX_LIVES; ++i) {
    spaceship_destroy(ships_left[i]);
  }
}

void lives_draw() {
  int i;
  for(i = 0; i < lives; ++i) {
    if(ships_left[i]) spaceship_draw(ships_left[i]);
  }
}

inline void lose_life() {
  if(lives) lives--;
  if(!lives) gameover = true;
}

void draw_gameover_message(ALLEGRO_FONT * font) {
  ALLEGRO_TRANSFORM transform, old;
  al_copy_transform(&old,al_get_current_transform());
  al_identity_transform(&transform);
  al_use_transform(&transform);
  al_draw_text(font, al_map_rgb(255,255,255), SCREEN_W / 2, SCREEN_H / 4, ALLEGRO_ALIGN_CENTRE, "Game Over");
  sprintf(score_str, "Score: %d", score);
  al_draw_text(font, al_map_rgb(255,255,255), SCREEN_W / 2, SCREEN_H / 2, ALLEGRO_ALIGN_CENTRE, score_str);
  al_use_transform(&old);
}

void check_blast_hits() {
  Blast * b = blast_get_first();
  while(b) {
    if(asteroid_check_collision(b->sx, b->sy) == HIT) {
      blast_remove_curr();
    }
    b = blast_next();
  }
}
