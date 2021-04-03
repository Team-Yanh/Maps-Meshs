#include "CompleteLine.h"


int isMarkedCell(SDL_Surface *surface, int x, int y)
{
    if (!(x < 0 || x >= surface->w || y < 0 || y >= surface->h))
    {
        Uint32 pixel = getPixel(surface,x,y);
        SDL_PixelFormat *format = surface->format;
        Uint8 r;
        Uint8 g;
        Uint8 b;
        SDL_GetRGB(pixel,format,&r,&g,&b);
        if(r == 0)
            return 1;
    }
    return 0; 
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
           {
                FindExtremity(image,ListEx,p);
           }
        }
    }

    for(size_t i = 0; i < ListEx->size; i++)
    {
        Point *Ex = *(ListEx->data + i);
        putPixel(image,Ex->x,Ex->y,red->pixel); 
    }
    vector_free(ListEx);
    freeColor(red);
    //LinkExtremity(image,ListEx);
}

void FindExtremity(SDL_Surface *image,struct vector *v,Point point)
{
    //init la queue et enqueu le point de depart
    Queue *q = createQueue();
    enqueue(q,point.x,point.y);
    int isEx ,x2 , y2, nbadj,nb;
    nb= 0;
    Color *mark= initColor(image->format);
    setRGB(mark,1,255,1);
    //parcours largeur sur tous les pixels noir 
    while(!isEmpty(q))
    {
        nb ++;
        nbadj =0;
        isEx = 1; 
        dequeue(q,&point.x,&point.y);
        putPixel(image , point.x,point.y,mark->pixel);
        for(int j = -1; j<=1;j++)
        {
            x2 = point.x;
            y2 = j + point.y;
            if(isMarkedCell(image,x2,y2))    
            {
                nbadj ++;
                putPixel(image , x2,y2,mark->pixel);
                isEx = 0;
                enqueue(q,x2,y2);        
            }
        }
        for(int i = -1;i <= 1; i++)
        {
            x2 = i + point.x;
            y2 = point.y;
            if(isMarkedCell(image,x2,y2))    
            {
                //marque le pixel
                nbadj++;
                putPixel(image , x2,y2,mark->pixel);
                isEx = 0;
                enqueue(q,x2,y2);        
            }
        }
        if(isEx == 1 || (nb == 1 && nbadj == 1))
        {
            if(!(point.x == 0 || point.x == image->w - 1 || point.y == 0 || point.y == image->h - 1))
            {
                Point *newex = malloc(sizeof(Point));
                newex->x = point.x;
                newex->y = point.y;
                vector_push(v,newex);
            }
        }
    }
    //marque = plus noir(plus exactement)
    freeColor(mark);
    //recupere les deux ou plus extremite dans v
    freeQueue(q);
    //free tout ca
}

int Distance(Point *p1,Point *p2)
{
    double t1 = p2->x - p1->x;
    double t2 = p2->y - p1->y;
   return sqrt(pow(t1,2) + pow(t2,2)); 
}

Point ClosestWall(SDL_Surface *image,Point *Ex)
{
    Point cur ={0,Ex->y};
    Point closest = {image->w, Ex->y};
    int closestDir = Distance(Ex,&closest);
    if (Distance(&cur,Ex) < closestDir)
    {
        closestDir = Distance(&cur,Ex);
        closest = cur;
    }
    cur.x =  Ex->x;
    cur.y = 0;
    if (Distance(&cur,Ex) < closestDir)
    {
        closestDir = Distance(&cur,Ex);
        closest = cur;
    }
    cur.x = Ex->x;
    cur.y = image->h;
    if (Distance(&cur,Ex) < closestDir)
    {
        closestDir = Distance(&cur,Ex);
        closest = cur;
    } 
    return closest;
}

void DrawLine(SDL_Surface *image,Point *p1,Point *p2)
{
    Color *black= initColor(image->format);
    setRGB(black,0,0,0);
    int x1,x2,y1,y2;
    x1 = p1->x;
    x2 = p2->x;
    y1 = p1->y;
    y2 = p2->y;
    if(x1 > x2)
    {
        while(x1>x2)
        {
            putPixel(image , x1,y1,black->pixel);
            x1 --;
        }
    }
    else
    {
       while(x2>x1)
       {
           putPixel(image , x1,y1,black->pixel);
           x1 ++;
       }
    } 
    if(y1 > y2)
    {
        while(y1>y2)
        {
            putPixel(image , x1,y1,black->pixel);
            y1 --;
        }
    }
    else
    {
       while(y2>y1)
       {
            putPixel(image , x1,y1,black->pixel);
            y1 ++;
       }
    } 
    freeColor(black);

}
void LinkExtremity(SDL_Surface *image,struct vector* ListEx)
{ 
    //prendre un elem de la liste
    void *Ex;
    void *temp;
    Point *temp2;
    Point *closest ;

    Point *Ex2;
    size_t i ;
    int clsi = -1;
    while(ListEx->size != 0)
    {
        Point clo;
        vector_pop(ListEx,&Ex);
        Ex2 = (Point *)Ex;
        clo = ClosestWall(image,Ex2);
        closest = &clo;
        i = 1;
        while(i <= ListEx->size )
        {
            vector_get(ListEx,i,&temp);
            temp2 = (Point *)temp;
            if(Distance(closest,Ex2)>Distance(Ex2,temp2))
            {
                closest = temp2;
                clsi = i;
            }
            i++;
        }
        if(clsi != -1)
        {
            vector_remove(ListEx,clsi,&temp);
        }
        DrawLine(image,Ex,closest);
        free(Ex);
    }
    //chercher le point le plus proche a une distance x et min 
    //relier les deux points 
}

