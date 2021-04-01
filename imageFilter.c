#include "imageFilter.h"
#include "queue.h"

HSV *RGBToHSV(RGB *rgb)
{
    HSV *res = malloc(sizeof(HSV));
    float r2 = rgb->r/255.0;
    float g2 = rgb->g/255.0;
    float b2 = rgb->b/255.0;

    float cmax = fmaxf(r2, fmaxf(g2, b2));
    float cmin = fminf(r2, fminf(g2, b2));
    float delta = cmax - cmin;

    if(cmax == cmin)
        res->h = 0.0;
    else
    {
        if(cmax == r2)
            res->h = fmod( (60.0 * (g2-b2)/delta + 360.0), 360.0);
        else
        {
            if(cmax == g2)
                res->h = fmod( (60.0 * (b2-r2)/delta + 120.0), 360.0);
            else
            {
                if(cmax == b2)
                    res->h = fmod( (60.0 * (r2-g2)/delta + 240.0), 360.0);
            }
        }
    }
    if(cmax == 0)
        res->s = 0;
    else
        res->s = delta/cmax;
    res->v = cmax;

    return res;
}

RGB *HSVToRGB(HSV *hsv) // not tested
{
    float h = hsv->h;
    float s = hsv->s;
    float v = hsv->v;

    RGB *rgb = malloc(sizeof(RGB));
    float c = v * s;
    float x = c * (1 - (float)fabs( fmod((h/60.0), 2.0) - 1 ) );
    float m = v - c;

    float r2, g2, b2;

    if(h >= 0.0 && h < 60.0)
    {
        r2 = c;
        g2 = x;
        b2 = 0;
    }
    else if(h >= 60.0 && h < 120.0)
    {
        r2 = x;
        g2 = c;
        b2 = 0;
    }
    else if(h >= 120.0 && h < 180.0)
    {
        r2 = 0;
        g2 = c;
        b2 = x;
    }
    else if(h >= 180.0 && h < 240.0)
    {
        r2 = 0;
        g2 = x;
        b2 = c;
    }
    else if(h >= 240.0 && h < 300.0)
    {
        r2 = x;
        g2 = 0;
        b2 = c;
    }
    else
    {
        r2 = c;
        g2 = 0;
        b2 = c;
    }

    rgb->r = (Uint8)((r2+m) * 255.0);
    rgb->g = (Uint8)((g2+m) * 255.0);
    rgb->b = (Uint8)((b2+m) * 255.0);
    return rgb;
}

Color *initColor(SDL_PixelFormat *format)
{
    Color *c = calloc(1, sizeof(Color));
    c->pixel = 0;
    c->format = format;
    c->rgb = calloc(1, sizeof(RGB));
    c->hsv = calloc(1, sizeof(HSV));

    return c;
}

void setRGB(Color *c, Uint8 r, Uint8 g, Uint8 b)
{
    c->rgb->r = r;
    c->rgb->g = g;
    c->rgb->b = b;

    c->pixel = SDL_MapRGB(c->format, r, g, b);
    free(c->hsv);
    c->hsv = RGBToHSV(c->rgb);
}

void setHSV(Color *c, float h, float s, float v)
{
    c->hsv->h = fmodf(h, 360.0);
    c->hsv->s = s;
    c->hsv->v = v;

    free(c->rgb);
    c->rgb = HSVToRGB(c->hsv);
    c->pixel = SDL_MapRGB(c->format,
            c->rgb->r, c->rgb->g, c->rgb->b);
}

void setPixel(Color *c, Uint32 pixel)
{
    c->pixel = pixel;

    Uint8 r, g, b;
    SDL_GetRGB(c->pixel, c->format,
            &r, &g, &b);
    c->rgb->r = r;
    c->rgb->g = g;
    c->rgb->b = b;
    free(c->hsv);
    c->hsv = RGBToHSV(c->rgb);
}

void freeColor(Color *color)
{
    free(color->rgb);
    free(color->hsv);
    free(color);
}

Uint32 getPixel(SDL_Surface *surface, int x, int y) //copied for sdl_wiki
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
        case 1:
            return *p;
            break;

        case 2:
            return *(Uint16 *)p;
            break;

        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;
            break;

        case 4:
            return *(Uint32 *)p;
            break;

        default:
            return 0;       /* shouldn't happen, but avoids warnings */
    }
}

void putPixel(SDL_Surface *surface, int x, int y, Uint32 pixel) //copied from sdl_wiki
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
        case 1:
            *p = pixel;
            break;

        case 2:
            *(Uint16 *)p = pixel;
            break;

        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            } else {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4:
            *(Uint32 *)p = pixel;
            break;
    }
}

void forEachPixel(SDL_Surface *image, void (*filter)(Color *c))
{
    Color *c = initColor(image->format);
    SDL_LockSurface(image);
    for(int i = 0; i < image->w; i++)
    {
        for(int j = 0; j < image->h; j++)
        {
            setPixel(c, getPixel(image, i, j));
            filter(c);
            putPixel(image, i, j, c->pixel);
        }
    }
    SDL_UnlockSurface(image);
    freeColor(c);
}

int isValidCell(SDL_Surface *surface, int x, int y)
{
    if(x < 0 || x >= surface->w || y < 0 || y >= surface->h)
        return 0;//false ie out of bound
    return 1; //true
}

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

int isSameColor(Color *c, Color *c2)
{
    return c->rgb->r == c2->rgb->r &&
        c->rgb->g == c2->rgb->g &&
        c->rgb->b == c2->rgb->b;
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

void colorCircles(SDL_Surface *image)
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
    setRGB(blackMarked, 0, 255, 0);
    setRGB(circle, 255, 0, 0);
    setRGB(white, 255, 255, 255);

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
    while(!isEmpty(q))
    {
        dequeue(q, &x, &y);
        colorZoneBFS(image, circle, x, y);
        replaceColor(image, blackMarked, circle);
    }

    freeQueue(q);
    freeColor(c);
    freeColor(currentColor);
    freeColor(circle);
    freeColor(white);
    freeColor(black);
    freeColor(blackMarked);
}

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
    }

    freeColor(black);
    freeColor(blackMarked);
    freeColor(bgColor);
    freeColor(currentColor);
    freeQueue(q);
    return counter;
}

void colorAllZones(SDL_Surface *image)
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

void keepTopoLineHSV(Color *c)
{
    Color *topoColor = initColor(c->format);
    setRGB(topoColor, 217, 200, 170);
    if(distanceToColorHSV(c, topoColor) > 1)
    {
        setRGB(c, 255, 255, 255);
    }
    freeColor(topoColor);
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
