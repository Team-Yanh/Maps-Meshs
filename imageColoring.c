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
            setPixel(currentColor, getPixel(image, x, y));
            if(!isSameColor(circle, currentColor))
            {
                colorZoneBFS(image, circle, x, y);
                replaceColor(image, blackMarked, circle);
            }

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

int colorRivers(SDL_Surface *image)
{
    SDL_Surface *riverImage = IMG_Load("images/river.bmp");
    if(riverImage == NULL) // river.bmp doesnt exist
        return 0;

    Color *currentColor = initColor(image->format);
    Color *riverColor = initColor(image->format);
    Color *white = initColor(image->format);
    Color *black = initColor(image->format);
    Color *blackMarked = initColor(image->format);
    setRGB(black, 0, 0, 0);
    setRGB(white, 255, 255, 255);
    setRGB(riverColor, 3, 3, 3);
    setRGB(blackMarked, 0, 255, 0);

    int nbRivers = 0;
    Queue *q = createQueue();

    // check a rectangle around the border
    
    // check 3 pixel heigh top and bottom lines
    for(int i = 0; i < image->w; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            setPixel(currentColor, getPixel(riverImage, i, j)); // top row
            if(isSameColor(black, currentColor)) // On a river
                enqueue(q, i, j);

            setPixel(currentColor, getPixel(riverImage, i, riverImage->h - 1 - j)); // bottom row
            if(isSameColor(black, currentColor))
                enqueue(q, i, riverImage->h - 1 - j);
        }
    }
    // check 3 pixel wide right and left columns
    for(int i = 0; i < image->h; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            setPixel(currentColor, getPixel(riverImage, j, i)); // left column
            if(isSameColor(black, currentColor)) // On a river
                enqueue(q, j, i);
                

            setPixel(currentColor, getPixel(riverImage, riverImage->w - 1 - j, i)); // right col
            if(isSameColor(black, currentColor))
                enqueue(q, riverImage->w - 1 - j, i);
        }
    }

    int x = 0;
    int y = 0;

    if(!isEmpty(q))
    {
        while(!isEmpty(q))
        {
            dequeue(q, &x, &y);
            //printf("River at (x,y): (%d, %d)\n", x, y);
            setPixel(currentColor, getPixel(image, x, y));
            if(!isSameColor(riverColor, currentColor))
            {
                nbRivers++;
                colorZoneBFS(image, riverColor, x, y);
                replaceColor(image, blackMarked, riverColor);
            }
        }
    }

    freeQueue(q);
    freeColor(currentColor);
    freeColor(riverColor);
    freeColor(white);
    freeColor(black);
    freeColor(blackMarked);

    return nbRivers;
}


