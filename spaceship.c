#include "spaceship.h"

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

void spaceship_destroy(Spaceship *ship) {
  free(ship);
}
