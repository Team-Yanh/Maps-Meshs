#include <gtk/gtk.h>
#include <cairo.h>
#include "uiTreatment.h"
#include "uiDraw.h"

void on_draw(unused GtkButton* b, cairo_t *cr, gpointer user_data)
{
    DrawManagement* dm = user_data;
    double zoom_value = 1;

    // - Draw bg
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_fill(cr);

    if (dm->pb != NULL)
    {
        // - Gets zoom value
        zoom_value = gtk_adjustment_get_value(dm->zoom);

        // - Triggers scrollbars
        gtk_widget_set_size_request(GTK_WIDGET(dm->darea), dm->w * zoom_value,
                dm->h * zoom_value);

        // - Scaling from zoom adjustment
        cairo_scale(cr, zoom_value, zoom_value);

        // - Load image
        gdk_cairo_set_source_pixbuf(cr, dm->pb, 0, 0);

        // - Draw image
        cairo_paint(cr);
    }
}
