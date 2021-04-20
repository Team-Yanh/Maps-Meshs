#include <gtk/gtk.h>
#include <cairo.h>
#include "uiTreatment.h"
#include "uiColorPick.h"
#include "uiDraw.h"

void remove_paint_signal(DrawManagement* dm)
{
    if (dm->paint_id > 0)
        g_signal_handler_disconnect(dm->ebox, dm->paint_id);

    dm->paint_id = 0;
}

void add_paint_signal(DrawManagement* dm)
{
    if (dm->pb != NULL && dm->paint_id == 0)
        dm->paint_id = g_signal_connect(dm->ebox, "button_press_event",
                G_CALLBACK(paint), dm);
}

void switch_paint_signal(DrawManagement* dm)
{
    if (dm->paint_id == 0)
    {
        add_paint_signal(dm);
        remove_pick_signal(dm);
    }
    else
        remove_paint_signal(dm);
}

void on_paint_btn_clicked(unused GtkButton* b, gpointer user_data)
{
    UserInterface* ui = user_data;

    switch_paint_signal(&ui->draw_left);
    switch_paint_signal(&ui->draw_right);
}

void put_color(guchar* pixel, RGB color)
{
    pixel[0] = color.r;
    pixel[1] = color.g;
    pixel[2] = color.b;
}

void paint(unused GtkEventBox* ebox, GdkEventButton* event, gpointer user_data)
{
    DrawManagement* dm = user_data;
    guchar* pixel = NULL;
    int n_channels = gdk_pixbuf_get_n_channels(dm->pb);
    int rowstride = gdk_pixbuf_get_rowstride(dm->pb);
    double zoom = 1;
    Zone zone;
    int ex = event->x, ey = event->y;

    // - Gets the middle pixel
    pixel = get_clicked_pixel(dm, ex, ey);

    if (pixel != NULL)
    {
        // - Gets the zoom value
        zoom = gtk_adjustment_get_value(dm->zoom);
        ex /= zoom;
        ey /= zoom;

        // - Inits the zone to be colored
        zone.size = 10;
        zone.init.x = ex - zone.size;
        zone.init.y = ey - zone.size;
        zone.offSet.x = 0;
        zone.offSet.y = 0;
        zone.w = zone.size * 2 + 1;
        zone.h = zone.size * 2 + 1;

        // - Gets the starting coloring column
        if (zone.init.x < 0)
        {
            zone.init.x = 0;
            zone.offSet.x = zone.size - ex;
            zone.w -= zone.offSet.x;
        }

        // - Gets the starting coloring line
        if (zone.init.y < 0)
        {
            zone.init.y = 0;
            zone.offSet.y = zone.size - ey;
            zone.h -= zone.offSet.y;
        }

        // - Sets the zone indices position
        zone.ind = zone.init;

        // - Gets the top left pixel of the zone to be colored
        pixel -= (zone.size - zone.offSet.y) * rowstride +
            (zone.size - zone.offSet.x) * n_channels;

        // - Tries to paint each pixel of the zone of size "size"
        while (zone.ind.y < zone.init.y + zone.h && zone.ind.y < dm->h)
        {
            while (zone.ind.x < zone.init.x + zone.w && zone.ind.x < dm->w)
            {
                put_color(pixel, dm->color);

                // - Goes to the next pixel of the line
                pixel += n_channels;
                zone.ind.x++;
            }

            // - Reset pixel beginning column
            pixel -= (zone.ind.x - zone.init.x) * n_channels;

            // - Resets x values
            zone.ind.x = zone.init.x;

            // - Goes to the next pixel line
            pixel += rowstride;
            zone.ind.y++;
        }

        // - ReCompute positions count of the zoom of the redraw
        zone.init.x = (ex - zone.size + zone.offSet.x) * zoom;
        zone.init.y = (ey - zone.size + zone.offSet.y) * zoom;

        // - Request draw signal
        gtk_widget_queue_draw_area(GTK_WIDGET(dm->darea),
                zone.init.x, zone.init.y, zone.w * zoom,
                zone.h * zoom);
    }
}

void on_draw(unused GtkDrawingArea* darea, cairo_t *cr, gpointer user_data)
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
