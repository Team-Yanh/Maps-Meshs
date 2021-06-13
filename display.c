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
    Uint32 bg = SDL_MapRGB(screen->format, 0, 0, 0);
    SDL_Rect rcDest = {0, 0, 0, 0};
    SDL_FillRect(screen, NULL, bg);
    SDL_BlitSurface(image, NULL, screen, &rcDest);
    SDL_Flip(screen);
    waitForClick();
}

void tempMain()
{
    int formats = IMG_INIT_JPG | IMG_INIT_PNG;
    int imageInit = IMG_Init(formats);
    if((imageInit&formats) != formats)
        errx(1, "Couldnt init SDL_Image");

    SDL_Surface *image = IMG_Load("images/valley.bmp");
    SDL_Surface *image2 = IMG_Load("images/checker2.png");
    if(image == NULL)
        errx(1, "Couldnt load image");
    SDL_Surface* screen = SDL_SetVideoMode(image->w, image->h, 32,
            SDL_HWSURFACE | SDL_DOUBLEBUF);

    Color *black = initColor(image->format);
    Color *red = initColor(image->format);
    setRGB(black, 0, 0, 0);
    setRGB(red, 255, 0, 0);

    int blurValue = 10;
    SDL_Surface *vImage = addVBorders(image, blurValue);
    SDL_Surface *hImage = addHBorders(image, blurValue);
    averageImages(vImage, hImage);
    SDL_FreeSurface(vImage);
    SDL_FreeSurface(image);
    updateScreen(screen, hImage);
    SDL_SaveBMP(hImage, "images/out.bmp");


    freeColor(black);
    freeColor(red);

    SDL_FreeSurface(image2);
    SDL_FreeSurface(screen);
    SDL_FreeSurface(hImage);
}

void display_images()
{
    int formats = IMG_INIT_JPG | IMG_INIT_PNG;
    int imageInit = IMG_Init(formats);
    if((imageInit&formats) != formats)
        errx(1, "Couldnt init SDL_Image");

    SDL_Surface *image;

    image = IMG_Load("images/test4.png");
    if(image == NULL)
        errx(1, "Couldnt load image");

    SDL_Surface* screen = SDL_SetVideoMode(image->w, image->h, 32,
            SDL_HWSURFACE | SDL_DOUBLEBUF);

    updateScreen(screen, image);

    SDL_FreeSurface(image);

    image = IMG_Load("images/out2.bmp");
    if(image == NULL)
        errx(1, "Couldnt load image");

    updateScreen(screen, image);

    SDL_FreeSurface(image);

    /*
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
    keepTopoLineHSV(image, topoColor, 1.0);
    updateScreen(screen, image);

    image = IMG_Load("images/test4.png");
    if(image == NULL)
        errx(1, "Couldnt load image");
    screen = SDL_SetVideoMode(image->w, image->h, 32,
            SDL_HWSURFACE | SDL_DOUBLEBUF);

    updateScreen(screen, image);

    SDL_FreeSurface(image);

    image = IMG_Load("images/out.png");
    if(image == NULL)
        errx(1, "Couldnt load image");
    screen = SDL_SetVideoMode(image->w, image->h, 32,
            SDL_HWSURFACE | SDL_DOUBLEBUF);

    updateScreen(screen, image);

    SDL_SaveBMP(image, "images/out.bmp");

    freeColor(black);
    freeColor(white);
    freeColor(red);
    freeColor(green);
    freeColor(orange);
    freeColor(topoColor);
    */
    SDL_FreeSurface(screen);
}


void display_images_hugo()
{
    int formats = IMG_INIT_JPG | IMG_INIT_PNG;
    int imageInit = IMG_Init(formats);
    if((imageInit&formats) != formats)
        errx(1, "Couldnt init SDL_Image");

    SDL_Surface *image;

    image = IMG_Load("images/test4cp.png");

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
