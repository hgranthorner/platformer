#!/bin/bash

gcc -I/opt/homebrew/include -Werror -pedantic -Wall -Wextra -std=c18 -L/opt/homebrew/lib -lSDL2 -lSDL2_ttf src/main.c -o play
