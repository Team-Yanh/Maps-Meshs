#include "CompleteLine.h"
int iswhite(SDL_Surface *surface, int x, int y)
{
    if (!(x < 0 || x >= surface->w || y < 0 || y >= surface->h))
    {
	Uint32 pixel = getPixel(surface,x,y);
        SDL_PixelFormat *format = surface->format;
        Uint8 r;
        Uint8 g;
        Uint8 b;
        SDL_GetRGB(pixel,format,&r,&g,&b);
	if(r == 255 && g == 255 && b == 255)
		return 1;
	return 0;
    }
    return 1;
}

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
    size_t width = image->w;
    size_t height = image->h;
    struct vector *ListEx = vector_new(1);
    SDL_PixelFormat *format = image->format;
    int c = 0;
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
                FindExtremity(image,ListEx,p,c);
                c = (c+1)% 256;
           }
        }
    }

    LinkExtremity(image,ListEx);
    vector_free(ListEx);
    //LinkExtremity(image,ListEx);
}

void FindExtremity(SDL_Surface *image,struct vector *v,Point point,int color)
{
    //init la queue et enqueu le point de depart
    Queue *q = createQueue();
    enqueue(q,point.x,point.y);
    int isEx ,x2 , y2, nbadjnm,nb,nbadj;
    nb= 0;
    Color *red = initColor(image->format);
    setRGB(red,255,0,color);
    Color *mark= initColor(image->format);
    setRGB(mark,1,255,1);


    //parcours largeur sur tous les pixels noir 
    while(!isEmpty(q))
    {
        nb ++;
	nbadj = 0;
        nbadjnm =0;
        isEx = 1;
        dequeue(q,&point.x,&point.y);
        for(int j = -1; j<=1;j++)
        {
            x2 = point.x;
            y2 = j + point.y;
            if(isMarkedCell(image,x2,y2))
            {
                nbadjnm ++;
                putPixel(image , x2,y2,mark->pixel);
                isEx = 0;
                enqueue(q,x2,y2);
            }

	    if(!iswhite(image,x2,y2) && j!=0)
	    {
		nbadj++;
	    }
        }
        for(int i = -1;i <= 1; i++)
        {
            x2 = i + point.x;
            y2 = point.y;
            if(isMarkedCell(image,x2,y2))
            {
                nbadjnm++;
                putPixel(image , x2,y2,mark->pixel);
                isEx = 0;
                enqueue(q,x2,y2);
            }
	    if(!iswhite(image,x2,y2) && i!= 0)
	    {
		    nbadj++;
	    }
        }

        if((isEx == 1 && nbadj == 1)|| (nb == 1 && nbadjnm == 1))
        {
            if(!(point.x == 0 || point.x == image->w - 1 || point.y == 0 || point.y == image->h - 1))
            {
                Point *newex = malloc(sizeof(Point));
                newex->x = point.x;
                newex->y = point.y;
                vector_push(v,newex);
                putPixel(image,newex->x,newex->y,red->pixel); 
            }
        }
    }
    //marque = plus noir(plus exactement)
    freeColor(mark);
    //recupere les deux ou plus extremite dans v
    freeColor(red);
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
    //putPixel(image , x1,y1,black->pixel);
    while(x1 != x2 || y1 != y2)
    {
        if(x1 > x2)
        {
            putPixel(image , x1,y1,black->pixel);
            x1 --;
        }
        else 
        {
            if(x1 != x2)
            {
                putPixel(image,x1,y1,black->pixel);
                x1 ++;
            }
        }
        if(y1 > y2)
        {
            putPixel(image , x1,y1,black->pixel);
            y1 --;
        }
        else 
        {
            if(y1 != y2)
            {
                putPixel(image,x1,y1,black->pixel);
                y1 ++;
            }
        }
        
    }
    if(isValidCell(image,x1,y1))
        putPixel(image , x1,y1,black->pixel);
    freeColor(black);

}
Point *closestPoint(SDL_Surface *image,struct vector* ListEx,Point *Ex,int *clsip)
{
    SDL_PixelFormat *format = image->format;
    Uint32 ExPixel = getPixel(image,Ex->x,Ex->y);
    Uint8 r;
    Uint8 g;
    Uint8 b;
    SDL_GetRGB(ExPixel,format,&r,&g,&b);

    int clsi = -1;
    int treshold = 30;

    Point clo;
    void *temp;
    Point *temp2;

    Uint32 clsPixel;
    clo = ClosestWall(image,Ex);
    Point *closest = &clo;
    if (Distance(Ex,closest) < treshold)
    {
        clsi = -2;
    }

    size_t i = 1;


    while(i <= ListEx->size )
    {
        vector_get(ListEx,i,&temp);
        temp2 = (Point *)temp;
        clsPixel = getPixel(image,temp2->x,temp2->y);
        Uint8 r2;
        Uint8 g2;
        Uint8 b2;
        SDL_GetRGB(clsPixel,format,&r2,&g2,&b2);
        if(Distance(closest,Ex)>Distance(Ex,temp2) && Distance(Ex,temp2) < treshold && (r != r2 || g != g2 || b != b2))
        {
            closest = temp2;
            clsi = i;
        }
        i++;
    }
    *clsip = clsi;

    return closest;
}
void LinkExtremity(SDL_Surface *image,struct vector* ListEx)
{ 
    //prendre un elem de la liste
    void *Ex;
    void *temp;
    Point *closest2;
    Point *closest ;
    Point *Ex2;
    int draw = 0;
    int i ;
    int clsi = -1;
    int clsi2;
    while(ListEx->size > 1)
    {
        i = 1;
        vector_get(ListEx,i,&Ex);
        Ex2 = (Point *)Ex;
        closest = closestPoint(image,ListEx,Ex2,&clsi);
        while (!draw)
        {    
            closest2 = closestPoint(image,ListEx,closest,&clsi2);
            if(closest2 != Ex2)
            {
                Ex2 = closest;
                closest = closest2;
                i = clsi;
                clsi = clsi2; 
            }
            else
            {

                draw = 1;
            }
        }
        
        if(clsi != -1)
        {

            DrawLine(image,Ex2,closest);
            if(clsi != -2)
            {
                vector_remove(ListEx,clsi,&temp);
                free(temp);
            }
        }
        vector_remove(ListEx,i,&Ex);
        free(Ex);
    }
    //chercher le point le plus proche a une distance x et min 
    //relier les deux points 
}

