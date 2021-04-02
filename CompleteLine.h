#pragma once

#include <SDL/SDL.h>
#include "SDL/SDL_image.h"
#include <math.h>
#include "queue.h"
#include "vector.h"
#include "imageFilter.h"

typedef struct Point {
int x;
int y;
}Point;

void FindExtremity(SDL_Surface *image , struct vector *v,Point point);
void LinkExtremity(SDL_Surface *image,struct vector *ListEx);
void FindAllExtremity(SDL_Surface *image);
