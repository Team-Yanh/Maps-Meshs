#pragma once

#include <SDL/SDL.h> // Uint8 type is from SDL

Uint32 distanceToColor(Uint8 r, Uint8 g, Uint8 b,
        Uint8 r2, Uint8 g2, Uint8 b2 );
void invert(Uint8 *r, Uint8 *g, Uint8 *b);
void grayscale(Uint8 *r, Uint8 *g, Uint8 *b);
void stepColoring(Uint8 *r, Uint8 *g, Uint8 *b);
void enhanceBlack(Uint8 *r, Uint8 *g, Uint8 *b);
void removeGreen(Uint8 *r, Uint8 *g, Uint8 *b);
void keepTopoLine(Uint8 *r, Uint8 *g, Uint8 *b);
void cleanGray(Uint8 *r, Uint8 *g, Uint8 *b);
