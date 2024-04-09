#include "joplin.h"
#include "styles.h"

#define MAX_WALLPAPER_WIDTH 4096
#define MAX_WALLPAPER_HEIGHT 2160
#define DEFAULT_HEIGHT 1080
#define DEFAULT_WIDTH 1920

typedef int paperwall_style;
enum paperwall_style {
    Voronoi,
    Spiral_Circles,
    Concentric_Rectangles,
    Perpendecular_Lines,
    _StyleEnumEnd
};

const char *height_param = "--height";
const char *width_param = "--width";
const char *style_param = "--style";

const char *usage = "Usage: ./wallpapers --height --width --style";

bool use_random_style;

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
void set_wallpaper(void) {
    char cwd[MAX_PATH];
    if (GetCurrentDirectory(sizeof(cwd), cwd) == 0) {
        fprintf(stderr, "ERROR: couldn't get currnet working directory.");
        exit(1);
    }
    // for (size_t i = 0; i < strlen(cwd); i++) {
    //     if (cwd[i] == '\\') cwd[i] = '/';
    // }
    printf("cwd: %s\n", cwd);
    char path[MAX_PATH];
    sprintf(path, "%s/output.png", cwd);
    if (SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, path, SPIF_UPDATEINIFILE) == 0) {
        fprintf(stderr, "ERROR: couldn't update system parameters.");
        exit(1);
    }
}
#endif

int main(int argc, char **argv) {
    srand(time(NULL));
    size_t height, width;
    paperwall_style style;
    size_t height_param_l = strlen(height_param);
    size_t width_param_l = strlen(width_param);
    size_t style_param_l = strlen(style_param);
    char *p, *end_p;
    if (argc > 4) {
        fprintf(stdout, usage);
        return -1;
    }
    height = DEFAULT_HEIGHT;
    width = DEFAULT_WIDTH;
    style = rand() % _StyleEnumEnd;

    for (size_t i = 0; (int)i < argc; i++) {
        if (!strncmp(argv[i], height_param, height_param_l)) {
            p = argv[i] + height_param_l + 1;
            height = strtol(p, &end_p, 10);
            if (*end_p != '\0' || height <= 0 || height > MAX_WALLPAPER_HEIGHT) {
                fprintf(stderr, "Incorrect height value.\t");
                if (height <= 0)
                    fprintf(stderr, "Min value is 1\n");
                else if (height > MAX_WALLPAPER_HEIGHT)
                    fprintf(stderr, "Max height value: %d\n", MAX_WALLPAPER_HEIGHT);
                return -1;
            }
        } else if (!strncmp(argv[i], width_param, width_param_l)) {
            p = argv[i] + width_param_l + 1;
            width = strtol(p, &end_p, 10);
            if (*end_p != '\0' || width <= 0 || width <= 0 || width > MAX_WALLPAPER_WIDTH) {
                fprintf(stderr, "Incorrect widht value.\t");
                if (width <= 0)
                    fprintf(stderr, "Min value is 1\n");
                else if (width > MAX_WALLPAPER_HEIGHT)
                    fprintf(stderr, "Max widht value: %d\n", MAX_WALLPAPER_WIDTH);
                return -1;
            }
        } else if (!strncmp(argv[i], style_param, style_param_l)) {
            p = argv[i] + style_param_l + 1;
            style = strtol(p, &end_p, 10);
            if (*end_p != '\0') {
                fprintf(stderr, "Incorrect style value\n");
                return -1;
            }
            if (style < 0 || style >= _StyleEnumEnd) {
                fprintf(stderr, "Max style value: %d\n", _StyleEnumEnd - 1);  // TODO: Print out possible style values
                return -1;
            }
        }
    }

    printf("w: %lld, h: %lld, style: %d\n", width, height, style);

    int res = init_image_array(width, height);
    if (res == -1) return -1;
    switch (style) {
        case Voronoi:
            printf("Creating in voronoi style...\n");
            draw_voronoi();
            break;
        case Spiral_Circles:
            printf("Creating in Spiral Circles style...\n");
            fflush(stdout);
            draw_spiral_circles();
            break;
        case Concentric_Rectangles:
            printf("Creating in Concentric Rectangles style...\n");
            fflush(stdout);
            draw_concentric_rects();
            break;
        case Perpendecular_Lines:
            printf("Creating in Perpendecular Lines style...\n");
            fflush(stdout);
            draw_perpendecular_lines();
            break;
   }
    #if defined(_WIN32) || defined(_WIN64)
    set_wallpaper();
    #endif
}