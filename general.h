#ifndef _GENERAL_H_
#define _GENERAL_H_

#include <stdlib.h>
#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#define SCREEN_W 640
#define SCREEN_H 480
#define FPS 40.0f
/* Converts from RADIANS -> DEGREES */
#define DEGREES(x) ((x)*(2*ALLEGRO_PI)/180.0f)
#define ALIVE 0
#define DEAD 1
#define HIT 1
#define MISS 0
#define MAX_DEG 180


#endif
