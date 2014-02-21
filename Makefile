CC = gcc
CFLAGS = -Wall -Werror -g
INC = -I./allegro/usr/local/include
ALLEGRO = -lallegro
PRIMITIVES = -lallegro_primitives
FONT = -lallegro_font -lallegro_ttf
MATH = -lm
LIBS = -L./allegro/usr/local/lib $(ALLEGRO) $(PRIMITIVES) $(FONT) $(MATH)

OBJS = blasteroids.o spaceship.o blast.o asteroid.o
HEADERS = general.h

blasteroids: ./allegro $(OBJS) $(HEADERS)
	$(CC) $(CFLAGS) -o blasteroids $(OBJS) $(LIBS)
blasteroids.o: blasteroids.c
	$(CC) $(CFLAGS) $(INC) -c blasteroids.c $(LIBS)
spaceship.o: spaceship.c spaceship.h
	$(CC) $(CFLAGS) $(INC) -c spaceship.c $(LIBS)
blast.o: blast.c blast.h
	$(CC) $(CFLAGS) $(INC) -c blast.c $(LIBS)
asteroid.o: asteroid.c asteroid.h
	$(CC) $(CFLAGS) $(INC) -c asteroid.c $(LIBS)
./allegro-5.0.10:
	tar -xvzf allegro-5.0.10.tar.gz
./allegro: allegro-5.0.10
	(cd allegro-5.0.10; mkdir Build; cd ./Build; cmake ..; make; make install DESTDIR=../../allegro)
	export LD_LIBRARY_PATH=./allegro/usr/local/lib:$LD_LIBRARY_PATH
.PHONY: clean
clean:
	rm -rf *.o blasteroids
