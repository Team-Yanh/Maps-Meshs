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
                    "button_press_event", G_CALLBACK(paint), &ui->draw_left);
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
                    "button_press_event", G_CALLBACK(paint), &ui->draw_right);
        else
        {
            g_signal_handler_disconnect(ui->draw_right.ebox,
                    ui->draw_right.paint_id);
            ui->draw_right.paint_id = 0;
        }
    }
}

void paint(unused GtkEventBox* ebox, unused GdkEventButton* event,
       unused gpointer udata)
{
    g_print("Paint !\n");
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
