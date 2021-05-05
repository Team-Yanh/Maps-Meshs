#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <math.h>
#include <gtk/gtk.h>

#include <SDL/SDL.h>

#include "SDL/SDL_image.h"
#include "imageFilter.h"
#include "imageUtils.h"
#include "imageColoring.h"

#include "CompleteLine.h"
#include "indices.h"
#include "map.h"
#include "opengl.h"
#include "display.h"
#include "uiTreatment.h"

int main(void)
{
    // - Partie I [UI - Mathieu]
    uiTreatment();

    // - Partie II [Traitement - Tim]
    SDL_Init(SDL_INIT_VIDEO);
    display_images();

    // - Partie III [Traitement - Hugo]
    display_images_hugo();
    SDL_Quit();

    // - Partie IV [Rendu 3D - Nico]
    display_terrain();

    return 0;
}
