#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

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
    float r = 1 / (p->x*p->x + p->y*p->y);
    p->x = r*(p->x-p->y)*(p->x+p->y);
    p->y = 2*p->x*p->y;
}

void v16(Point *p, float aff[6]) {
    float r = 2 / (p->x*p->x + p->y*p->y + 1);
    p->x = r*p->y;
    p->y = r*p->x;
}

void v17(Point *p, float aff[6]) {
    p->x = p->x + aff[2]*sin(tan(3*p->y));
    p->y = p->y + aff[5]*sin(tan(3*p->x));
}

TransformFunc funcs[] = {v1, v2, v3, v16, v17, v4};
int nFuncs = sizeof(funcs) / sizeof(TransformFunc);

int main() {
    srand(time(NULL));

    for (int i = 0; i < (sizeof(affines) / sizeof(float[6])); ++i) {
        for (int j = 0; j < 6; ++j) affines[i][j] = (rand() / (float)RAND_MAX)*2 - 1;
    }

    const int w = 600, h = 500;
    Color hist[w][h];
    for (int x = 0; x < w; ++x) {
        for (int y = 0; y < h; ++y) {
            hist[x][y] = (Color) {0.0, 0.0, 0.0};
        }
    }

    float maxR = 0, maxG = 0, maxB = 0;
    Point p = {0.5, 0.5};
    float color = 0.5;
    for (int i = 0; i < 10000000; ++i) {
        int affIdx = rand() % sizeof(affines) / sizeof(float[6]);
        affine(&p, affines[affIdx]);
        int idx = rand() % nFuncs;
        funcs[idx](&p, affines[affIdx]);
        int x = (int)((p.x + 1) * (w / 2));
        int y = (int)((p.y + 1) * (h / 2));
        color = (color + ((float)idx / nFuncs)) / 2;
        if (p.x > -1 && p.x < 1 && p.y > -1 && p.y < 1) {
            hist[x][y].r += hue2rgb(color).r;
            if (hist[x][y].r > maxR) maxR = hist[x][y].r;
            hist[x][y].g += hue2rgb(color).g;
            if (hist[x][y].g > maxG) maxG = hist[x][y].g;
            hist[x][y].b += hue2rgb(color).b;
            if (hist[x][y].b > maxB) maxB = hist[x][y].b;
        }
    }

    printf("P3\n%d %d\n255\n", w, h);
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            //int val = (int)((1 / log(2)) * log((float)hist[x][y] / max + 1) * 255);
            //int val = (int)((float)hist[x][y] / max * 255) * 10;
            //if (val > 255) val = 255;
            //NOTE: const power below (4) can be any even number
            const int p = 24;
            //int val = (int)(255 * (1 - pow((float)hist[x][y] / max - 1, 4)));
            printf("%d %d %d\n",
                    (int)(255 * (1 - pow(hist[x][y].r / maxR - 1, p))),
                    (int)(255 * (1 - pow(hist[x][y].g / maxG - 1, p))),
                    (int)(255 * (1 - pow(hist[x][y].b / maxB - 1, p))));
        }
    }

    return 0;
}
