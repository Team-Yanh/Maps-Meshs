#include "imageUtils.h"

void thickenColor(SDL_Surface *image, Color *color)
{
    Queue *q = createQueue();
    int x = 0;
    int y = 0;

    Uint32 pixel = color->pixel;

    for(int i = 0; i < image->w; i++)
    {
        for(int j = 0; j < image->h; j++)
        {
            Uint32 currentPixel = getPixel(image, i, j);
            if(currentPixel == pixel)
                enqueue(q, i, j);
        }
    }

    SDL_LockSurface(image);
    while(!isEmpty(q))
    {
        dequeue(q, &x, &y);
        //color each adjacent neighbour the same color
        if(isValidCell(image, x-1, y))
            putPixel(image, x-1, y, pixel);
        if(isValidCell(image, x+1, y))
            putPixel(image, x+1, y, pixel);

        if(isValidCell(image, x, y-1))
            putPixel(image, x, y-1, pixel);
        if(isValidCell(image, x, y+1))
            putPixel(image, x, y+1, pixel);
    }
    SDL_UnlockSurface(image);
    freeQueue(q);
}

Uint32 distanceToColorHSV(Color *c1, Color *c2)
{
    return  sqrt( (c1->hsv->h - c2->hsv->h) * (c1->hsv->h - c2->hsv->h) );
}

Uint32 distanceToColor(Color *c1, Color *c2)
{
    return (Uint32)sqrt( (c1->rgb->r - c2->rgb->r)*(c1->rgb->r - c2->rgb->r) +
            (c1->rgb->b - c2->rgb->b)*(c1->rgb->b - c2->rgb->b) + 
            (c1->rgb->g - c2->rgb->g)*(c1->rgb->g - c2->rgb->g));
}

void invert(Color *c)
{
    setRGB(c , 255 - c->rgb->r, 255 - c->rgb->g, 255 - c->rgb->b);
}

void grayscale(Color *c)
{
    Uint8 luminance = 0.21 * c->rgb->r + 0.71 * c->rgb->g + 0.07 * c->rgb->b;
    setRGB(c , luminance, luminance, luminance);
}

void stepColoring(Color *c)
{
    setRGB(c , c->rgb->r - (c->rgb->r % 25),
            c->rgb->g - (c->rgb->g % 25), c->rgb->b - (c->rgb->b % 25));
}

void enhanceBlack(Color *c)
{
    Uint32 distanceToBlack = (Uint32) sqrt( (c->rgb->r * c->rgb->r) +
            (c->rgb->g * c->rgb->g) + (c->rgb->b * c->rgb->b) );
    if(distanceToBlack < 100)
        if(distanceToBlack < 100)
        {
            setRGB(c, 0, 0, 0);
        }
}

void keepTopoLine(Color *c)
{
    Color *topoColor = initColor(c->format);
    setRGB(topoColor, 217, 200, 170);
    if(distanceToColor(c, topoColor) > 10)
    {
        setRGB(c, 255, 255, 255);
    }
    freeColor(topoColor);
}

void keepTopoLineHSV(SDL_Surface *image, Color *topoColor)
{
    Color *currentColor = initColor(topoColor->format);
    Color *white = initColor(topoColor->format);
    setRGB(topoColor, 217, 200, 170);
    setRGB(white, 255, 255, 255);
    for(int i = 0; i < image->w; i++)
    {
        for(int j = 0; j < image->h; j++)
        {
            setPixel(currentColor, getPixel(image, i, j));
            if(distanceToColorHSV(currentColor, topoColor) > 1)
                putPixel(image, i, j, white->pixel);
        }
    }
    freeColor(currentColor);
    freeColor(white);
}

void setMonochromatic(SDL_Surface *image, Color *c)
{
    Color *white = initColor(image->format);
    Color *currentColor = initColor(image->format);
    SDL_LockSurface(image);
    for(int i = 0; i < image->w; i++)
    {
        for(int j = 0; j < image->h; j++)
        {
            setPixel(currentColor, getPixel(image, i, j));
            if(isSameColor(white, currentColor))
                putPixel(image, i, j, c->pixel);
        }
    }
    SDL_UnlockSurface(image);
    freeColor(white);
    freeColor(currentColor);
}

void replaceColor(SDL_Surface *image, Color *c1, Color *c2)
{
    Color *currentColor = initColor(image->format);
    for(int i = 0; i < image->w; i++)
    {
        for(int j = 0; j < image->h; j++)
        {
            setPixel(currentColor, getPixel(image, i, j));
            if(isSameColor(currentColor, c1))
                putPixel(image, i, j, c2->pixel);
        }
    }
    freeColor(currentColor);
}

Color *getAverage(SDL_Surface *image, const int x, const int y, int blurLevel)
{
    Color *average = initColor(image->format);

    long r = 0;
    long g = 0;
    long b = 0;
    int cellCount = 0;

    for(int i = x - blurLevel; i <= x + blurLevel; i++)
    {
        for(int j = y - blurLevel; j <= y + blurLevel; j++) // all pixels in grid around x,y
        {
            if(isValidCell(image, i, j))
            {
                cellCount++;
                setPixel(average, getPixel(image, i, j));
                r += average->rgb->r;
                g += average->rgb->g;
                b += average->rgb->b;
            }
        }
    }

    r /= cellCount;
    g /= cellCount;
    b /= cellCount;
    setRGB(average, (Uint8)r, (Uint8)g, (Uint8)b);

    return average;
}

void blur(SDL_Surface **image, int blurLevel)
{
    // copy image to result;
    SDL_Surface *result = SDL_ConvertSurface(*image, (*image)->format, SDL_SWSURFACE);

    Color *currentColor;
    for(int i = 0; i < (*image)->w; i++)
    {
        for(int j = 0; j < (*image)->h; j++) // each pixel
        {
            currentColor = getAverage(*image, i, j, blurLevel);
            putPixel(result, i, j, currentColor->pixel);
            freeColor(currentColor);
        }
    }
    SDL_FreeSurface(*image); // replace image by result
    *image = result;
}

float *heightList(SDL_Surface *image, int nbRows, int nbCols)
{
    int rowsStep = image->h / nbRows;
    int colsStep = image->w / nbCols;
    int x = 0;
    int y = 0;

    printf("nbRows %d, nbCols %d, rowsStep %d, colsStep %d\n", nbRows, nbCols, rowsStep, colsStep);
    Color *currentColor = initColor(image->format);

    float *result = malloc(nbRows * nbCols * sizeof(float));

    for(int i = 0; i < nbRows; i++) // get all values
    {
        for(int j = 0; j < nbCols; j++)
        {
            x = colsStep * j;
            y = image->h - 1 - rowsStep * i;
            setPixel(currentColor, getPixel(image, x, y));
            result[j + nbCols * i] = ((float)currentColor->rgb->r) / 255.0;
            //printf("%.1f ", result[j + nbCols * i]);
        }
        //printf("\n");
    }

    freeColor(currentColor);
    return result;
}
