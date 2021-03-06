#pragma once

#include <SDL/SDL.h>
#include "SDL/SDL_image.h"
#include "filters.h"

Uint32 getPixel(SDL_Surface *surface, int x, int y);
void putPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
void forEachPixel(SDL_Surface *image, void(*f)(Uint8 *r, Uint8 *g, Uint8 *b));
void removeAllColors(SDL_Surface *image);
void removeLetters(SDL_Surface *image);
void colorZones(SDL_Surface *image);
