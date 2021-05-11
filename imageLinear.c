#include "imageLinear.h"

int calculY(Point *p1,Point *p2,int x3)
{
    return p1->y + ((p2->y - p1->y)/ (p2->x - p1->x)) * (x3 - p1->x);
}

int CalculYpoly(struct vector *v,int x3)
{
    int s,t;
    int k = 0;
    void *vi;
    Point *pi;
    void *vj;
    Point *pj;
    for (size_t i = 0;i < v->size;i ++)
    {
        s = 1;
        t = 1;
        vector_get(v,i,&vi);
        pi = (Point *)vi;
        for (size_t j = 0; i < v->size; i++)
        {
            if (j != i )
            {
                vector_get(v,i,&vj);
                pj = (Point *)vj;
                s = s * (x3 - pj->x);
                t = t * (pi->x - pj->x);
            }
        }
        k = k+ ((s/t) * pi->y);
    }
    return k;
}
void interpol(SDL_Surface *image,Point *p1,Point *p2)
{
    Color *black= initColor(image->format);
    setRGB(black,0,0,0);

    int x3,y3,yav,borne,borney;
     if (p1->x < p2->x)
    {
        x3 = p1->x + 1;
        borne = p2->x;
        yav = p1->y;
    }

    else
    {
        x3 = p2->x + 1;
        borne = p1->x;
        yav=p2->y;
    }

    while(x3 < borne)
    {
        y3 = calculY(p1,p2,x3);

        putPixel(image , x3,y3,black->pixel);

        if (yav > y3)
        {
            borney = yav;
            yav = y3;
        }
        else 
        {
            borney = y3;
        }
        while (yav < borney)
        {
            putPixel(image,x3,yav,black->pixel);
            printf("%d",yav);
            yav ++;
        }
        /*
        if(yav > y3)
        {

            while (yav > y3)
            {
                putPixel(image,x3,yav,black->pixel);
                yav --;
            }
        }   
        else
        {
            while (yav < y3)
            {
                putPixel(image,x3,yav,black->pixel);
                yav++;
            }
        }
        */
        yav = y3;

        x3 ++;
    }  
    freeColor(black);
}
