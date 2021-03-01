#include "display.h"

size_t findOptimalCellSize(grid *g, SDL_Surface *surface)
{
    //maximize the size of the cells according to surface width and height
    size_t max_horizontal_cell_size = surface->w / g->width;
    size_t max_vertical_cell_size = surface->h / g->height;

    if(max_horizontal_cell_size > max_vertical_cell_size)
        return max_vertical_cell_size;
    else
        return max_horizontal_cell_size;
}

void drawGrid(grid *g, SDL_Surface *surface)
{
    size_t cell_size = findOptimalCellSize(g, surface);
    int x = 0;
    int y = 0;

    SDL_Rect cell = {.x = x, .y = y,
        .w = cell_size, .h = cell_size};

    for(size_t i = 0; i < g->width; i++)
    {
        cell.x = i * cell_size;
        for(size_t j = 0; j < g->height; j++)
        {
            cell.y = j * cell_size;

            //padding 1px on each side of the rectangle
            cell.x += 1;
            cell.y += 1;
            cell.w -= 2;
            cell.h -= 2;

            SDL_FillRect(surface, &cell, SDL_MapRGB(surface->format,
                        255, 255, 255));
            //unpad
            cell.x -= 1;
            cell.y -= 1;
            cell.w += 2;
            cell.h += 2;
        }
    }
}
