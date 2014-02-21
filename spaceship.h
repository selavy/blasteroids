#ifndef _SPACESHIP_H_
#define _SPACESHIP_H_

#include "general.h"

typedef struct {
  /* location */
  float sx;
  float sy;
  /* direction heading in degrees */
  float heading;
  float speed;
  /* 1 = dead, 0 = alive */
  int gone;
  ALLEGRO_COLOR color;
} Spaceship;

Spaceship * spaceship_create();
void spaceship_draw(Spaceship *s);
void spaceship_destroy(Spaceship * ship);

#endif
