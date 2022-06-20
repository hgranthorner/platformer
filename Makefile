UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	INCLUDE = -Werror -pedantic -Wall -Wextra -std=c18
	LIBS =
endif
ifeq ($(UNAME_S),Darwin)
	INCLUDE = -I/opt/homebrew/include -Werror -pedantic -Wall -Wextra -std=c18
	LIBS = -L/opt/homebrew/lib
endif

CC = gcc
CFLAGS = $(INCLUDE) -D_THREAD_SAFE $(LIBS) -lSDL2 -lSDL2_ttf
OBJFILES = src/rect.o src/line.o src/player.o src/camera.o src/main.o src/file.o src/render.o src/editor.o

.PHONY: all
all: play

play: $(OBJFILES)
	$(CC) $(OBJFILES) -o play $(CFLAGS)

src/rect.o: src/rect.c src/rect.h
	$(CC) $(INCLUDE) -c $< -o $@

src/line.o: src/line.c src/line.h
	$(CC) $(INCLUDE) -c $< -o $@

src/player.o: src/player.c src/player.h src/line.o src/rect.o src/consts.h
	$(CC) $(INCLUDE) -c $< -o $@

src/camera.o: src/camera.c src/camera.h src/player.o src/consts.h
	$(CC) $(INCLUDE) -c $< -o $@

src/file.o: src/file.c src/file.h src/rect.o src/player.o src/consts.h
	$(CC) $(INCLUDE) -c $< -o $@

src/render.o: src/render.c src/render.h src/consts.h src/rect.o src/player.o src/camera.o src/file.o
	$(CC) $(INCLUDE) -c $< -o $@

src/editor.o: src/editor.c src/editor.h src/consts.h src/file.o
	$(CC) $(INCLUDE) -c $< -o $@

src/main.o: src/main.c src/render.o src/editor.o
	$(CC) $(INCLUDE) -c $< -o $@

clean:
	rm -f src/*.o
	rm -f play
