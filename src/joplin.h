#ifndef JOPLIN_H
#define JOPLIN_H

#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>
#include <unistd.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define ABS(x) ((x) > 0) ? (x) : ((~x) + 1)

#define RED_COLOR 0xFF0000FF
#define BLUEISH_COLOR 0xFF625b04
#define BLUE_COLOR 0xFFFF0000
#define WHITE_COLOR 0xFFFFFFFF
#define PALETEE_COLORS_COUNT 9

#define OUTPUT_FILE "output.png"

#define SEED_COUNT 600
#define SEED_SIZE 2
#define SEED_COLOR WHITE_COLOR
#define PALETEE_COLORS_COUNT 9

typedef uint32_t Color32;

typedef struct {
    int x, y;
    Color32 color;
} Seed;

typedef struct Point {
    int x, y;
} Point;

int sqr_distance(int x1, int y1, int x2, int y2);
Color32 create_random_collor(void);
void init_palette(void);
void render_image(void);
int init_image_array(size_t width, size_t height);
void draw_line(Point a, Point b, Color32 color, size_t stroke);
void draw_circle(Point center, size_t r, Color32 color, size_t stroke, bool fill);
void draw_rectangle(Point center, size_t width, size_t height, Color32 color, size_t stroke, bool fill);
void draw_rectangle(Point center, size_t width, size_t height, Color32 color, size_t stroke, bool fill);
void fill_image(Color32 color);
#endif