CFLAGS = -I include -L lib -l SDL2-2.0.0 -Werror -pedantic -Wall -Wextra -std=c18

learn_sdl:
	gcc -c src/rect.c -o src/rect.o
	gcc -c src/line.c -o src/line.o
	gcc -c src/player.c -o src/player.o
	gcc -c src/main.c -o src/main.o
	gcc src/rect.o src/line.o src/player.o src/main.o -o play $(CFLAGS)
