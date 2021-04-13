#include <gtk/gtk.h>
#include <cairo.h>
#include "uiTreatment.h"
#include "uiDraw.h"

void on_draw(unused GtkButton* b, cairo_t *cr, unused gpointer user_data)
{
    UserInterface* ui = user_data;

    // - Draw bg
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_fill(cr);

    if (ui->treated_pixbuf != NULL)
    {
        // - Load image
        gdk_cairo_set_source_pixbuf(cr, ui->treated_pixbuf, 0, 0);

        //- Draw image
        cairo_paint(cr);
    }
}
