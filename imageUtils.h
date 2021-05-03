#pragma once

#include <SDL/SDL.h>
#include "SDL/SDL_image.h"
#include <math.h>
#include <err.h>
#include <pthread.h>
#include "queue.h"
#include "imageFilter.h"


double distanceToColorHSV(Color *c, Color *c2);
double distanceToColor(Color *c, Color *c2);

void invert(Color *c);
void grayscale(Color *c);
void stepColoring(Color *c);
void enhanceBlack(Color *c);
void thickenColor(SDL_Surface *surface, Color *c);
void keepTopoLine(Color *c);
void keepTopoLineHSV(SDL_Surface *image, Color *topoColor, double threshold);
void setMonochromatic(SDL_Surface *image, Color *c);
void blur(SDL_Surface **image, int blurLevel);
void removeIsolatedPixels(SDL_Surface *image);
float *heightList(SDL_Surface *image, int nbRows, int nbCols);

void replaceColor(SDL_Surface *image, Color *c1, Color *c2);

void keepColorAndSave(char *filenameSource, char *filenameDest, RGB c, double threshold); //topoline or river
void FindAllExtremityAndSave(char *filenameSource, char *filenameDest);
void makeHeightMap(char *filenameSource, char* filenameDest); // make height map from topolines
