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
            load_image(&ui->draw_left, filename);
            g_free(filename);
        }
    }

    // - Hide dialog file chooser
    gtk_widget_hide(GTK_WIDGET(ui->dlg_file_chooser));
}

void on_treat_btn_clicked(unused GtkButton* button, gpointer user_data)
{
    DrawManagement* dm = user_data;
    char filename[] = "images/test1.png";

    load_image(dm, filename);
}

void load_image(DrawManagement* dm, char* filename)
{
    // - Load the new image to draw
    dm->pb = gdk_pixbuf_new_from_file(filename, NULL);
    dm->w = gdk_pixbuf_get_width(dm->pb);
    dm->h = gdk_pixbuf_get_height(dm->pb);

    // - Triggers scrollbars
    gtk_widget_set_size_request(GTK_WIDGET(dm->darea), dm->w, dm->h);

    // - Send a draw signal to redraw the entire area
    gtk_widget_queue_draw(GTK_WIDGET(dm->darea));

    // - Sets the scale to 1
    gtk_adjustment_set_value(dm->zoom, 1.0);
}

void on_zoom(unused GtkScale* zoom_scale, gpointer user_data)
{
    DrawManagement* dm = user_data;

    if (dm->pb != NULL)
        gtk_widget_queue_draw(GTK_WIDGET(dm->darea));
}

void uiTreatment()
{
    // - Init gtk
    gtk_init(NULL, NULL); 
    // - Loads the UI description and builds it
    GtkBuilder* builder = NULL;
    UserInterface* ui = g_slice_new(UserInterface);
    DrawManagement draw_l;
    DrawManagement draw_r;

    // - Managing error
    builder = gtk_builder_new_from_file("ui.glade");

    // - Gets the widgets
    ui->window = GTK_WINDOW(gtk_builder_get_object(builder,"window"));
    ui->img_open_btn= GTK_BUTTON(gtk_builder_get_object(builder,
                "img_open_btn"));
    ui->treat_btn = GTK_BUTTON(gtk_builder_get_object(builder, "treat_btn"));
    ui->color_picker_btn = GTK_BUTTON(
            gtk_builder_get_object(builder, "color_picker_btn"));
    ui->paint_btn = GTK_BUTTON(gtk_builder_get_object(builder, "paint_btn"));
    ui->dlg_file_chooser = GTK_FILE_CHOOSER_DIALOG(
            gtk_builder_get_object(builder, "dlg_file_chooser"));
    ui->color_wheel_btn = GTK_COLOR_CHOOSER(gtk_builder_get_object(
            builder, "color_wheel_btn"));
    ui->rgb_entries[0] = GTK_ENTRY(gtk_builder_get_object(builder, "r_entry"));
    ui->rgb_entries[1] = GTK_ENTRY(gtk_builder_get_object(builder, "g_entry"));
    ui->rgb_entries[2] = GTK_ENTRY(gtk_builder_get_object(builder, "b_entry"));

    draw_l.darea = GTK_DRAWING_AREA(gtk_builder_get_object(builder,
            "draw_area_1"));
    draw_l.pb = NULL;
    draw_l.ebox = GTK_EVENT_BOX(gtk_builder_get_object(builder, "event_box_1"));
    draw_l.scale = GTK_SCALE(gtk_builder_get_object(builder, "zoom_scale_1"));
    draw_l.zoom = GTK_ADJUSTMENT(gtk_builder_get_object(builder,
            "zoom_adjustment_1"));
    draw_l.pick_id = 0;
    draw_l.paint_id = 0;

    draw_r.darea = GTK_DRAWING_AREA(gtk_builder_get_object(builder,
            "draw_area_2"));
    draw_r.pb = NULL;
    draw_r.ebox = GTK_EVENT_BOX(gtk_builder_get_object(builder, "event_box_2"));
    draw_r.scale = GTK_SCALE(gtk_builder_get_object(builder, "zoom_scale_2"));
    draw_r.zoom = GTK_ADJUSTMENT(gtk_builder_get_object(builder,
            "zoom_adjustment_2"));
    draw_r.pick_id = 0;
    draw_r.paint_id = 0;

    ui->draw_left = draw_l;
    ui->draw_right = draw_r;

    // - Free bulder
    g_object_unref(builder);

    // - Connects signal handlers
    g_signal_connect(ui->window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(ui->img_open_btn, "clicked",
            G_CALLBACK(on_img_open_btn_clicked), ui);
    g_signal_connect(ui->color_picker_btn, "clicked",
            G_CALLBACK(on_color_picker_btn_clicked), ui);
    g_signal_connect(ui->draw_left.scale, "value-changed",
            G_CALLBACK(on_zoom), &ui->draw_left);
    g_signal_connect(ui->draw_right.scale, "value-changed",
            G_CALLBACK(on_zoom), &ui->draw_right);
    g_signal_connect(ui->draw_left.darea, "draw", G_CALLBACK(on_draw),
            &ui->draw_left);
    g_signal_connect(ui->draw_right.darea, "draw", G_CALLBACK(on_draw),
            &ui->draw_right);
    g_signal_connect(ui->treat_btn, "clicked", G_CALLBACK(on_treat_btn_clicked),
            &ui->draw_right);
    g_signal_connect(ui->paint_btn, "clicked", G_CALLBACK(on_paint_btn_clicked),
            ui);
    g_signal_connect(GTK_EDITABLE(ui->rgb_entries[0]), "changed",
            G_CALLBACK(update_rgb_value), ui);
    g_signal_connect(GTK_EDITABLE(ui->rgb_entries[1]), "changed",
            G_CALLBACK(update_rgb_value), ui);
    g_signal_connect(GTK_EDITABLE(ui->rgb_entries[2]), "changed",
            G_CALLBACK(update_rgb_value), ui);
    g_signal_connect(GTK_COLOR_BUTTON(ui->color_wheel_btn), "color-set",
            G_CALLBACK(update_color_wheel_value), ui);

    gtk_main();

    g_slice_free(UserInterface, ui);
}
