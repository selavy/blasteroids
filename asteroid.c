#include "asteroid.h"

typedef struct _asteroid_list {
  struct _asteroid_list * next;
  struct _asteroid_list * prev;
  Asteroid * asteroid;
} alist;

static alist * list = NULL;
static void add(Asteroid * asteroid);

static int rand_init = 0;
static void destroy(Asteroid * asteroid);
static void draw(Asteroid * asteroid);
static int move(Asteroid * asteroid);
static Asteroid * asteroid_create();

static void add(Asteroid * asteroid) {
  if(!asteroid) return;
  if( list == NULL ) {
    list = malloc(sizeof(*list));
    if(!list) {
      fputs("Unable to allocate memory for list!\n", stderr);
      return;
    }
    list->asteroid = asteroid;
    list->next = NULL;
    list->prev = NULL;
  }
  else {
    alist * n = malloc(sizeof(*n));
    if(!n) {
      fputs("Unable to allocate memory for list!\n", stderr);
      return;
    }
    n->asteroid = asteroid;
    n->prev = NULL;
    n->next = list;
    list->prev = n;
    list = n;
  }
}

static void destroy(Asteroid * asteroid) {
  if(asteroid) free(asteroid);
}

static void draw(Asteroid * asteroid) {
  ALLEGRO_TRANSFORM transform;
  al_identity_transform(&transform);
  al_rotate_transform(&transform, DEGREES(asteroid->twist));
  al_scale_transform(&transform, asteroid->scale, asteroid->scale);
  al_translate_transform(&transform, asteroid->sx, asteroid->sy);
  al_use_transform(&transform);
  al_draw_line(-20, 20 , -25, 5  , asteroid->color, 2.0f);
  al_draw_line(-25, 5  , -25, -10, asteroid->color, 2.0f);
  al_draw_line(-25, -10, -5 , -10, asteroid->color, 2.0f);
  al_draw_line(-5 , -10, -10, -20, asteroid->color, 2.0f);
  al_draw_line(-10, -20, 5  , -20, asteroid->color, 2.0f);
  al_draw_line(5  , -20, 20 , -10, asteroid->color, 2.0f);
  al_draw_line(20 , -10, 20 , -5 , asteroid->color, 2.0f);
  al_draw_line(20 , -5 , 0  , 0  , asteroid->color, 2.0f);
  al_draw_line(0  , 0  , 20 , 10 , asteroid->color, 2.0f);
  al_draw_line(20 , 10 , 10 , 20 , asteroid->color, 2.0f);
  al_draw_line(10 , 20 , 0  , 15 , asteroid->color, 2.0f);
  al_draw_line(0  , 15 , -20, 20 , asteroid->color, 2.0f);
}

static int move(Asteroid * asteroid) {
  const float theta = DEGREES(asteroid->heading);
  asteroid->sx += ASTEROID_SPEED * sin(theta);
  asteroid->sy += ASTEROID_SPEED * cos(theta);
  asteroid->twist += asteroid->rot_velocity;
  if((asteroid->sx > SCREEN_W) || (asteroid->sx < 0)) return 0;
  else if((asteroid->sy > SCREEN_H) || (asteroid->sy < 0)) return 0;
  else return 1;
}

static Asteroid * asteroid_create() {
  Asteroid * asteroid = malloc(sizeof(*asteroid));
  const int r = rand();
  const int choice = r % 4;
  
  if(!asteroid) {
    fputs("Unable to create new asteroid!\n", stderr);
    return NULL;
  }

  if(!rand_init) {
    rand_init = 1;
    srand(time(NULL));
  }

  switch( choice ) {
  case 0:
    asteroid->sx = r % SCREEN_W;
    asteroid->sy = 0;
    break;
  case 1:
    asteroid->sx = r % SCREEN_W;
    asteroid->sy = SCREEN_H;
    break;
  case 2:
    asteroid->sx = 0;
    asteroid->sy = r % SCREEN_H;
    break;
  case 3:
    asteroid->sx = SCREEN_W;
    asteroid->sy = r % SCREEN_H;
    break;
  }

  asteroid->heading = rand() % MAX_DEG;
  asteroid->twist = 0.0f;
  asteroid->rot_velocity = ROT_VELOCITY;
  asteroid->scale = 1.0f;
  asteroid->gone = ALIVE;
  asteroid->color = al_map_rgb(255, 255, 255);
  return asteroid;
}

void asteroid_add() {
  Asteroid * asteroid = asteroid_create();
  add(asteroid);
}

void asteroid_destroy() {
  alist * n = list;
  alist * t;
  while(n) {
    destroy(n->asteroid);
    t = n->next;
    free(n);
    n = t;
  }
}

void asteroid_move() {
  alist * n = list;
  while(n) {
    if(!move(n->asteroid)) {
      alist * tmp = n->next;
      if(list == n) list = n->next;
      destroy(n->asteroid);
      if(n->prev) n->prev->next = n->next;
      if(n->next) n->next->prev = n->prev;
      free(n);
      n = tmp;
    }
    else n = n->next;
  }
}

void asteroid_draw() {
  alist * n = list;
  while(n) {
    draw(n->asteroid);
    n = n->next;
  }
}

int asteroid_check_collision(float sx, float sy) {
  alist * n = list;
  while(n) {
    if((abs(n->asteroid->sx - sx) < 30.0f) && (abs(n->asteroid->sy - sy) < 30.0f)) {
      /* break the asteroid into 2 pieces */
      if( n->asteroid->scale != 0.5f ) {
	Asteroid *first, *second;
	first = asteroid_create();
	second = asteroid_create();
	if(first) {
	  first->sx = n->asteroid->sx;
	  first->sy = n->asteroid->sy;
	  first->twist = n->asteroid->twist;
	  first->heading = n->asteroid->heading + 30;
	  first->scale = n->asteroid->scale * 0.5;
	  add(first);
	}
	if(second) {
	  second->sx = n->asteroid->sx;
	  second->sy = n->asteroid->sy;
	  second->twist = n->asteroid->twist;
	  second->heading = n->asteroid->heading - 30;
	  second->scale = n->asteroid->scale * 0.5;
	  add(second);
	}
      }

      /* remove the current asteroid */
      alist * tmp = n->next;
      if(list == n) list = tmp;
      destroy(n->asteroid);
      if(n->prev) n->prev->next = n->next;
      if(n->next) n->next->prev = n->prev;
      free(n);
      n = tmp;
      return HIT;
    }
    n = n->next;
  }
  return MISS;
}
