#include <gtk/gtk.h>
#include "colorPicker.h"
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

RGB on_img_main_clicked(unused GtkEventBox* img_event_box,
        GdkEventButton *event, gpointer user_data)
{
    // - Gets our variable ui
    UserInterface* ui = user_data;
    RGB rgb = {255, 255, 255};
    GdkRGBA col = {255, 255, 255, 1};

    // - Coordinates of the mouse click
    int x = event->x;
    int y = event->y;

    // - Informations about the image
    int rowstride = gdk_pixbuf_get_rowstride(ui->displayed_pixbuf);
    int n_channels = gdk_pixbuf_get_n_channels(ui->displayed_pixbuf);
    int width = gdk_pixbuf_get_width(ui->displayed_pixbuf);
    int height = gdk_pixbuf_get_height(ui->displayed_pixbuf);

    // - If the image is loaded
    if (x < width && y < height)
    {
        guchar* pixel = NULL;
        guchar* pixels = gdk_pixbuf_get_pixels(ui->displayed_pixbuf);

        // - Process the address of the pixel at the right coordinates
        // - (Just like in a Matrix)
        pixel = pixels + y * rowstride + x * n_channels;

        // - Gets the rgb values from the pixel
        rgb.r = pixel[0];
        rgb.g = pixel[1];
        rgb.b = pixel[2];

        // - Sets the values to set in the color wheel btn
        col.red = (double)rgb.r/255.0;
        col.green = (double)rgb.g/255.0;
        col.blue = (double)rgb.b/255.0;
    }

    // - Sets the color of the color wheel
    gtk_color_chooser_set_rgba(ui->color_wheel_btn, &col);

    // - Disconnects the signal of the colorpicker
    g_signal_handler_disconnect(img_event_box, ui->handler_id);
        ui->handler_id = 0;

    return rgb;
}

void on_color_picker_btn_clicked(unused GtkButton* button, gpointer user_data)
{
    // - Gets the ui
    UserInterface* ui = user_data;

    // - If the signal is not already connected, then connects
    if (ui->handler_id == 0 && ui->displayed_pixbuf != NULL)
        ui->handler_id = g_signal_connect(ui->img_event_box,
                "button_press_event", G_CALLBACK(on_img_main_clicked),
                user_data);
}

void on_zoom(unused GtkScale* zoom_wheel, gpointer user_data)
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
                height, GDK_INTERP_BILINEAR);

        // - Display the image
        gtk_image_set_from_pixbuf(ui->img_main, ui->displayed_pixbuf);
    }
}

void colorPicker()
{
    // - Init gtk
    gtk_init(NULL, NULL); 
    // - Loads the UI description and builds it
    GtkBuilder* builder = gtk_builder_new();
    GError* error = NULL;

    // - Managing error
    if (gtk_builder_add_from_file(builder, "ui.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
    }

    // - Gets the widgets
    GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(builder,"window"));
    GtkButton* img_open_btn= GTK_BUTTON(
            gtk_builder_get_object(builder, "img_open_btn"));
    GtkButton* color_picker_btn = GTK_BUTTON(
            gtk_builder_get_object(builder, "color_picker_btn"));
    GtkFileChooserDialog* dlg_file_chooser = GTK_FILE_CHOOSER_DIALOG(
            gtk_builder_get_object(builder, "dlg_file_chooser"));
    GtkImage* img_main = GTK_IMAGE(gtk_builder_get_object(builder,
            "img_main"));
    GtkEventBox* img_event_box = GTK_EVENT_BOX(gtk_builder_get_object(builder,
            "img_event_box"));
    GtkColorChooser* color_wheel_btn = GTK_COLOR_CHOOSER(gtk_builder_get_object(
            builder, "color_wheel_btn"));
    GtkScale* zoom_wheel = GTK_SCALE(gtk_builder_get_object(builder, 
            "zoom_wheel"));
    GtkAdjustment* zoom = GTK_ADJUSTMENT(gtk_builder_get_object(builder,
            "zoom_adjustment"));

    g_object_ref_sink(builder);

    // - Define our struct ui with the widgets we got from the builder
    UserInterface ui = {
        .window = window,
        .img_open_btn = img_open_btn,
        .color_picker_btn = color_picker_btn,
        .color_wheel_btn = color_wheel_btn,
        .dlg_file_chooser = dlg_file_chooser,
        .img_event_box = img_event_box,
        .img_main = img_main,
        .zoom_wheel = zoom_wheel,
        .handler_id = 0,
        .loaded_pixbuf = NULL,
        .displayed_pixbuf = NULL,
        .zoom = zoom,
    };

    // - Connects signal handlers
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(img_open_btn, "clicked",
            G_CALLBACK(on_img_open_btn_clicked), &ui);
    g_signal_connect(color_picker_btn, "clicked",
            G_CALLBACK(on_color_picker_btn_clicked), &ui);
    g_signal_connect(zoom_wheel, "value-changed", G_CALLBACK(on_zoom), &ui);

    gtk_main();
}
