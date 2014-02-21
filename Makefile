CC = gcc
CFLAGS = -Wall -Werror -g
ALLEGRO = -lallegro
PRIMITIVES = -lallegro_primitives
LIBS = $(ALLEGRO) $(PRIMITIVES)

OBJS = blasteroids.o spaceship.o

blaseroids: $(OBJS)
	$(CC) $(CFLAGS) -o blasteroids $(OBJS) $(LIBS)

%: %.c
	$(CC) $(CFLAGS) $^ $(LIBS)

.PHONY: clean
clean:
	rm -rf *.o blasteroids