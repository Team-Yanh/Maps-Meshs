#include "imageLinear.h"

int CalculY(Point *p1,Point *p2,int x3)
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
    int x3,y3;
    if (p1->x < p2->x)
    {
        x3 = p1->x +1;
        while(x3 < p2->x)
        {
            y3 = CalculY(p1,p2,x3);
            putPixel(image , x3,y3,black->pixel);
            putPixel(image , x3,y3+1,black->pixel);
            putPixel(image , x3,y3-1,black->pixel);
            x3 ++;
        }    
    }
    else 
    {
        x3 = p2->x +1;
        while(x3 < p1->x)
        {
        y3 = CalculY(p1,p2,x3);
            putPixel(image , x3,y3,black->pixel);
            putPixel(image , x3,y3+1,black->pixel);
            putPixel(image , x3,y3-1,black->pixel);
            x3 ++;
        }
    }
    freeColor(black);
}
