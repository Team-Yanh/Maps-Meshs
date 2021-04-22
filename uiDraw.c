#include <gtk/gtk.h>
#include <cairo.h>
#include "uiTreatment.h"
#include "uiColorPick.h"
#include "uiDraw.h"

void add_paintable(DrawManagement* dm)
{
    if (dm->paintable_id == 0 && dm->pb != NULL)
        dm->paintable_id = g_signal_connect(dm->ebox, "button_press_event",
               G_CALLBACK(begin_paint), dm);
}

void remove_paintable(DrawManagement* dm)
{
    if (dm->paintable_id != 0)
        g_signal_handler_disconnect(dm->ebox, dm->paintable_id);

    dm->paintable_id = 0;
}

void add_paint(DrawManagement* dm)
{
    if (dm->paint_id == 0 && dm->pb != NULL)
        dm->paint_id = g_signal_connect(dm->ebox, "motion_notify_event",
                G_CALLBACK(paint), dm);
}

void remove_paint(DrawManagement* dm)
{
    if (dm->paint_id != 0)
        g_signal_handler_disconnect(dm->ebox, dm->paint_id);

    dm->paint_id = 0;
}

void add_release(DrawManagement* dm)
{
    if (dm->release_id == 0 && dm->pb != NULL)
        dm->release_id = g_signal_connect(dm->ebox, "button_release_event",
                G_CALLBACK(end_paint), dm);
}

void remove_release(DrawManagement* dm)
{
    if (dm->release_id != 0)
        g_signal_handler_disconnect(dm->ebox, dm->release_id);

    dm->release_id = 0;
}

void switch_paintable(DrawManagement* dm)
{
    if (dm->paintable_id == 0)
        add_paintable(dm);
    else
        remove_paintable(dm);
}

void on_paint_btn_clicked(unused GtkButton* b, gpointer user_data)
{
    UserInterface* ui = user_data;

    switch_paintable(&ui->draw_left);
    switch_paintable(&ui->draw_right);
}

void begin_paint(GtkEventBox* ebox, unused GdkEventButton* event, gpointer data)
{
    DrawManagement* dm = data;

    // - Paint au click
    paint(ebox, event, dm);

    // - Ajoute le signal pour peindre
    add_paint(dm);

    // - Ajoute le signal pour retirer la fonction paint quand on relache
    // - le bouton
    add_release(dm);
}

void end_paint(unused GtkEventBox* eb, unused GdkEventButton* ev, gpointer data)
{
    DrawManagement* dm = data;

    // - Retire le signal de release
    remove_release(dm);

    // - Retire le signal de paint
    remove_paint(dm);
}

static void put_color(guchar* pixel, RGB color)
{
    pixel[0] = color.r;
    pixel[1] = color.g;
    pixel[2] = color.b;
}

void paint(unused GtkEventBox* ebox, GdkEventButton* event, gpointer user_data)
{
    DrawManagement* dm = user_data;
    UserInterface* ui = dm->ui;
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
        zone.size = gtk_adjustment_get_value(ui->size) - 1;
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
