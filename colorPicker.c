#include <gtk/gtk.h>
#include "colorPicker.h"
#include "imageFilter.h"

void on_img_open_btn_clicked(unused GtkButton* button, gpointer user_data)
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

        // - If the filename exists
        if (file_name != NULL)
        {
            GdkPixbuf* pixbuf = NULL;

            // - Sets the image on the ui
            gtk_image_set_from_file(GTK_IMAGE(ui->img_main), file_name);
            // - Gets the pixbuf storage of the image
            pixbuf = gtk_image_get_pixbuf(ui->img_main);

            // - Initialize our image_loaded field of our ui
            ui->img_loaded->pixbuf = pixbuf;
            ui->img_loaded->pixels = gdk_pixbuf_get_pixels(pixbuf);
            ui->img_loaded->rowstride = gdk_pixbuf_get_rowstride(pixbuf);
            ui->img_loaded->n_channels = gdk_pixbuf_get_n_channels(pixbuf);
            ui->img_loaded->width = gdk_pixbuf_get_width(pixbuf);
            ui->img_loaded->height = gdk_pixbuf_get_height(pixbuf);

            g_free(file_name);
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
    RGB rgb = {0, 0, 0};

    // - Coordinates of the mouse click
    int x = event->x;
    int y = event->y;

    // - If the image is loaded
    if (ui->img_loaded->pixbuf != NULL && x < ui->img_loaded->width
            && y < ui->img_loaded->height)
    {
        guchar* pixel = NULL;

        // - Process the address of the pixel at the right coordinates
        // - (Just like in a Matrix)
        pixel = ui->img_loaded->pixels + y * ui->img_loaded->rowstride + x
            * ui->img_loaded->n_channels;

        rgb.r = pixel[0];
        rgb.g = pixel[1];
        rgb.b = pixel[2];

        g_signal_handler_disconnect(img_event_box, ui->handler_id);
        ui->handler_id = 0;
    }

    g_print("Rouge: %d\nVert: %d\nBleu: %d\n", rgb.r, rgb.g, rgb.b);

    return rgb;
}

void on_color_picker_btn_clicked(unused GtkButton* button, gpointer user_data)
{
    UserInterface* ui = user_data;
    if (ui->handler_id == 0)
        ui->handler_id = g_signal_connect(ui->img_event_box,
                "button_press_event", G_CALLBACK(on_img_main_clicked),
                user_data);
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

    //g_object_ref_sink(builder);

    // - Define our struct ui with the widgets we got from the builder
    UserInterface ui = {
        .window = window,
        .img_open_btn = img_open_btn,
        .color_picker_btn = color_picker_btn,
        .dlg_file_chooser = dlg_file_chooser,
        .img_event_box = img_event_box,
        .img_main = img_main,
        .img_loaded = calloc(1, sizeof(Image)),
        .handler_id = 0,
    };


    // - Connects signal handlers
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(img_open_btn, "clicked",
            G_CALLBACK(on_img_open_btn_clicked), &ui);
    g_signal_connect(color_picker_btn, "clicked",
            G_CALLBACK(on_color_picker_btn_clicked), &ui);

    g_object_unref(builder);

    gtk_main();

    free(ui.img_loaded);
}
