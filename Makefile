UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	INCLUDE =
	LIBS =
endif
ifeq ($(UNAME_S),Darwin)
	INCLUDE = -I/opt/homebrew/include
	LIBS = -L/opt/homebrew/lib
endif

CC = gcc
CFLAGS = $(INCLUDE) -D_THREAD_SAFE $(LIBS) -lSDL2 -lSDL2_ttf -Werror -pedantic -Wall -Wextra -std=c18
OBJFILES = src/rect.o src/line.o src/player.o src/camera.o src/main.o src/file.o src/render.o

.PHONY: all
all: play

play: $(OBJFILES)
	$(CC) $(OBJFILES) -o play $(CFLAGS)

src/.c.o:
	$(CC) $(INCLUDE) -c $<
# src/rect.o: src/rect.c
# 	$(CC) $(INCLUDE) -c $? -o $@
# src/line.o: src/line.c
# 	$(CC) $(INCLUDE) -c $? -o $@
# src/player.o: src/player.c
# 	$(CC) $(INCLUDE) -c $? -o $@
# src/camera.o: src/camera.c
# 	$(CC) $(INCLUDE) -c $? -o $@
# src/main.o: src/main.c
# 	$(CC) $(INCLUDE) -c $? -o $@
# src/file.o: src/file.c
# 	$(CC) $(INCLUDE) -c $? -o $@
# src/render.o: src/render.c
# 	$(CC) $(INCLUDE) -c $? -o $@

clean:
	rm -f src/*.o
	rm -f play
