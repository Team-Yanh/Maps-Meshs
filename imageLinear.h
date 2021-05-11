#pragma once

#include <SDL/SDL.h>
#include "SDL/SDL_image.h"
#include "vector.h"
#include "imageFilter.h"

typedef struct Point {
int x;
int y;
}Point;

int CalculY(Point *p1,Point *p2,int x3);
void interpol(SDL_Surface *image,Point *p1,Point *p2);
