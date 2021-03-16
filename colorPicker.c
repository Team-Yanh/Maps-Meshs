#include <gtk/gtk.h>
#include "colorPicker.h"
#include "imageFilter.h"

void on_menuitm_open_activate(unused GtkButton* button, gpointer user_data)
{
    // - Gets our variable ui
    UserInterface* ui = user_data;

    gchar* file_name = NULL;

    // - Display the dialog box file chooser;
    gtk_widget_show(GTK_WIDGET(ui->dlg_file_chooser));

    // - Blocks in a main loop until dialog emits response signal
    if (gtk_dialog_run(GTK_DIALOG(ui->dlg_file_chooser)) == GTK_RESPONSE_OK)
    {
        // - Gets the file name that were choosen
        file_name = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(
                    ui->dlg_file_chooser));

        // - If the filename exists, set the image
        if (file_name != NULL)
            gtk_image_set_from_file(GTK_IMAGE(ui->img_main), file_name);

        g_free(file_name);
    }

    // - Hide dialog file chooser
    gtk_widget_hide(GTK_WIDGET(ui->dlg_file_chooser));
}

RGB on_img_main_clicked(unused GtkEventBox* img_event_box,
        GdkEventButton *event, gpointer user_data)
{
    // - Gets our variable ui
    UserInterface* ui = user_data;
    // - Gets the pixbuf storage of our img
    GdkPixbuf* pixbuf = gtk_image_get_pixbuf(ui->img_main);

    guchar* pixels = NULL;
    guchar* pixel = NULL;

    RGB rgb = {0, 0, 0};

    // - Coordinates of the mouse click
    int x = 0;
    int y = 0;

    // - Number of columns in the pixbuf, the number of channels for each pixel
    int rowstride = 0;
    int n_channels = 0;

    // - If the image is loaded
    if (pixbuf != NULL)
    {
        // - Gets the pixel buffer of the pixbuf storage
        pixels = gdk_pixbuf_get_pixels(pixbuf);
        // - Gets the number of columns
        rowstride = gdk_pixbuf_get_rowstride(pixbuf);
        // - Gets the number of color channels
        n_channels = gdk_pixbuf_get_n_channels(pixbuf);
        // - Gets the coordinates of the mouse event click
        x = event->x;
        y = event->y;

        // - Process the address of the pixel at the right coordinates
        // - (Just like in a Matrix)
        pixel = pixels + y * rowstride + x * n_channels;

        rgb.r = pixel[0];
        rgb.g = pixel[1];
        rgb.b = pixel[2];
    }

    return rgb;
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
    GtkMenuItem* menuitm_open = GTK_MENU_ITEM(
            gtk_builder_get_object(builder, "menuitm_open"));
    GtkFileChooserDialog* dlg_file_chooser = GTK_FILE_CHOOSER_DIALOG(
            gtk_builder_get_object(builder, "dlg_file_chooser"));
    GtkImage* img_main = GTK_IMAGE(gtk_builder_get_object(builder,
                "img_main"));
    GtkEventBox* img_event_box = GTK_EVENT_BOX(gtk_builder_get_object(builder,
                "img_event_box"));

    g_object_ref_sink(builder);

    // - Define our struct ui with the widgets we got from the builder
    UserInterface ui = {
        .window = window,
        .menuitm_open = menuitm_open,
        .dlg_file_chooser = dlg_file_chooser,
        .img_main = img_main
    };

    // - Connects signal handlers
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(menuitm_open, "activate",
            G_CALLBACK(on_menuitm_open_activate), &ui);
    g_signal_connect(img_event_box, "button_press_event",
            G_CALLBACK(on_img_main_clicked), &ui);

    g_object_unref(builder);

    gtk_main();
}
