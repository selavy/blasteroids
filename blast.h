#ifndef _BLAST_H_
#define _BLAST_H_

#include <stdio.h>
#include "general.h"

typedef struct {
  float sx;
  float sy;
  float heading;
  float speed;
  int gone;
  ALLEGRO_COLOR color;
} Blast;

#define BLAST_SPEED 10.0f


void blast_fire(float sx, float sy, float heading);
void blast_destroy();
void blast_move();
void blast_draw();
inline Blast * blast_get_first();
inline Blast * blast_next();
void blast_remove_curr();

#endif
