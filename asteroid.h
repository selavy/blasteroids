#ifndef _ASTEROID_H_
#define _ASTEROID_H_

#include <stdio.h>
#include "general.h"

typedef struct {
  float sx;
  float sy;
  float heading;
  float twist;
  float speed;
  float rot_velocity;
  float scale;
  int gone;
  ALLEGRO_COLOR color;
} Asteroid;

#define ROT_VELOCITY 5.0f
#define ASTEROID_SPEED 5.0f

void asteroid_add();
void asteroid_destroy();
void asteroid_move();
void asteroid_draw();

#endif
