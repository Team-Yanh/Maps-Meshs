#include "imageUtils.h"
#include "imageColoring.h"
#include "CompleteLine.h"

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

double distanceToColorHSV(Color *c1, Color *c2)
{
    return  sqrt( (c1->hsv->h - c2->hsv->h) * (c1->hsv->h - c2->hsv->h) );
}

double distanceToColor(Color *c1, Color *c2)
{
    return sqrt( (c1->rgb->r - c2->rgb->r)*(c1->rgb->r - c2->rgb->r) +
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
    if(distanceToColor(c, topoColor) > 10.0)
    {
        setRGB(c, 255, 255, 255);
    }
    freeColor(topoColor);
}

void keepTopoLineHSV(SDL_Surface *image, Color *topoColor, double threshold)
{
    Color *currentColor = initColor(topoColor->format);
    Color *white = initColor(topoColor->format);
    //setRGB(topoColor, 217, 200, 170);
    setRGB(white, 255, 255, 255);
    for(int i = 0; i < image->w; i++)
    {
        for(int j = 0; j < image->h; j++)
        {
            setPixel(currentColor, getPixel(image, i, j));
            if(distanceToColorHSV(currentColor, topoColor) > threshold)
                putPixel(image, i, j, white->pixel);
        }
    }
    freeColor(currentColor);
    freeColor(white);
}

void setMonochromatic(SDL_Surface *image, Color *c)
{
    Color *white = initColor(image->format);
    setRGB(white, 255, 255, 255);
    Color *currentColor = initColor(image->format);
    SDL_LockSurface(image);
    for(int i = 0; i < image->w; i++)
    {
        for(int j = 0; j < image->h; j++)
        {
            setPixel(currentColor, getPixel(image, i, j));
            if(!isSameColor(white, currentColor))
                putPixel(image, i, j, c->pixel);
        }
    }
    SDL_UnlockSurface(image);
    freeColor(white);
    freeColor(currentColor);
}

int replaceColor(SDL_Surface *image, Color *c1, Color *c2)
{
    int hasColor = 0;
    Color *currentColor = initColor(image->format);
    for(int i = 0; i < image->w; i++)
    {
        for(int j = 0; j < image->h; j++)
        {
            setPixel(currentColor, getPixel(image, i, j));
            if(isSameColor(currentColor, c1))
            {
                hasColor = 1;
                putPixel(image, i, j, c2->pixel);
            }
        }
    }
    freeColor(currentColor);

    return hasColor;
}

void lerpColor(Color *result, Color *leftColor, Color *rightColor, double f)
{
    Uint8 r = (Uint8)((double)leftColor->rgb->r * (1.0 - f))
                    + ((double)rightColor->rgb->r * f);
    Uint8 g = (Uint8)((double)leftColor->rgb->g * (1.0 - f))
                    + ((double)rightColor->rgb->g * f);
    Uint8 b = (Uint8)((double)leftColor->rgb->b * (1.0 - f))
                    + ((double)rightColor->rgb->b * f);
    setRGB(result, r, g, b);
}

void averageColors(Color *color, Color *color2, Color *result)
{
    uint16_t r = color->rgb->r + color2->rgb->r;
    uint16_t g = color->rgb->g + color2->rgb->g;
    uint16_t b = color->rgb->b + color2->rgb->b;
    setRGB(result, r / 2, g / 2, b / 2);
}

int hasNeighbours(SDL_Surface *image, int i, int j)
{
    Color *black = initColor(image->format);
    setRGB(black, 0, 0, 0);
    int result = 0;

    if(isValidCellAndColor(image, i - 1, j - 1, black))
        result = 1;
    if(isValidCellAndColor(image, i - 1, j, black))
        result = 1;
    if(isValidCellAndColor(image, i - 1, j + 1, black))
        result = 1;

    if(isValidCellAndColor(image, i, j - 1, black))
        result = 1;
    if(isValidCellAndColor(image, i, j + 1, black))
        result = 1;

    if(isValidCellAndColor(image, i + 1, j - 1, black))
        result = 1;
    if(isValidCellAndColor(image, i + 1, j, black))
        result = 1;
    if(isValidCellAndColor(image, i + 1, j + 1, black))
        result = 1;

    freeColor(black);
    return result;
}

void removeIsolatedPixels(SDL_Surface *image)
{
    Color *currentColor = initColor(image->format);
    
    Color *white = initColor(image->format);
    setRGB(white, 255, 255, 255);

    for(int i = 0; i < image->w; i++)
    {
        for(int j = 0; j < image->h; j++)
        {
            setPixel(currentColor, getPixel(image, i, j));

            // if no 8 neighbour is black, remove the pixel
            if(!hasNeighbours(image, i, j))
                putPixel(image, i, j, white->pixel);
        }
    }
    freeColor(white);
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

typedef struct blurArgs{
    pthread_t id;
    SDL_Surface *src;
    SDL_Surface *dst;
    int blurLevel;
    int begin;
    int end;
}blurArgs;

void *blurWorker(void *myArgs)
{
    blurArgs *args = (blurArgs *)myArgs;

    Color *currentColor;
    for(int i = args->begin; i < args->end; i++)
    {
        for(int j = 0; j < args->src->h; j++)
        {
            currentColor = getAverage(args->src, i, j, args->blurLevel);
            putPixel(args->dst, i, j, currentColor->pixel);
            freeColor(currentColor);
        }
    }

    pthread_exit(NULL);
}

void blur(SDL_Surface **image, int blurLevel)
{
    // copy image to result;
    SDL_Surface *result = SDL_ConvertSurface(*image, (*image)->format, SDL_SWSURFACE);

    // create thread pool
    int nbThreads = 4;
    blurArgs threadPool[nbThreads];
    int widthSlice = (*image)->w / nbThreads;
    for(int i = 0; i < nbThreads; i++)
    {
        threadPool[i].src = *image;
        threadPool[i].dst = result;
        threadPool[i].blurLevel = blurLevel;
        threadPool[i].begin = widthSlice * i;
        threadPool[i].end = widthSlice * (i + 1);
        pthread_create(&threadPool[i].id, NULL, blurWorker, &threadPool[i]);
    }
    /*
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
    */
    for(int i = 0; i < nbThreads; i++)
        pthread_join(threadPool[i].id, NULL);

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


void keepColorAndSave(char *filenameSource, char *filenameDest, RGB c, double threshold)
{
    int formats = IMG_INIT_JPG | IMG_INIT_PNG;
    int imageInit = IMG_Init(formats);
    if((imageInit&formats) != formats)
        errx(1, "Couldnt init SDL_Image");

    SDL_Surface *image;

    image = IMG_Load(filenameSource);
    if(image == NULL)
        errx(1, "Couldnt load image");

    printf("Filtering %s 1/2...\n", filenameDest);

    Color *color = initColor(image->format);
    setRGB(color, c.r, c.g, c.b);
    Color *black = initColor(image->format);
    setRGB(black, 0, 0, 0);

    keepTopoLineHSV(image, color, threshold);

    printf("Filtering %s 2/2...\n", filenameDest);
    setMonochromatic(image, black);
    removeIsolatedPixels(image);
    thickenColor(image, black);

    printf("Filtering done\n");
    freeColor(color);
    freeColor(black);

    SDL_SaveBMP(image, filenameDest);

    SDL_FreeSurface(image);
}

void FindAllExtremityAndSave(char *filenameSource, char *filenameDest)
{
    int formats = IMG_INIT_JPG | IMG_INIT_PNG;
    int imageInit = IMG_Init(formats);
    if((imageInit&formats) != formats)
        errx(1, "Couldnt init SDL_Image");

    SDL_Surface *image;

    image = IMG_Load(filenameSource);
    if(image == NULL)
        errx(1, "Couldnt load image");

    printf("Fixing lines...\n");

    FindAllExtremity(image);

    printf("Fixing done\n");
 
    SDL_SaveBMP(image, filenameDest);

    SDL_FreeSurface(image);

}

void makeHeightMap(char *filenameSource, char* filenameDest)
{
    printf("Making height map...\n");

    int formats = IMG_INIT_JPG | IMG_INIT_PNG;
    int imageInit = IMG_Init(formats);
    if((imageInit&formats) != formats)
        errx(1, "Couldnt init SDL_Image");

    SDL_Surface *image;

    image = IMG_Load(filenameSource);
    if(image == NULL)
        errx(1, "Couldnt load image");

    int nbColors = colorAllZonesFromCircles(image);
    printf("Normalizing...\n");
    normalize(image, nbColors);
    
    printf("Addind details at borders...\n");
    int blurValue = 10;
    SDL_Surface *vImage = addVBorders(image, blurValue);
    SDL_Surface *hImage = addHBorders(image, blurValue);
    averageImages(vImage, hImage);
    SDL_FreeSurface(vImage);
    SDL_FreeSurface(image);
    image = hImage;

    printf("Bluring...\n");
    blur(&image, 2);

    printf("Height map done\n");
    SDL_SaveBMP(image, filenameDest);

    SDL_FreeSurface(image);
}
