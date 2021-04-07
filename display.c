#include "display.h"

void waitForClick()
{
    SDL_Event event;
    SDL_bool click = SDL_FALSE;
    while(!click)
    {
        SDL_WaitEvent(&event);
        if(event.type == SDL_MOUSEBUTTONDOWN)
            click = SDL_TRUE;
    }
}

void updateScreen(SDL_Surface *screen, SDL_Surface *image)
{
    SDL_Rect rcDest = {0, 0, 0, 0};
    SDL_BlitSurface(image, NULL, screen, &rcDest);
    SDL_Flip(screen);
    waitForClick();
}

void display_images()
{
    int formats = IMG_INIT_JPG | IMG_INIT_PNG;
    int imageInit = IMG_Init(formats);
    if((imageInit&formats) != formats)
        errx(1, "Couldnt init SDL_Image");

    SDL_Surface *image;

    image = IMG_Load("images/test1.png");
    if(image == NULL)
        errx(1, "Couldnt load image");
    SDL_Surface* screen = SDL_SetVideoMode(image->w, image->h, 32,
            SDL_HWSURFACE | SDL_DOUBLEBUF);
    

    Color *black = initColor(image->format);
    Color *white = initColor(image->format);
    Color *red = initColor(image->format);
    Color *green = initColor(image->format);
    Color *orange = initColor(image->format);
    Color *topoColor = initColor(image->format);
    setRGB(black, 0, 0, 0);
    setRGB(white, 255, 255, 255);
    setRGB(red, 255, 0, 0);
    setRGB(green, 0, 255, 0);
    setRGB(orange, 255, 165, 0);
    setRGB(topoColor, 217, 200, 170);    
    

    updateScreen(screen, image);
    keepTopoLineHSV(image, topoColor);
    updateScreen(screen, image);

    image = IMG_Load("images/test4.png");
    if(image == NULL)
        errx(1, "Couldnt load image");
    screen = SDL_SetVideoMode(image->w, image->h, 32,
            SDL_HWSURFACE | SDL_DOUBLEBUF);
    updateScreen(screen, image);

    image = IMG_Load("images/out.png");
    if(image == NULL)
        errx(1, "Couldnt load image");
    screen = SDL_SetVideoMode(image->w, image->h, 32,
            SDL_HWSURFACE | SDL_DOUBLEBUF);
    updateScreen(screen, image);

    //colorCircles(image);
    //updateScreen(screen, image);
    //colorAllZonesFromCircles(image);
    //updateScreen(screen, image);

    //colorCircles(image);
    //replaceColor(image, green, black);
    //colorAllZonesFromCircles(image);
    //setMonochromatic(image, red);
    //thickenColor(image, black);

    SDL_SaveBMP(image, "images/out.bmp");

    freeColor(black);
    freeColor(white);
    freeColor(red);
    freeColor(green);
    freeColor(orange);
    freeColor(topoColor);

    SDL_FreeSurface(image);
    SDL_FreeSurface(screen);
}


void display_images_hugo()
{
    int formats = IMG_INIT_JPG | IMG_INIT_PNG;
    int imageInit = IMG_Init(formats);
    if((imageInit&formats) != formats)
        errx(1, "Couldnt init SDL_Image");

    SDL_Surface *image;

    image = IMG_Load("images/test5.png");

    if(image == NULL)
        errx(1, "Couldnt load image");

    SDL_Surface* screen = SDL_SetVideoMode(image->w, image->h, 32,
            SDL_HWSURFACE | SDL_DOUBLEBUF);

    SDL_Rect rcDest = {0, 0, 0, 0};

    SDL_BlitSurface(image, NULL, screen, &rcDest);
    SDL_Flip(screen);

    waitForClick();

    FindAllExtremity(image);

    SDL_BlitSurface(image, NULL, screen, &rcDest);
    SDL_Flip(screen);
    
    waitForClick();

    SDL_SaveBMP(image, "images/out.bmp");

    SDL_FreeSurface(image);
    SDL_FreeSurface(screen);
}