int colorAllZonesFromCircles(SDL_Surface *image)
{
    int nbColors = 1;

    printf("Finding summits...\n");

    Color *currentColor = initColor(image->format);
    Color *circle = initColor(image->format);
    Color *c = initColor(image->format);
    Color *river = initColor(image->format);
    Color *riverUp = initColor(image->format);
    Color *white = initColor(image->format);
    Color *black = initColor(image->format);
    Color *blackMarked = initColor(image->format);
    setRGB(black, 0, 0, 0);
    setRGB(blackMarked, 0, 255, 0);
    setRGB(circle, 254, 254, 254);
    setRGB(c, 253, 253, 253);
    setRGB(river, 3, 3, 3);
    setRGB(riverUp, 4, 4, 4);
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

    int nbRivers = colorRivers(image);
    if(nbRivers > 0)
        nbColors++;


    printf("Coloring...\n");

    int circleDrawn = 0;
    int riverDrawn = 0;

    while(isWhiteInImage(image)) //while we can, color adjacent circles with c or riverUp
    {
        circleDrawn = 0;
        riverDrawn = 0;
        for(int i = 0; i < image->w; i++)
        {
            for(int j = 0; j < image->h; j++)
            {
                // color adjacent circle colors (coloring downward)
                setPixel(currentColor, getPixel(image, i, j));
                if(isSameColor(circle, currentColor))
                {
                    if(isValidCellAndColor(image, i-1, j, white))
                    {
                        circleDrawn = 1;
                        colorZoneBFS(image, c, i-1, j);
                        replaceColor(image, blackMarked, c);
                    }
                    if(isValidCellAndColor(image, i+1, j, white))
                    {
                        circleDrawn = 1;
                        colorZoneBFS(image, c, i+1, j);
                        replaceColor(image, blackMarked, c);
                    }
                    if(isValidCellAndColor(image, i, j-1, white))
                    {
                        circleDrawn = 1;
                        colorZoneBFS(image, c, i, j-1);
                        replaceColor(image, blackMarked, c);
                    }
                    if(isValidCellAndColor(image, i, j+1, white))
                    {
                        circleDrawn = 1;
                        colorZoneBFS(image, c, i, j+1);
                        replaceColor(image, blackMarked, c);
                    }
                }

                // color adjacent river colors (coloring upward)
                setPixel(currentColor, getPixel(image, i, j));
                if(isSameColor(river, currentColor))
                {
                    if(isValidCellAndColor(image, i-1, j, white))
                    {
                        riverDrawn = 1;
                        colorZoneBFS(image, riverUp, i-1, j);
                        replaceColor(image, blackMarked, riverUp);
                    }
                    if(isValidCellAndColor(image, i+1, j, white))
                    {
                        riverDrawn = 1;
                        colorZoneBFS(image, riverUp, i+1, j);
                        replaceColor(image, blackMarked, riverUp);
                    }
                    if(isValidCellAndColor(image, i, j-1, white))
                    {
                        riverDrawn = 1;
                        colorZoneBFS(image, riverUp, i, j-1);
                        replaceColor(image, blackMarked, riverUp);
                    }
                    if(isValidCellAndColor(image, i, j+1, white))
                    {
                        riverDrawn = 1;
                        colorZoneBFS(image, riverUp, i, j+1);
                        replaceColor(image, blackMarked, riverUp);
                    }
                }

            }
        }

        if(circleDrawn)
            nbColors++;
        if(riverDrawn)
            nbColors++;
        
        int colorDiff = 1;

        setRGB(circle, c->rgb->r, c->rgb->g, c->rgb->b);
        setRGB(c, c->rgb->r - colorDiff, c->rgb->g - colorDiff, c->rgb->b - colorDiff);

        setRGB(river, riverUp->rgb->r, riverUp->rgb->g, riverUp->rgb->b);
        setRGB(riverUp, riverUp->rgb->r + colorDiff, riverUp->rgb->g + colorDiff, riverUp->rgb->b + colorDiff);
    }

    freeColor(currentColor);
    freeColor(circle);
    freeColor(c);
    freeColor(river);
    freeColor(riverUp);
    freeColor(white);
    freeColor(black);
    freeColor(blackMarked);

    printf("nbColors : %d\n", nbColors);
    return nbColors;
}

void normalize(SDL_Surface *image, int nbColors)
{
    printf("Normalizing...1/2\n");
    int step = 255/nbColors;

    Color *c = initColor(image->format);
    Color *currentColor = initColor(image->format);
    setRGB(c, 1, 1, 1);
    setRGB(currentColor, 3, 3, 3);
    // start at: 1,1,1

    while(currentColor->rgb->r < 255) // squash all colors so they are all adjacent
    {
        if(replaceColor(image, currentColor, c))
            setRGB(c, c->rgb->r + 1, c->rgb->g + 1, c->rgb->b + 1);
        setRGB(currentColor, currentColor->rgb->r + 1, currentColor->rgb->g + 1, currentColor->rgb->b + 1);
    }

    setRGB(currentColor, nbColors, nbColors, nbColors);
    setRGB(c, 255, 255, 255);

    // color from top to bottom
    printf("Normalizing... 2/2\n");
    for(int i = 0; i < nbColors; i++)
    {
        replaceColor(image, currentColor, c);

        setRGB(currentColor, currentColor->rgb->r - 1, currentColor->rgb->g - 1, currentColor->rgb->b - 1);
        setRGB(c, c->rgb->r - step, c->rgb->g - step, c->rgb->b - step);
    }

    freeColor(currentColor);
    freeColor(c);
}

void averageImages(SDL_Surface *imageSource, SDL_Surface *imageDest)
{
    assert(imageSource->w == imageDest->w);
    assert(imageSource->h == imageDest->h);

    Color *c = initColor(imageSource->format);
    Color *c2 = initColor(imageSource->format);
    Color *average = initColor(imageSource->format);

    for(int i = 0; i < imageSource->w; i++)
    {
        for(int j = 0; j< imageSource->h; j++)
        {
            setPixel(c, getPixel(imageSource, i, j));
            setPixel(c2, getPixel(imageDest, i, j));
            averageColors(c, c2, average);
            putPixel(imageDest, i, j, average->pixel);
        }
    }

    freeColor(c);
    freeColor(c2);
    freeColor(average);
}


