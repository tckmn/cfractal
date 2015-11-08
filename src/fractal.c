#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

typedef struct {
    float x;
    float y;
} Point;

typedef struct {
    float r;
    float g;
    float b;
} Color;

Color hue2rgb(float hue) {
    if (hue < 1.0/3) {
        return (Color) {hue*3,           1-hue*3,       0};
    } else if (hue < 2.0/3) {
        return (Color) {0,               (hue-1.0/3)*3, 1-(hue-1.0/3)*3};
    } else {
        return (Color) {1-(hue-2.0/3)*3, 0,             (hue-2.0/3)*3};
    }
}

typedef void (*TransformFunc)(Point *p, float aff[6]);

void affine(Point *p, float aff[6]) {
    p->x = aff[0]*p->x + aff[1]*p->y + aff[4];
    p->y = aff[2]*p->x + aff[3]*p->y + aff[5];
}

float affines[][6] = {
    {0,0,0,0,0,0},
    {0,0,0,0,0,0},
    {0,0,0,0,0,0},
    {0,0,0,0,0,0}
};

void v0(Point *p, float aff[6]) {
}

void v1(Point *p, float aff[6]) {
    p->x = sin(p->x);
    p->y = sin(p->y);
}

void v2(Point *p, float aff[6]) {
    float r = 1 / (p->x*p->x + p->y*p->y);
    p->x = r*p->x;
    p->y = r*p->y;
}

void v3(Point *p, float aff[6]) {
    float r = p->x*p->x + p->y*p->y;
    p->x = sin(r)*p->x - cos(r)*p->y;
    p->y = sin(r)*p->x + cos(r)*p->y;
}

void v4(Point *p, float aff[6]) {
    float r = 1 / sqrt(p->x*p->x + p->y*p->y);
    p->x = r*(p->x-p->y)*(p->x+p->y);
    p->y = 2*p->x*p->y;
}

void v5(Point *p, float aff[6]) {
    p->x = atan(p->x / p->y) / M_PI;
    p->y = sqrt(p->x*p->x + p->y*p->y) - 1;
}

void v6(Point *p, float aff[6]) {
    float r = sqrt(p->x*p->x + p->y*p->y),
          theta = atan(p->x / p->y);
    p->x = r * sin(theta + r);
    p->y = r * cos(theta - r);
}

void v7(Point *p, float aff[6]) {
    float r = sqrt(p->x*p->x + p->y*p->y),
          theta = atan(p->x / p->y);
    p->x = r * sin(theta * r);
    p->y = r * -cos(theta * r);
}

void v8(Point *p, float aff[6]) {
    float r = sqrt(p->x*p->x + p->y*p->y),
          theta = atan(p->x / p->y);
    p->x = theta / M_PI * sin(M_PI * r);
    p->y = theta / M_PI * cos(M_PI * r);
}

void v9(Point *p, float aff[6]) {
    float r = sqrt(p->x*p->x + p->y*p->y),
          theta = atan(p->x / p->y);
    p->x = (1 / r) * (cos(theta) + sin(r));
    p->y = (1 / r) * (sin(theta) - cos(r));
}

void v10(Point *p, float aff[6]) {
    float r = sqrt(p->x*p->x + p->y*p->y),
          theta = atan(p->x / p->y);
    p->x = sin(theta) / r;
    p->y = r * cos(theta);
}

void v11(Point *p, float aff[6]) {
    float r = sqrt(p->x*p->x + p->y*p->y),
          theta = atan(p->x / p->y);
    p->x = sin(theta) * cos(r);
    p->y = cos(theta) * sin(r);
}

void v12(Point *p, float aff[6]) {
    float r = sqrt(p->x*p->x + p->y*p->y),
          theta = atan(p->x / p->y),
          p0 = sin(theta + r),
          p1 = cos(theta - r);
    p->x = r * (pow(p0, 3) + pow(p1, 3));
    p->y = r * (pow(p0, 3) - pow(p1, 3));
}

