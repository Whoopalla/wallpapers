#include "joplin.h"
#include "stb_image_init.c"


Color32 palette[PALETEE_COLORS_COUNT];
Color32 *image;
size_t image_width, image_height;
Seed seeds[SEED_COUNT];

int init_image_array(size_t width, size_t height) {
    image = (Color32 *)malloc(sizeof(Color32) * width * height);
    image_width = width;
    image_height = height;
    if (image == NULL) {
        fprintf(stderr, "Not enough memory to allocate image array.\n");
        return -1;
    }
    return 0;
}

bool points_equal(Point a, Point b) {
    if (a.x == b.x && a.y == b.y) {
        return true;
    } else {
        return false;
    }
}

int sqr_distance(int x1, int y1, int x2, int y2) {
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}

bool is_within_image(int x, int y) {
    return y >= 0 && y < (int)image_height && x >= 0 && x < (int)image_width;
}

void draw_if_within_image(int x, int y, Color32 color) {
    if (is_within_image(x, y)) {
        image[y * image_width + x] = color;
    }
}

Color32 create_random_collor(void) {
    const float golden_ratio_conjugate = 0.618033988749895;
    static float h;
    h = rand() % 1000 + 1;
    h /= 1000;
    float r, g, b;
    const float s = 0.5;
    const float v = 0.95;
    int h_i = h * 360 / 60;  // Convert to deegrees
    float f = h * 6 - h_i;
    float p = v * (1 - s);
    float q = v * (1 - f * s);
    float t = v * (1 - (1 - f) * s);

    if (h_i >= 0 && h_i < 1) r = v, g = t, b = p;
    if (h_i >= 1 && h_i < 2) r = q, g = v, b = p;
    if (h_i >= 2 && h_i < 3) r = p, g = v, b = t;
    if (h_i >= 3 && h_i < 4) r = p, g = q, b = v;
    if (h_i >= 4 && h_i < 5) r = t, g = p, b = v;
    if (h_i >= 5 && h_i < 6) r = v, g = p, b = q;
    r *= 256, g *= 256, b *= 256;
    uint8_t r_c = (uint8_t)r;
    uint8_t g_c = (uint8_t)g;
    uint8_t b_c = (uint8_t)b;

    uint32_t result = 0;
    result |= 0xFF000000;
    result |= (0xFF & r_c) << (8 * 0);
    result |= (0xFF & g_c) << (8 * 1);
    result |= (0xFF & b_c) << (8 * 2);
    h += golden_ratio_conjugate;
    h /= 10;
    return result;
}

void init_palette(void) {
    for (size_t i = 0; i < PALETEE_COLORS_COUNT; i++) {
        palette[i] = create_random_collor();
    }
}

static void plotline_low(Point a, Point b, Color32 color) {
    int dx = b.x - a.x;
    int dy = b.y - a.y;

    int yi = 1;
    if (dy < 0) {
        yi = -1;
        dy = -dy;
    }
    int d = 2 * dy - dx;
    int y = a.y;

    for (int x = a.x; x <= b.x; x++) {
        if (is_within_image(x, y)) {
            image[y * image_width + x] = color;
        }
        if (d > 0) {
            y += yi;
            d += 2 * (dy - dx);
        } else {
            d += 2 * dy;
        }
    }
}

static void plotline_high(Point a, Point b, Color32 color) {
    int dx = b.x - a.x;
    int dy = b.y - a.y;

    int xi = 1;
    if (dy < 0) {
        xi = -1;
        dx = -dx;
    }
    int d = 2 * dx - dy;
    int x = a.x;

    for (int y = a.y; y <= b.y; y++) {
        if (is_within_image(x, y)) {
            image[y * image_width + x] = color;
        }
        if (d > 0) {
            x += xi;
            d += 2 * (dx - dy);
        } else {
            d += 2 * dx;
        }
    }
}

void draw_line(Point a, Point b, Color32 color, size_t stroke) {
    if (a.x == b.x && a.y == b.y) return;
    for (size_t s = 0; s <= stroke; s++) {
        if (ABS(a.y - b.y) < ABS(a.x - b.x)) {
            if (s > 0) {
                draw_line((Point){.x = a.x, .y = a.y++}, (Point){.x = b.x, .y = b.y++}, color, 0);
            }
            if (a.x > b.x) {
                plotline_low(b, a, color);
            } else {
                plotline_low(a, b, color);
            }
        } else {
            if (s > 0) {
                draw_line((Point){.x = a.x++, .y = a.y}, (Point){.x = b.x++, .y = b.y}, color, 0);
            }
            if (a.y > b.y) {
                plotline_high(b, a, color);
            } else {
                plotline_high(a, b, color);
            }
        }
    }
}

