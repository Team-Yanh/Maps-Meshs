#include <math.h>
#include "filters.h"


Uint32 distanceToColor(Uint8 r, Uint8 g, Uint8 b,
        Uint8 r2, Uint8 g2, Uint8 b2 )
{
    return (Uint32)sqrt( (r - r2)*(r - r2) + (b - b2)*(b - b2)
            + (g - g2)*(g - g2));
}
void invert(Uint8 *r, Uint8 *g, Uint8 *b)
{
    *r = 255 - *r;
    *g = 255 - *g;
    *b = 255 - *b;
}

void grayscale(Uint8 *r, Uint8 *g, Uint8 *b)
{
    Uint8 luminance = 0.21 * *r + 0.71 * *g + 0.07 * *b;
    *r = luminance;
    *g = luminance;
    *b = luminance;
}

void stepColoring(Uint8 *r, Uint8 *g, Uint8 *b)
{
    *r = *r - (*r % 25);
    *g = *g - (*g % 25);
    *b = *b - (*b % 25);
}

void enhanceBlack(Uint8 *r, Uint8 *g, Uint8 *b)
{
    Uint32 distanceToBlack = (Uint32) sqrt( (*r * *r) + (*g * *g) + (*b * *b) );
    if(distanceToBlack < 100)
    {
        *r = 0;
        *g = 0;
        *b = 0;
    }
}

void removeGreen(Uint8 *r, Uint8 *g, Uint8 *b)
{
    if(*g > 230)
    {
        *r = 255;
        *g = 255;
        *b = 255;
    }
}

void keepTopoLine(Uint8 *r, Uint8 *g, Uint8 *b)
{
    if(distanceToColor(*r, *g, *b, 217, 200, 170) > 30)
    {
        *r = 255;
        *g = 255;
        *b = 255;
    }
}

void cleanGray(Uint8 *r, Uint8 *g, Uint8 *b)
{
    Uint8 gray = (*r + *g + *b) / 3;
    Uint32 distance = distanceToColor(*r, *g, *b, gray, gray, gray);
    if(distance < 20)
    {
        *r = 255;
        *g = 255;
        *b = 255;
    }
}