void v13(Point *p, float aff[6]) {
    float r = sqrt(p->x*p->x + p->y*p->y),
          theta = atan(p->x / p->y),
          omega = (rand() % 2 == 0) ? 0 : M_PI;
    p->x = sqrt(r) * cos(theta / 2 + omega);
    p->y = sqrt(r) * sin(theta / 2 + omega);
}

void v14(Point *p, float aff[6]) {
    if (p->x >= 0 && p->y >= 0) {
        // NOP
    } else if (p->x < 0 && p->y >= 0) {
        p->x = 2*p->x;
    } else if (p->x >= 0 && p->y < 0) {
        p->x = p->y/2;
    } else {
        p->x = 2*p->x;
        p->x = p->y/2;
    }
}

void v15(Point *p, float aff[6]) {
    p->x = p->x + aff[1] * sin(p->y / (aff[2]*aff[2]));
    p->y = p->y + aff[4] * sin(p->x / (aff[5]*aff[5]));
}

void v16(Point *p, float aff[6]) {
    float r = 2 / (sqrt(p->x*p->x + p->y*p->y) + 1);
    p->x = r*p->y;
    p->y = r*p->x;
}

void v17(Point *p, float aff[6]) {
    p->x = p->x + aff[2]*sin(tan(3*p->y));
    p->y = p->y + aff[5]*sin(tan(3*p->x));
}

// NOTE: this one is bad
void v18(Point *p, float aff[6]) {
    p->x = exp(p->x - 1) * cos(M_PI * p->y);
    p->y = exp(p->x - 1) * sin(M_PI * p->y);
}

void v19(Point *p, float aff[6]) {
    float r = sqrt(p->x*p->x + p->y*p->y),
          theta = atan(p->x / p->y);
    p->x = pow(r, sin(theta)) * cos(theta);
    p->y = pow(r, sin(theta)) * sin(theta);
}

// NOTE: this one is also bad
void v20(Point *p, float aff[6]) {
    p->x = cos(M_PI * p->x) * cosh(p->y);
    p->y = -sin(M_PI * p->x) * sinh(p->y);
}

void render(int w, int h, int nFuncs, TransformFunc *funcs, int num) {
    Color **hist = malloc(w * sizeof(Color*));
    for (int x = 0; x < w; ++x) {
        hist[x] = malloc(h * sizeof(Color));
        for (int y = 0; y < h; ++y) {
            hist[x][y] = (Color) {0.0, 0.0, 0.0};
        }
    }

    float maxR = 0, maxG = 0, maxB = 0;
    Point p = {0.5, 0.5};
    float color = 0.5;
    for (long i = 0; i < (long)w * (long)h * 15L; ++i) {
        int affIdx = rand() % sizeof(affines) / sizeof(float[6]);
        affine(&p, affines[affIdx]);
        int idx = rand() % nFuncs;
        funcs[idx](&p, affines[affIdx]);
        int x = (int)((p.x + 1) * (w / 2));
        int y = (int)((p.y + 1) * (h / 2));
        color = (color + ((float)idx / nFuncs)) / 2;
        if (x >= 0 && x < w && y >= 0 && y < h) {
            hist[x][y].r += hue2rgb(color).r;
            if (hist[x][y].r > maxR) maxR = hist[x][y].r;
            hist[x][y].g += hue2rgb(color).g;
            if (hist[x][y].g > maxG) maxG = hist[x][y].g;
            hist[x][y].b += hue2rgb(color).b;
            if (hist[x][y].b > maxB) maxB = hist[x][y].b;
        }
    }

    char buf[20];  // just to be safe. too tired to figure out the real value
    sprintf(buf, "out%03d.ppm", num);
    FILE *f = fopen(buf, "w");
    fprintf(f, "P3\n%d %d\n255\n", w, h);
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            //int val = (int)((1 / log(2)) * log((float)hist[x][y] / max + 1) * 255);
            //int val = (int)((float)hist[x][y] / max * 255) * 10;
            //if (val > 255) val = 255;
            //NOTE: const power below (24) can be any even number
            const int p = 10;
            //int val = (int)(255 * (1 - pow((float)hist[x][y] / max - 1, 4)));
            fprintf(f, "%d %d %d\n",
                    (int)(255 * (1 - pow(hist[x][y].r / maxR - 1, p))),
                    (int)(255 * (1 - pow(hist[x][y].g / maxG - 1, p))),
                    (int)(255 * (1 - pow(hist[x][y].b / maxB - 1, p))));
        }
    }
    fclose(f);
}

