#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include "grid.h"

size_t findOptimalCellSize(grid *g, SDL_Surface *surface);
void drawGrid(grid *g, SDL_Surface *surface);
