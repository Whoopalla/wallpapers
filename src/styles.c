#include "joplin.h"
#include "styles.h"

extern Seed seeds[SEED_COUNT];
extern Color32 palette[PALETEE_COLORS_COUNT];
extern Color32 *image;
extern size_t image_width, image_height;

void draw_spiral_circles(void) {
    srand(time(NULL));
    init_palette();
    fill_image(create_random_collor());

    Point center = {.x = image_width / 2, .y = image_height / 2};
    double a = 1.514;
    double b = 1.823;
    int x = image_width / 2, y = image_height / 2;

    for (int i = 0; i <= 1000 * M_PI; i++) {
        center.x = x + (a + b * i) * cos(i);
        center.y = y + (a + b * i) * sin(i);
        draw_circle(center, 30 + i / 100, palette[rand() % (sizeof(palette) / sizeof(Color32))], 1, true);
    }

    render_image();
    free(image);
}


void draw_concentric_rects(void) {
    srand(time(NULL));
    init_palette();
    fill_image(BLUEISH_COLOR);

    Point center = {.x = 0, .y = 0};
    Color32 color;
    for (int x = 240; x < (int)image_width; x += 360) {
        for (int y = 180; y < (int)image_height; y += 360) {
            center.x = x;
            center.y = y;
            color = palette[rand() % (sizeof(palette) / sizeof(Color32))];
            for (int j = 1; j < 20; j++) {
                draw_rectangle(center, j * 15, j * 15, color, 6, false);
            }
        }
    }

    render_image();
    free(image);
}

void draw_perpendecular_lines(void) {
    srand(time(NULL));
    init_palette();
    fill_image(BLUEISH_COLOR);

    Point a = {.x = image_width / 2, .y = image_height / 2};
    Point b = {.x = 0, .y = image_height / 2};
    int x = 0, y = 0, step = 60, stroke = 30;
    while (x < (int)image_width * 4) {
        a.x = 0;
        a.y = y;
        b.x = x;
        b.y = 0;
        draw_line(a, b, palette[rand() % sizeof(palette) / sizeof(Color32)], stroke);

        x += step;
        y += step;
    }

    x = 0;
    y = image_height;
    while (x < (int)image_width * 4) {
        a.x = 0;
        a.y = y;
        b.x = x;
        b.y = image_height;
        draw_line(a, b, palette[rand() % sizeof(palette) / sizeof(Color32)], stroke);

        x += step;
        y -= step;
    }

    render_image();
    free(image);
}

static void create_seeds(void) {
    srand(time(NULL));
    init_palette();
    int x, y;
    Color32 color;
    for (int i = 0; i < SEED_COUNT; i++) {
        x = rand() % image_width;
        y = rand() % image_height;
        color = palette[rand() % (sizeof(palette) / sizeof(Color32))];
        seeds[i].x = x;
        seeds[i].y = y;
        seeds[i].color = (color);
    }
}

static void draw_seeds(void) {
    for (int i = 0; i < SEED_COUNT; i++) {
        int left = MAX(seeds[i].x - SEED_SIZE, 0);
        int right = seeds[i].x + SEED_SIZE;
        int top = MAX(seeds[i].y - SEED_SIZE, 0);
        int bottom = seeds[i].y + SEED_SIZE;

        for (int y = top; y <= bottom; y++) {
            for (int x = left; x <= right; x++) {
                if (sqr_distance(seeds[i].x, seeds[i].y, x, y) <= SEED_SIZE * SEED_SIZE) {
                    image[y * image_width + x] = (Color32)SEED_COLOR;
                }
            }
        }
    }
}

void draw_voronoi(void) {
    srand(time(NULL));
    init_palette();
    create_seeds();
    Seed *closest = &seeds[0];
    for (int y = 0; y < (int)image_height; y++) {
        for (int x = 0; x < (int)image_width; x++) {
            for (size_t s = 0; s < SEED_COUNT; s++) {
                if (sqr_distance(x, y, seeds[s].x, seeds[s].y) < sqr_distance(x, y, closest->x, closest->y)) {
                    closest = &seeds[s];
                }
            }
            image[y * image_width + x] = closest->color;
        }
    }
    render_image();
    free(image);
}