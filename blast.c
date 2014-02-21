#include "blast.h"

/*************************************/
/*       Blast linked list           */
/*************************************/
typedef struct _blast_list {
  struct _blast_list * next;
  struct _blast_list * prev;
  Blast * blast;
} blist;

static blist * list = NULL;
static blist * curr = NULL;
static void add( Blast * blast );
/************************************/

static Blast * blast_create(float sx, float sy, float heading);
static void destroy(Blast * blast);
static int move(Blast * blast);
static void draw(Blast * blast);

static Blast * blast_create(float sx, float sy, float heading) {
  Blast * blast = malloc(sizeof(*blast));
  if(!blast) return NULL;

  blast->sx = sx;
  blast->sy = sy;
  blast->heading = heading;
  blast->speed = BLAST_SPEED;
  blast->gone = ALIVE;
  blast->color = al_map_rgb(255,255,0);
  return blast;
}

static void destroy(Blast * blast) {
  if(blast) free(blast);
}

static int move(Blast * blast) {
  const float theta = DEGREES(blast->heading);
  blast->sx += BLAST_SPEED * sin(theta);
  blast->sy -= BLAST_SPEED * cos(theta);

  if((blast->sx > SCREEN_W) || (blast->sx < 0)) return 0;
  else if((blast->sy > SCREEN_H) || (blast->sy < 0)) return 0;
  else return 1;
}

static void draw(Blast * blast) {
  ALLEGRO_TRANSFORM transform;
  al_identity_transform(&transform);
  al_rotate_transform(&transform, DEGREES(blast->heading));
  al_translate_transform(&transform, blast->sx, blast->sy);
  al_use_transform(&transform);
  al_draw_line(0, 0, 0, 2, blast->color, 2.0f);
}

static void add( Blast * blast ) {
  if(!blast) return;
  if(list == NULL) {
    list = malloc(sizeof(*list));
    if(!list) {
      fputs("Unable to allocate memory for list!\n", stderr);
      return;
    }
    list->blast = blast;
    list->next = NULL;
    list->prev = NULL;
  }
  else {
    blist * n = malloc(sizeof(*n));
    if(!n) {
      fputs("Unable to allocate memory for list!\n", stderr);
      return;
    }
    n->blast = blast;
    n->prev = NULL;
    n->next = list;
    list->prev = n;
    list = n;
  }
}

void blast_fire(float sx, float sy, float heading) {
  Blast * blast = blast_create( sx, sy, heading );
  add(blast);
}

void blast_destroy() {
  blist * n = list;
  blist * t;
  while(n) {
    destroy(n->blast);
    t = n->next;
    free(n);
    n = t;
  }
}

void blast_move() {
  blist * n = list;
  while(n) {
    if(!move(n->blast)) {
      blist * tmp = n->next;
      /* if n is the head of the list, reset list to n->next */
      if(list == n) list = n->next;
      destroy(n->blast);
      if(n->prev) n->prev->next = n->next;
      if(n->next) n->next->prev = n->prev;
      free(n);
      n = tmp;
    }
    else n = n->next;
  }
}

void blast_draw() {
  blist * n = list;
  while(n != NULL) {
    draw(n->blast);
    n = n->next;
  }
}

inline Blast * blast_get_first() {
  curr = list;
  return (curr) ? curr->blast : NULL;
}

inline Blast * blast_next() {
  curr = (curr) ? curr->next : NULL;
  return (curr) ? curr->blast : NULL;
}

void blast_remove_curr() {
  blist * tmp = (curr) ? curr->next : NULL;
  if(!curr) return;
  if(list == curr) list = tmp;
  destroy(curr->blast);
  if(curr->prev) curr->prev->next = curr->next;
  if(curr->next) curr->next->prev = curr->prev;
  free(curr);
  curr = tmp;
}
