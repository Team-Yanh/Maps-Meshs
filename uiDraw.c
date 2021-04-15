#include <gtk/gtk.h>
#include <cairo.h>
#include "uiTreatment.h"
#include "uiDraw.h"

void on_paint_btn_clicked(unused GtkButton* b, gpointer user_data)
{
    UserInterface* ui = user_data;

    if (ui->draw_left.pb != NULL)
    {
        if (ui->draw_left.paint_id == 0)
            ui->draw_left.paint_id = g_signal_connect(ui->draw_left.ebox,
                    "button_press_event", G_CALLBACK(paint), ui);
        else
        {
            g_signal_handler_disconnect(ui->draw_left.ebox, ui->draw_left.paint_id);
            ui->draw_left.paint_id = 0;
        }
    }

    if (ui->draw_right.pb != NULL)
    {
        if (ui->draw_right.paint_id == 0 && ui->draw_right.pb != NULL)
            ui->draw_right.paint_id = g_signal_connect(ui->draw_right.ebox,
                    "button_press_event", G_CALLBACK(paint), ui);
        else
        {
            g_signal_handler_disconnect(ui->draw_right.ebox,
                    ui->draw_right.paint_id);
            ui->draw_right.paint_id = 0;
        }
    }
}

void paint(GtkEventBox* ebox, GdkEventButton* event, gpointer user_data)
{
    UserInterface* ui = user_data;
    DrawManagement dm;
    guchar* pixel = NULL;
    int size = 1;

    if (ebox == ui->draw_left.ebox)
        dm = ui->draw_left;
    else
        dm = ui->draw_right;

    // - Gets the pixel
    pixel = get_clicked_pixel(dm, event->x, event->y);

    if (pixel != NULL)
    {
        // - Puts the color
        pixel[0] = ui->color.r;
        pixel[1] = ui->color.g;
        pixel[2] = ui->color.b;

        size *= gtk_adjustment_get_value(dm.zoom);

        // - Request draw signal
        gtk_widget_queue_draw_area(GTK_WIDGET(dm.darea),
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
