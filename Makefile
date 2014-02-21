CC = gcc
CFLAGS = -Wall -Werror -g
ALLEGRO = -lallegro
PRIMITIVES = -lallegro_primitives
LIBS = $(ALLEGRO) $(PRIMITIVES)

OBJS = blasteroids.o spaceship.o
HEADERS = general.h

blaseroids: $(OBJS) $(HEADERS)
	$(CC) $(CFLAGS) -o blasteroids $(OBJS) $(LIBS)

%: %.c %.h
	$(CC) $(CFLAGS) $^ $(LIBS)

.PHONY: clean
clean:
	rm -rf *.o blasteroids