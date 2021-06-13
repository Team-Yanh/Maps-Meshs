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


int isValidCell(SDL_Surface *surface, int x, int y)
{
    if(x < 0 || x >= surface->w || y < 0 || y >= surface->h)
        return 0;//false ie out of bound
    return 1; //true
}

int isValidCellAndColor(SDL_Surface *image, int x, int y, Color *c)
{
    if(isValidCell(image, x, y))
    {
        Color *cellColor = initColor(image->format);
        setPixel(cellColor, getPixel(image, x, y));
        if(isSameColor(cellColor, c))
        {
            freeColor(cellColor);
            return 1;
        }
        else
        {
            freeColor(cellColor);
            return 0;
        }
    }
    return 0;
}

int isSameColor(Color *c, Color *c2)
{
    return c->pixel == c2->pixel;
}

int isWhiteInImage(SDL_Surface *image)
{
    Color *white = initColor(image->format);
    setRGB(white, 255, 255, 255);
    Color *currentColor = initColor(image->format);

    for(int i = 0; i < image->w; i++)
    {
        for(int j = 0; j < image->h; j++)
        {
            setPixel(currentColor, getPixel(image, i, j));
            if(isSameColor(white, currentColor))
            {
                freeColor(white);
                freeColor(currentColor);
                return 1;
            }
        }
    }

    freeColor(white);
    freeColor(currentColor);
    return 0;
}
