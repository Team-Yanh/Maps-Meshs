#include <gtk/gtk.h>

#if !defined(SPARSE) && defined(__GNUC__)
   #define unused __attribute__((unused))
#else 
   #define unused
#endif

void on_clicked(unused GtkButton* button, gpointer user_data)
{
    gtk_label_set_text(GTK_LABEL(user_data), "Hello World");
}

int main(int argc, char** argv)
{
    // - Init gtk
    gtk_init(&argc, &argv);

    // - Loads the UI description and builds it
    GtkBuilder* builder = gtk_builder_new();
    GError* error = NULL;

    // - Managing error
    if (gtk_builder_add_from_file(builder, "ui.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    // - Gets the widgets
    GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(builder,"window"));
    GtkButton* button = GTK_BUTTON(gtk_builder_get_object(builder, "button"));
    GtkLabel* label = GTK_LABEL(gtk_builder_get_object(builder, "label"));

    // - Connects signal handlers
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(button, "clicked", G_CALLBACK(on_clicked), label);

    gtk_main();

    g_object_unref(GTK_WIDGET(builder));
    gtk_widget_destroy(GTK_WIDGET(window));
    gtk_widget_destroy(GTK_WIDGET(button));
    gtk_widget_destroy(GTK_WIDGET(label));

    return 0;
}
