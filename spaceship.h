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

enum SHIP_MOVE {
  UP, DOWN, LEFT, RIGHT
};

#define TURN_SPEED 5.0f
#define MOVEMENT_SPEED 5.0f
#define MAX_DEG 180
#define SHIP_H 20
#define SHIP_W 16
#define SHIP_R 8

Spaceship * spaceship_create();
void spaceship_destroy(Spaceship * ship);
void spaceship_draw(Spaceship *s);
void spaceship_move(Spaceship *ship, enum SHIP_MOVE direction);

#endif
