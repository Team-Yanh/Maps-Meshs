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

void paint(unused GtkEventBox* ebox, GdkEventButton* event, gpointer user_data)
{
    DrawManagement* dm = user_data;
    guchar* pixel = NULL;
    int size = 10;


    // - Gets the pixel
    pixel = get_clicked_pixel(dm, event->x, event->y);

    if (pixel != NULL)
    {
        // - Puts the color
        pixel[0] = dm->color.r;
        pixel[1] = dm->color.g;
        pixel[2] = dm->color.b;

        size *= gtk_adjustment_get_value(dm->zoom);

        // - Request draw signal
        gtk_widget_queue_draw_area(GTK_WIDGET(dm->darea),
                event->x - size / 2, event->y - size / 2, size, size);
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
