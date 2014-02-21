#include "spaceship.h"
/* #include <stdio.h> */

Spaceship * spaceship_create() {
    /* create space ship */
  Spaceship * spaceship = malloc(sizeof(*spaceship));
  if(!spaceship) return NULL;
  spaceship->sx = SCREEN_W / 2.0f;
  spaceship->sy = SCREEN_H / 2.0f;
  spaceship->heading = 0;
  spaceship->speed = 0;
  spaceship->gone = 0;
  spaceship->color = al_map_rgb(255, 255, 255);
  return spaceship;
}

void spaceship_destroy(Spaceship *ship) {
  if(ship) free(ship);
}

void spaceship_draw(Spaceship *s) {
  ALLEGRO_TRANSFORM transform;
  al_identity_transform(&transform);
  al_rotate_transform(&transform, DEGREES(s->heading));
  al_translate_transform(&transform, s->sx, s->sy);
  al_use_transform(&transform);
  al_draw_line(-8, 9, 0, -11, s->color, 3.0f);
  al_draw_line(0, -11, 8, 9, s->color, 3.0f);
  al_draw_line(-6, 4, -1, 4, s->color, 3.0f);
  al_draw_line(6, 4, 1, 4, s->color, 3.0f);
}


void spaceship_move(Spaceship * ship, enum SHIP_MOVE heading) {
  if(!ship) return;

  if(heading == UP) {
    float theta = DEGREES(ship->heading);
    float delta_x = MOVEMENT_SPEED * sin(theta);
    float delta_y = -1 * MOVEMENT_SPEED * cos(theta);

    if( delta_x <= 0 ) {
      ship->sx = (ship->sx >= -1 * delta_x) ? ship->sx + delta_x : ship->sx;
    }
    else {
      ship->sx = (ship->sx <= SCREEN_W - SHIP_R - delta_x) ? ship->sx + delta_x : ship->sx;
    }
    if( delta_y <= 0 ) {
      ship->sy = (ship->sy >= -1 * delta_y) ? ship->sy + delta_y : ship->sy;
    }
    else {
      ship->sy = (ship->sy <= SCREEN_H - SHIP_R - delta_y) ? ship->sy + delta_y : ship->sy;
    }
  }
  /* CANNOT MOVE BACKWARDS */
  /* else if(heading == DOWN) {
    float theta = DEGREES(ship->heading);
    ship->sx -= MOVEMENT_SPEED * sin(theta);
    ship->sy -= MOVEMENT_SPEED * cos(theta);
    } */ 
  else if(heading == LEFT) {
    ship->heading = ship->heading - TURN_SPEED;
    ship->heading = (ship->heading >= 0) ? ship->heading : MAX_DEG + ship->heading;
  } else if(heading == RIGHT) {
    ship->heading = ship->heading + TURN_SPEED;
    ship->heading = (ship->heading <= MAX_DEG) ? ship->heading : ship->heading - MAX_DEG;
  }

  /* printf("heading = %f => %f degrees\n", ship->heading, DEGREES(ship->heading)); */
}


