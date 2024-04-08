CC=gcc
CFLAGS= -Wall -Wpedantic -Wextra -ggdb -g  -lm

wallpapers: ./src/wallpapers.c
	$(CC) $(CFLAGS) -o ./build/wallpapers ./src/styles.c ./src/joplin.c ./src/wallpapers.c 