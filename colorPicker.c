#include <gtk/gtk.h>
#include "colorPicker.h"

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
    GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(builder,"window_main"));
    GtkMenuItem* menuitm_open = GTK_MENU_ITEM(
            gtk_builder_get_object(builder, "menuitm_open"));
    GtkFileChooserDialog* dlg_file_chooser = GTK_FILE_CHOOSER_DIALOG(
            gtk_builder_get_object(builder, "dlg_file_chooser"));
    GtkImage* img_main = GTK_IMAGE(gtk_builder_get_object(builder,
                "img_main"));

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

    g_object_unref(builder);

    gtk_main();

    gtk_widget_destroy(GTK_WIDGET(window));
    gtk_widget_destroy(GTK_WIDGET(menuitm_open));
    gtk_widget_destroy(GTK_WIDGET(dlg_file_chooser));
    gtk_widget_destroy(GTK_WIDGET(img_main));
}