static int findNextColorH(SDL_Surface *image, Color *c, int x, int y)
{
    int newX = x + 1;
    Color *currentColor = initColor(image->format);
    while(newX < image->w)
    {
        setPixel(currentColor, getPixel(image, newX, y));
        if(!isSameColor(currentColor, c))
            break;
        newX++;
    }
    freeColor(currentColor);
    return newX;
}

void drawHLine(SDL_Surface *image, Color *c, int x, int y, int x2, int y2)
{
    assert(y >= 0 && y < image->h);
    assert(y == y2);

    if(x < 0)
        x = 0;
    if(x > image->w)
        x = image->w;
    if(x2 < 0)
        x2 = 0;
    if(x2 > image->w)
        x2 = image->w;
    
    assert(x <= x2);
    
    for(int i = x; i < x2; i++)
    {
        putPixel(image, i, y, c->pixel);
    }
}

SDL_Surface *addHBorders(SDL_Surface *image, int width)
{
    SDL_Surface *newImage = SDL_CreateRGBSurface(0, image->w, image->h, 32, 0, 0, 0, 0);
    SDL_BlitSurface(image,NULL,newImage,NULL);

    Color *currentColor = initColor(image->format);
    Color *nextColor = initColor(image->format);
    Color *averageColor = initColor(image->format);
    int x = 0;
    int nextColorI = 1;

    for(int y = 0; y < image->h; y++) // for each line
    {
        x = 0;
        setPixel(currentColor, getPixel(image, x, y));
        
        while( (nextColorI = findNextColorH(image, currentColor, x, y)) < image->w)
        {
            setPixel(nextColor, getPixel(image, nextColorI, y));
            averageColors(currentColor, nextColor, averageColor);
            drawHLine(newImage, averageColor, nextColorI - width, y, nextColorI + width, y);
            x = nextColorI + width;

            setPixel(currentColor, nextColor->pixel);
        }
    }

    freeColor(currentColor);
    freeColor(nextColor);
    freeColor(averageColor);

    return newImage;
}

static int findNextColorV(SDL_Surface *image, Color *c, int x, int y)
{
    int newY = y + 1;
    Color *currentColor = initColor(image->format);
    while(newY < image->h)
    {
        setPixel(currentColor, getPixel(image, x, newY));
        if(!isSameColor(currentColor, c))
            break;
        newY++;
    }

    freeColor(currentColor);
    return newY;
}

void drawVLine(SDL_Surface *image, Color *c, int x, int y, int x2, int y2)
{
    assert(x >= 0 && x < image->w);
    assert(x == x2);

    if(y < 0)
        y = 0;
    if(y > image->h)
        y = image->h;
    if(y2 < 0)
        y2 = 0;
    if(y2 > image->h)
        y2 = image->h;
    
    assert(y <= y2);
    
    for(int i = y; i < y2; i++)
    {
        putPixel(image, x, i, c->pixel);
    }
}

SDL_Surface *addVBorders(SDL_Surface *image, int height)
{
    SDL_Surface *newImage = SDL_CreateRGBSurface(0, image->w, image->h, 32, 0, 0, 0, 0);
    SDL_BlitSurface(image,NULL,newImage,NULL);

    Color *currentColor = initColor(image->format);
    Color *nextColor = initColor(image->format);
    Color *averageColor = initColor(image->format);
    int y = 0;
    int nextColorI = 1;

    for(int x = 0; x < image->w; x++) // for each column
    {
        y = 0;
        setPixel(currentColor, getPixel(image, x, y));
        
        while( (nextColorI = findNextColorV(image, currentColor, x, y)) < image->h)
        {
            setPixel(nextColor, getPixel(image, x, nextColorI));
            averageColors(currentColor, nextColor, averageColor);
            drawVLine(newImage, averageColor, x, nextColorI - height, x, nextColorI + height);
            y = nextColorI + height;

            setPixel(currentColor, nextColor->pixel);
        }
    }

    freeColor(currentColor);
    freeColor(nextColor);
    freeColor(averageColor);

    return newImage;
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