#include <gtk/gtk.h>
#include "uiTreatment.h"
#include "uiColorPick.h"
#include "uiDraw.h"
#include "imageFilter.h"

void on_img_open_btn_clicked(unused GtkButton* button, gpointer user_data)
{
    // - Gets our variable ui
    UserInterface* ui = user_data;

    gchar* filename = NULL;

    // - Display the dialog box file chooser;
    gtk_widget_show(GTK_WIDGET(ui->dlg_file_chooser));

    // - Blocks in a main loop until dialog emits response signal
    if (gtk_dialog_run(GTK_DIALOG(ui->dlg_file_chooser)) == GTK_RESPONSE_OK)
    {
        // - Gets the file name that were choosen
        filename = gtk_file_chooser_get_filename(
                GTK_FILE_CHOOSER(ui->dlg_file_chooser));

        // - If the filename exists
        if (filename != NULL)
        {
            // - Create the saved pixbuf and the display pixbuf
            ui->loaded_pixbuf= gdk_pixbuf_new_from_file(filename, NULL);
            ui->displayed_pixbuf = gdk_pixbuf_new_from_file(filename, NULL);

            // - Display image
            gtk_image_set_from_pixbuf(ui->img_main, ui->displayed_pixbuf);

            // - Sets the scale to 1
            gtk_adjustment_set_value(ui->zoom, 1.0);

            g_free(filename);
        }
    }

    // - Hide dialog file chooser
    gtk_widget_hide(GTK_WIDGET(ui->dlg_file_chooser));
}

void on_treat_btn_clicked(unused GtkButton* button, gpointer user_data)
{
    UserInterface* ui = user_data;
    char filename[] = "images/test1.png";
    int width = 0, height = 0;

    // - Load the new image to draw
    ui->treated_pixbuf = gdk_pixbuf_new_from_file(filename, NULL);

    // - Width and height of the pixbuf
    width = gdk_pixbuf_get_width(ui->treated_pixbuf);
    height = gdk_pixbuf_get_height(ui->treated_pixbuf);

    // - Send a draw signal to redraw the entire area
    gtk_widget_queue_draw_area(GTK_WIDGET(ui->draw_area), 0, 0, width, height);
}

void on_zoom(unused GtkScale* zoom_scale, gpointer user_data)
{
    // - Gets ui
    UserInterface* ui = user_data;

    double zoom_value = 0;
    int width = 0;
    int height = 0;

    if (ui->displayed_pixbuf != NULL)
    {
        // - Gets the zoom value
        zoom_value = gtk_adjustment_get_value(ui->zoom);

        // - Computes the new width and height of the scaled image
        width = gdk_pixbuf_get_width(ui->loaded_pixbuf) * zoom_value;
        height = gdk_pixbuf_get_height(ui->loaded_pixbuf) * zoom_value;

        // - Scale the pixbuf
        ui->displayed_pixbuf = gdk_pixbuf_scale_simple(ui->loaded_pixbuf, width,
                height, GDK_INTERP_NEAREST);

        // - Display the image
        gtk_image_set_from_pixbuf(ui->img_main, ui->displayed_pixbuf);
    }
}

void uiTreatment()
{
    // - Init gtk
    gtk_init(NULL, NULL); 
    // - Loads the UI description and builds it
    GtkBuilder* builder = NULL;
    UserInterface* ui = g_slice_new(UserInterface);

    // - Managing error
    builder = gtk_builder_new_from_file("ui.glade");

    // - Gets the widgets
    ui->window = GTK_WINDOW(gtk_builder_get_object(builder,"window"));
    ui->img_open_btn= GTK_BUTTON(gtk_builder_get_object(builder,
                "img_open_btn"));
    ui->treat_btn = GTK_BUTTON(gtk_builder_get_object(builder, "treat_btn"));
    ui->color_picker_btn = GTK_BUTTON(
            gtk_builder_get_object(builder, "color_picker_btn"));
    ui->dlg_file_chooser = GTK_FILE_CHOOSER_DIALOG(
            gtk_builder_get_object(builder, "dlg_file_chooser"));
    ui->img_main = GTK_IMAGE(gtk_builder_get_object(builder, "img_main"));
    ui->draw_area = GTK_DRAWING_AREA(gtk_builder_get_object(builder,
            "draw_area"));
    ui->img_event_box = GTK_EVENT_BOX(gtk_builder_get_object(builder,
            "img_event_box"));
    ui->color_wheel_btn = GTK_COLOR_CHOOSER(gtk_builder_get_object(
            builder, "color_wheel_btn"));
    ui->zoom_scale = GTK_SCALE(gtk_builder_get_object(builder, "zoom_scale_1"));
    ui->zoom = GTK_ADJUSTMENT(gtk_builder_get_object(builder,
            "zoom_adjustment_1"));
    ui->rgb_entries[0] = GTK_ENTRY(gtk_builder_get_object(builder, "r_entry"));
    ui->rgb_entries[1] = GTK_ENTRY(gtk_builder_get_object(builder, "g_entry"));
    ui->rgb_entries[2] = GTK_ENTRY(gtk_builder_get_object(builder, "b_entry"));
    ui->handler_id = 0;
    ui->loaded_pixbuf = NULL;
    ui->displayed_pixbuf = NULL;
    ui->treated_pixbuf = NULL;

    // - Free bulder
    g_object_unref(builder);

    // - Connects signal handlers
    g_signal_connect(ui->window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(ui->img_open_btn, "clicked",
            G_CALLBACK(on_img_open_btn_clicked), ui);
    g_signal_connect(ui->color_picker_btn, "clicked",
            G_CALLBACK(on_color_picker_btn_clicked), ui);
    g_signal_connect(ui->zoom_scale, "value-changed", G_CALLBACK(on_zoom), ui);
    g_signal_connect(GTK_EDITABLE(ui->rgb_entries[0]), "changed",
            G_CALLBACK(update_rgb_value), ui);
    g_signal_connect(GTK_EDITABLE(ui->rgb_entries[1]), "changed",
            G_CALLBACK(update_rgb_value), ui);
    g_signal_connect(GTK_EDITABLE(ui->rgb_entries[2]), "changed",
            G_CALLBACK(update_rgb_value), ui);
    g_signal_connect(GTK_COLOR_BUTTON(ui->color_wheel_btn), "color-set",
            G_CALLBACK(update_color_wheel_value), ui);
    g_signal_connect(ui->draw_area, "draw", G_CALLBACK(on_draw), ui);
    g_signal_connect(ui->treat_btn, "clicked", G_CALLBACK(on_treat_btn_clicked),
            ui);

    gtk_main();

    g_slice_free(UserInterface, ui);
}
