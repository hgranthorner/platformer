INCLUDE = -I/opt/homebrew/include
LIBS = -L/opt/homebrew/lib
CFLAGS = $(INCLUDE) -D_THREAD_SAFE $(LIBS) -lSDL2 -lSDL2_ttf -Werror -pedantic -Wall -Wextra -std=c18
OBJFILES = src/rect.o src/line.o src/player.o src/camera.o src/main.o src/file.o src/render.o

.PHONY: all
all: play

play: $(OBJFILES)
	gcc $(OBJFILES) -o play $(CFLAGS)

src/rect.o: src/rect.c
	gcc $(INCLUDE) -c src/rect.c -o src/rect.o
src/line.o: src/line.c
	gcc $(INCLUDE) -c src/line.c -o src/line.o
src/player.o: src/player.c
	gcc $(INCLUDE) -c src/player.c -o src/player.o
src/camera.o: src/camera.c
	gcc $(INCLUDE) -c src/camera.c -o src/camera.o
src/main.o: src/main.c
	gcc $(INCLUDE) -c src/main.c -o src/main.o
src/file.o: src/file.c
	gcc $(INCLUDE) -c src/file.c -o src/file.o
src/render.o: src/render.c
	gcc $(INCLUDE) -c src/render.c -o src/render.o

clean:
	rm -f src/*.o
	rm -f play