void draw_circle(Point center, size_t r, Color32 color, size_t stroke, bool fill) {
    int ty = center.y - r;
    int by = center.y + r;
    int lx = center.x - r;
    int rx = center.x + r;

    for (int s = 0; s <= (int)stroke; s++) {
        for (int y = ty - s; y <= by + s; y++) {
            if (is_within_image(0, y)) {
                for (int x = lx - s; x <= rx + s; x++) {
                    if (fill) {
                        if ((size_t)sqr_distance(center.x, center.y, x, y) < ((r + s) * (r + s)) - s) {
                            draw_if_within_image(x, y, color);
                        }
                    } else {
                        if ((size_t)sqr_distance(center.x, center.y, x, y) - ((r + s) * (r + s)) < 2 * (r + s) + 1) {
                            draw_if_within_image(x, y, color);
                        }
                    }
                }
            }
        }
    }
}

void draw_rectangle(Point center, size_t width, size_t height, Color32 color, size_t stroke, bool fill) {
    int ty = center.y - height / 2 - height % 2 + 1;
    int by = center.y + height / 2;
    int lx = center.x - width / 2 - width % 2 + 1;
    int rx = center.x + width / 2;

    for (size_t s = 0; s <= stroke; s++) {
        for (int x = lx - (int)s; x <= rx + (int)s; x++) {
            draw_if_within_image(x, ty - s, color);
            draw_if_within_image(x, by + s, color);
        }

        for (int y = ty - (int)s; y <= by + (int)s; y++) {
            draw_if_within_image(lx - s, y, color);
            draw_if_within_image(rx + s, y, color);
        }
    }

    if (fill) {
        for (int y = ty; y <= by; y++) {
            for (int x = lx; x <= rx; x++) {
                draw_if_within_image(x, y, color);
            }
        }
    }
}

void draw_triangle(Point a, Point b, Point c, Color32 color, size_t stroke, bool fill) {
    draw_line(a, b, color, stroke);
    draw_line(b, c, color, stroke);
    draw_line(c, a, color, stroke);

    if (fill) {
        Point centroid = {.x = (a.x + b.x + c.x) / 3, .y = (a.y + b.y + c.y) / 3};
        for (int i = 0; i < 20; i++) {
            if (a.x > centroid.x) a.x--;
            if (a.x < centroid.x) a.x++;
            if (b.x > centroid.x) b.x--;
            if (b.x < centroid.x) b.x++;
            if (c.x > centroid.x) c.x--;
            if (c.x < centroid.x) c.x++;

            if (a.y > centroid.y) a.y--;
            if (a.y < centroid.y) a.y++;
            if (b.y > centroid.y) b.y--;
            if (b.y < centroid.y) b.y++;
            if (c.y > centroid.y) c.y--;
            if (c.y < centroid.y) c.y++;

            // draw_triangle(a, b, c, color, 0, false);
        }
    }
}

void fill_image(Color32 color) {
    for (int y = 0; y < (int)image_height; y++) {
        for (int x = 0; x < (int)image_width; x++) {
            image[y * image_width + x] = color;
        }
    }
}

void render_image(void) {
    printf("rendering...\n");
    if (stbi_write_png(OUTPUT_FILE, image_width, image_height, sizeof(Color32), image, image_width * sizeof(Color32)) == 0) {
        fprintf(stderr, "ERROR: couldn't create %s file\n", OUTPUT_FILE);
        exit(1);
    }
}

// int main(void) {
//     time_t start = time(NULL);
//     fill_image(BLUEISH_COLOR);
//     srand(time(NULL));
//
//     Point a = {.x = IMAGE_image_width / 2 - 100, .y = IMAGE_image_height / 2};
//     Point b = {.x = IMAGE_image_width / 2, .y = IMAGE_image_height / 2 - 100};
//     Point c = {.x = IMAGE_image_width / 2 + 125, .y = IMAGE_image_height / 2};
//
//     //draw_triangle(a, b, c, WHITE_COLOR, 0, true);
//
//     a.x -= 341;
//     a.y += 234;
//     c.y += 420;
//     c.x += 434;
//
//     draw_triangle(a, b, c, WHITE_COLOR, 0, true);
//
//     render_image(OUTPUT_FILE_PPM);
//     printf("Time spent: %llds\n", time(NULL) - start);
//     return 0;
// }