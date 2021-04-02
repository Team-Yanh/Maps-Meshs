#include "CompleteLine.h"


int isMarkedCell(SDL_Surface *surface, int x, int y)
{
    Uint32 pixel = getPixel(surface,x,y);
    SDL_PixelFormat *format = surface->format;
    Uint8 r;
    Uint8 g;
    Uint8 b;
    SDL_GetRGB(pixel,format,&r,&g,&b);
    if (x < 0 || x >= surface->w || y < 0 || y >= surface->h || r != 0)
        return 0;
    return 1; 
}

void FindAllExtremity(SDL_Surface *image)
{
    Color *red = initColor(image->format);
    setRGB(red,255,0,0);
    size_t width = image->w;
    size_t height = image->h;
    struct vector *ListEx = vector_new(1);
    SDL_PixelFormat *format = image->format;

    for (size_t i = 0 ; i < width;i ++)
    {
        for(size_t j = 0; j < height; j ++)
        {
            Uint32 pixel = getPixel(image,i,j);
            Uint8 r;
            Uint8 g;
            Uint8 b;
            Point p = { i , j };
            SDL_GetRGB(pixel,format,&r,&g,&b);
            if(r == 0 && g == 0 && b == 0)
                FindExtremity(image,ListEx,p);
        }
    }

    for(size_t i = 0; i < ListEx->size; i++)
    {
        Point *Ex = *(ListEx->data + i);
        putPixel(image,Ex->x,Ex->y,red->pixel); 
    }
    vector_free(ListEx);

    //LinkExtremity(image,ListEx);
}

void FindExtremity(SDL_Surface *image,struct vector *v,Point point)
{
    //init la queue et enqueu le point de depart
    Queue *q = createQueue();
    enqueue(q,point.x,point.y);
    int isEx ;
    Color *mark= initColor(image->format);
    setRGB(mark,0,255,0);
    //parcours largeur sur tous les pixels noir 
    while(!isEmpty(q))
    {
        isEx = 1;
        dequeue(q,&point.x,&point.y);
        for(int i = -1;i <= 1; i++)
        {
            for(int j = -1; j<=1;j++)
            {
                int x2 = i + point.x;
                int y2 = j + point.y;
                if(isMarkedCell(image,x2,y2))    
                {
                    //marque le pixel
                    putPixel(image , x2,y2,mark->pixel);
                    isEx = 0;
                    enqueue(q,x2,y2);        
                }
            }
        }
        
        if(isEx)
        {
            Point *newex = malloc(sizeof(Point));
            newex->x = point.x;
            newex->y = point.y;
            vector_push(v,newex);
        }
    }
    //marque = plus noir(plus exactement)
    freeColor(mark);
    //recupere les deux ou plus extremite dans v
    //free tout ca
}
/*
void LinkExtremity(SDL_Surface *image,struct vector* ListEx)
{ 
    //TODO //prendre un elem de la liste
    //chercher le point le plus proche a une distance x et min 
    //relier les deux points 
}
*/
