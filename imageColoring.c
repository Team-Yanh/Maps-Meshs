#include "imageColoring.h"

int colorZoneBFS(SDL_Surface *image, Color *c, int x, int y)
{
    Queue *q = createQueue();

    int counter = 0;
    Color *black = initColor(image->format);
    setRGB(black, 0, 0, 0);
    Color *blackMarked = initColor(image->format);
    setRGB(blackMarked, 0, 255, 0);
    Color *bgColor = initColor(image->format);
    Color *currentColor = initColor(image->format);
    setPixel(bgColor, getPixel(image, x, y));

    putPixel(image, x, y, c->pixel);
    enqueue(q, x, y);

    while(!isEmpty(q))
    {
        dequeue(q, &x, &y);

        if(isValidCell(image, x-1, y))
        {
            setPixel(currentColor, getPixel(image, x-1, y));
            if(isSameColor(bgColor, currentColor))
            {
                putPixel(image, x-1, y, c->pixel);
                enqueue(q, x-1, y);
            }
            else if(isSameColor(black, currentColor))
            {
                counter++;
                colorZoneBFS(image, blackMarked, x-1, y);
            }
        }
        else
        {
            counter++;
        }
        if(isValidCell(image, x+1, y))
        {
            setPixel(currentColor, getPixel(image, x+1, y));
            if(isSameColor(bgColor, currentColor))
            {
                putPixel(image, x+1, y, c->pixel);
                enqueue(q, x+1, y);
            }
            else if(isSameColor(black, currentColor))
            {
                counter++;
                colorZoneBFS(image, blackMarked, x+1, y);
            }
        }
        else
        {
            counter++;
        }
        if(isValidCell(image, x, y-1))
        {
            setPixel(currentColor, getPixel(image, x, y-1));
            if(isSameColor(bgColor, currentColor))
            {
                putPixel(image, x, y-1, c->pixel);
                enqueue(q, x, y-1);
            }
            else if(isSameColor(black, currentColor))
            {
                counter++;
                colorZoneBFS(image, blackMarked, x, y-1);
            }
        }
        else
        {
            counter++;
        }
        if(isValidCell(image, x, y+1))
        {
            setPixel(currentColor, getPixel(image, x, y+1));
            if(isSameColor(bgColor, currentColor))
            {
                putPixel(image, x, y+1, c->pixel);
                enqueue(q, x, y+1);
            }
            else if(isSameColor(black, currentColor))
            {
                counter++;
                colorZoneBFS(image, blackMarked, x, y+1);
            }
        }
        else
        {
            counter++;
        }
    }

    freeColor(black);
    freeColor(blackMarked);
    freeColor(bgColor);
    freeColor(currentColor);
    freeQueue(q);
    return counter;
}

int isValidNeighbour(SDL_Surface *image, int x, int y)
{
    Color *white = initColor(image->format);
    setRGB(white, 255, 255, 255);
    Color *currentCellColor = initColor(image->format);
    int res = isValidCell(image, x, y);
    if(res == 1)
    {
        setPixel(currentCellColor, getPixel(image, x, y));
        res = isSameColor(white, currentCellColor);
    }
    freeColor(white);
    freeColor(currentCellColor);
    return res;
}

int colorCircles(SDL_Surface *image)
{
    int counter = 0;
    Queue *q = createQueue();

    Color *c = initColor(image->format);
    Color *currentColor = initColor(image->format);
    Color *circle = initColor(image->format);
    Color *white = initColor(image->format);
    Color *black = initColor(image->format);
    Color *blackMarked = initColor(image->format);
    setRGB(c, 200, 0, 0);
    setRGB(black, 0, 0, 0);
    setRGB(white, 255, 255, 255);
    setRGB(blackMarked, 0, 255, 0);
    setRGB(circle, 254, 254, 254);

    for(int i = 0; i < image->w; i++)
    {
        for(int j = 0; j < image->h; j++)
        {
            setPixel(currentColor, getPixel(image, i, j));
            if(isSameColor(white, currentColor))
            {
                counter = colorZoneBFS(image, c, i, j);
                replaceColor(image, blackMarked, black);
                if(counter == 1)
                    enqueue(q, i, j);
            }
        }
    }

    int x = 0;
    int y = 0;
    int result = 0;

    replaceColor(image, c, white);

    if(!isEmpty(q)) // have circles
    {
        result = 1;
        while(!isEmpty(q))
        {
            dequeue(q, &x, &y);
            colorZoneBFS(image, circle, x, y);
            replaceColor(image, blackMarked, circle);
        }
    }

    freeQueue(q);
    freeColor(c);
    freeColor(currentColor);
    freeColor(circle);
    freeColor(white);
    freeColor(black);
    freeColor(blackMarked);

    return result;
}

