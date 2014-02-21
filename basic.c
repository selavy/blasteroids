#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>

int main(int argc, char **argv) {
  ALLEGRO_DISPLAY *display = NULL;
  if(!al_init()) {
    fputs("failed to initialize allegro!\n", stderr);
    exit(0);
  }

  display = al_create_display(640, 480);
  if(!display) {
    fputs("failed to create display!\n", stderr);
    exit(0);
  }

  al_clear_to_color(al_map_rgb(0, 0, 0));
  al_flip_display();
  al_rest(10.0);
  al_destroy_display(display);
  return 0;
}
    