int main(int argc, char **argv) {
    int w = 1000, h = 1000, nFrames = 1;

    int c;
    while ((c = getopt(argc, argv, "f:w:h:")) != -1) {
        switch (c) {
        case 'w':
            w = atoi(optarg);
            break;
        case 'h':
            h = atoi(optarg);
            break;
        case 'f':
            nFrames = atoi(optarg);
            break;
        case '?':
            fprintf(stderr, "bad option `-%c'\n", optopt);
            return 1;
        default:
            fprintf(stderr, "something happened???");
            return 1;
        }
    }

    int nFuncs = argc - optind;
    TransformFunc *funcs = malloc(nFuncs * sizeof(TransformFunc));
    for (int i = optind; i < argc; ++i) {
        switch (atoi(argv[i])) {
        case 0:  funcs[i - optind] = v0;  break;
        case 1:  funcs[i - optind] = v1;  break;
        case 2:  funcs[i - optind] = v2;  break;
        case 3:  funcs[i - optind] = v3;  break;
        case 4:  funcs[i - optind] = v4;  break;
        case 5:  funcs[i - optind] = v5;  break;
        case 6:  funcs[i - optind] = v6;  break;
        case 7:  funcs[i - optind] = v7;  break;
        case 8:  funcs[i - optind] = v8;  break;
        case 9:  funcs[i - optind] = v9;  break;
        case 10: funcs[i - optind] = v10; break;
        case 11: funcs[i - optind] = v11; break;
        case 12: funcs[i - optind] = v12; break;
        case 13: funcs[i - optind] = v13; break;
        case 14: funcs[i - optind] = v14; break;
        case 15: funcs[i - optind] = v15; break;
        case 16: funcs[i - optind] = v16; break;
        case 17: funcs[i - optind] = v17; break;
        case 18: funcs[i - optind] = v18; break;
        case 19: funcs[i - optind] = v19; break;
        case 20: funcs[i - optind] = v20; break;
        default:
            fprintf(stderr, "unknown variation %s\n", argv[i]);
            return 1;
        }
    }

    srand(time(NULL));

    for (int i = 0; i < (sizeof(affines) / sizeof(float[6])); ++i) {
        for (int j = 0; j < 6; ++j) affines[i][j] = (rand() / (float)RAND_MAX)*2 - 1;
    }

    for (int i = 0; i < nFrames; ++i) {
        render(w, h, nFuncs, funcs, i);
        affines[0][0] += 0.01;
        affines[0][1] -= 0.01;
        affines[0][2] -= 0.01;
        affines[0][3] += 0.01;
        affines[0][4] += 0.01;
        affines[0][5] -= 0.01;
        affines[1][0] += 0.01;
        affines[1][1] -= 0.01;
        affines[1][2] -= 0.01;
        affines[1][3] += 0.01;
        affines[1][4] += 0.01;
        affines[1][5] -= 0.01;
        affines[2][0] += 0.01;
        affines[2][1] -= 0.01;
        affines[2][2] -= 0.01;
        affines[2][3] += 0.01;
        affines[2][4] += 0.01;
        affines[2][5] -= 0.01;
        affines[3][0] += 0.01;
        affines[3][1] -= 0.01;
        affines[3][2] -= 0.01;
        affines[3][3] += 0.01;
        affines[3][4] += 0.01;
        affines[3][5] -= 0.01;
    }

    if (nFrames > 1) {
        char buf[100];
        sprintf(buf, "convert -delay 2 -size %dx%d -loop 0 out*.ppm out.gif", w, h);
        system(buf);
    }

    return 0;
}