int colorAllZonesFromCircles(SDL_Surface *image)
{
    int nbColors = 1;

    printf("Finding summits...\n");

    Color *currentColor = initColor(image->format);
    Color *circle = initColor(image->format);
    Color *c = initColor(image->format);
    Color *white = initColor(image->format);
    Color *black = initColor(image->format);
    Color *blackMarked = initColor(image->format);
    setRGB(black, 0, 0, 0);
    setRGB(blackMarked, 0, 255, 0);
    setRGB(circle, 254, 254, 254);
    setRGB(c, 253, 253, 253);
    setRGB(white, 255, 255, 255);

    int isCircle = colorCircles(image);
    if(isCircle == 0) // no circle
    {
        thickenColor(image, black);
        isCircle = colorCircles(image);
        if(isCircle == 0) // no circle after thickenning, color top left
        {
            colorZoneBFS(image, circle, 0, 0);
            replaceColor(image, blackMarked, circle);
        }
    }

    printf("Coloring...\n");

    while(isWhiteInImage(image)) //while we can, color adjacent circles with c
    {
        for(int i = 0; i < image->w; i++)
        {
            for(int j = 0; j < image->h; j++)
            {
                setPixel(currentColor, getPixel(image, i, j));
                if(isSameColor(circle, currentColor))
                {
                    if(isValidCellAndColor(image, i-1, j, white))
                    {
                        colorZoneBFS(image, c, i-1, j);
                        replaceColor(image, blackMarked, c);
                    }
                    if(isValidCellAndColor(image, i+1, j, white))
                    {
                        colorZoneBFS(image, c, i+1, j);
                        replaceColor(image, blackMarked, c);
                    }
                    if(isValidCellAndColor(image, i, j-1, white))
                    {
                        colorZoneBFS(image, c, i, j-1);
                        replaceColor(image, blackMarked, c);
                    }
                    if(isValidCellAndColor(image, i, j+1, white))
                    {
                        colorZoneBFS(image, c, i, j+1);
                        replaceColor(image, blackMarked, c);
                    }
                }
            }
        }
        nbColors++;
        setRGB(circle, c->rgb->r, c->rgb->g, c->rgb->b);
        setRGB(c, c->rgb->r - 1, c->rgb->g - 1, c->rgb->b - 1);
    }

    freeColor(currentColor);
    freeColor(circle);
    freeColor(c);
    freeColor(white);
    freeColor(black);
    freeColor(blackMarked);

    printf("nbColors : %d\n", nbColors);
    return nbColors;
}

void normalize(SDL_Surface *image, int nbColors)
{
    int step = 255/nbColors;

    Color *c = initColor(image->format);
    Color *currentColor = initColor(image->format);
    setRGB(c, 0, 0, 0);
    setRGB(currentColor, 255 - nbColors, 255 - nbColors, 255 - nbColors);
    // start at 254 : circle color

    for(int i = nbColors; i > 0; i--)
    {
        replaceColor(image, currentColor, c);
        setRGB(c, c->rgb->r + step, c->rgb->g + step, c->rgb->b + step);
        setRGB(currentColor, currentColor->rgb->r + 1,
               currentColor->rgb->g + 1, currentColor->rgb->b + 1);
    }

    freeColor(currentColor);
    freeColor(c);
}

void colorZoneDFS(SDL_Surface *image, Color *c, int x, int y)
{
    putPixel(image, x, y, c->pixel);
    if(isValidNeighbour(image, x-1, y))
        colorZoneDFS(image, c, x-1, y);
    if(isValidNeighbour(image, x+1, y))
        colorZoneDFS(image, c, x+1, y);
    if(isValidNeighbour(image, x, y-1))
        colorZoneDFS(image, c, x, y-1);
    if(isValidNeighbour(image, x, y+1))
        colorZoneDFS(image, c, x, y+1);
}

int colorZoneDFSCount(SDL_Surface *image, Color *c, int x, int y)
{
    int res = 0;
    Color *black = initColor(image->format);
    Color *blackMark = initColor(image->format);
    Color *white = initColor(image->format);
    Color *currentColor = initColor(image->format);
    setRGB(white, 255, 255, 255);
    setRGB(black, 0, 0, 0);
    setRGB(blackMark, 1, 1, 1);
    putPixel(image, x, y, c->pixel);

    if(isValidCell(image, x-1, y))
    {
        currentColor->pixel = getPixel(image, x-1, y);
        if(isSameColor(currentColor, white))
            colorZoneDFS(image, c, x-1, y);
        else
        {
            if(isSameColor(currentColor, black))
            {
                res++;
                colorZoneDFS(image, blackMark, x-1, y);
            }
        }
    }

    if(isValidCell(image, x+1, y))
    {
        currentColor->pixel = getPixel(image, x+1, y);
        if(isSameColor(currentColor, white))
            colorZoneDFS(image, c, x+1, y);
        else
        {
            if(isSameColor(currentColor, black))
            {
                res++;
                colorZoneDFS(image, blackMark, x+1, y);
            }
        }
    }

    if(isValidCell(image, x, y-1))
    {
        currentColor->pixel = getPixel(image, x, y-1);
        if(isSameColor(currentColor, white))
            colorZoneDFS(image, c, x, y-1);
        else
        {
            if(isSameColor(currentColor, black))
            {
                res++;
                colorZoneDFS(image, blackMark, x, y-1);
            }
        }
    }
    if(isValidCell(image, x, y+1))
    {
        currentColor->pixel = getPixel(image, x, y+1);
        if(isSameColor(currentColor, white))
            colorZoneDFS(image, c, x, y+1);
        else
        {
            if(isSameColor(currentColor, black))
            {
                res++;
                colorZoneDFS(image, blackMark, x, y+1);
            }
        }
    }
    freeColor(black);
    freeColor(blackMark);
    freeColor(white);
    freeColor(currentColor);
    return res;
}

void colorAllZonesNaive(SDL_Surface *image)
{
    Color *c = initColor(image->format);
    Color *white = initColor(image->format);
    Color *currentColor = initColor(image->format);

    setHSV(c, 0.0, 1.0, 1.0);
    setRGB(white, 255, 255, 255);

    for(int i = 0; i < image->w; i++)
    {
        for(int j = 0; j < image->h; j++)
        {
            setPixel(currentColor, getPixel(image, i, j));
            if(isSameColor(currentColor, white))
            {
                colorZoneDFS(image, c, i, j);
                setHSV(c, c->hsv->h + 20, 1.0, 1.0);
            }
        }
    }

    freeColor(c);
    freeColor(white);
    freeColor(currentColor);
}

