#include <gtk/gtk.h>
#include "uiTreatment.h"
#include "uiColorPick.h"
#include "uiDraw.h"
#include "imageFilter.h"

void remove_paint_pick_signals(UserInterface* ui)
{
    remove_paintable(&ui->draw_left);
    remove_paintable(&ui->draw_right);

    remove_pick_signal(&ui->draw_right);
    remove_pick_signal(&ui->draw_left);
}

void on_img_open_btn_clicked(unused GtkButton* button, gpointer user_data)
{
    // - Gets our variable ui
    UserInterface* ui = user_data;
    gchar* filename = NULL;

    // - Disconnects all signals
    remove_paint_pick_signals(ui);

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

/*
static RGB converts_gdkrgba_to_rgb(GdkRGBA* color)
{
    RGB rgb;

    rgb.r = color->red * 255;
    rgb.g = color->green * 255;
    rgb.b = color->blue * 255;

    return rgb;
}
*/

void on_treat_btn_clicked(unused GtkButton* button, gpointer user_data)
{
    UserInterface* ui = user_data;
    //GdkRGBA river_color, topo_color;
    //RGB river_rgb, topo_rgb;
    //double threshold = 1.0;
    char filename[] = "images/test1.png";

    // - Disconnects all signals
    remove_paint_pick_signals(ui);

    // - Gets river and topologic line colors
    //gtk_color_chooser_get_rgba(ui->river.color, &river_color);
    //gtk_color_chooser_get_rgba(ui->topo.color, &topo_color);

    //river_rgb = converts_gdkrgba_to_rgb(&river_color);
    //topo_rgb = converts_gdkrgba_to_rgb(&topo_color);

    // - Gets threshold value
    //threshold = gtk_adjustment_get_value(ui->threshold);

    // - Treats the image

    // - Loads the treated img
    load_image(&ui->draw_right, filename);
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
    UserInterface* ui = dm->ui;

    remove_paint_pick_signals(ui);

    if (dm->pb != NULL)
        gtk_widget_queue_draw(GTK_WIDGET(dm->darea));
}

guchar* get_clicked_pixel(DrawManagement* dm, int x, int y)
{
    double zoom = 1;
    int rowstride = 0, n_channels = 0, width = 0, height = 0;
    guchar* pixel = NULL;
    guchar* pixels = NULL;

    zoom = gtk_adjustment_get_value(dm->zoom);
    width = dm->w * zoom;
    height = dm->h * zoom;
    n_channels = gdk_pixbuf_get_n_channels(dm->pb);
    rowstride = gdk_pixbuf_get_rowstride(dm->pb);

    if (x >= 0 && x < width && y >= 0 && y < height)
    {
        // - Gets the pixels
        pixels = gdk_pixbuf_get_pixels(dm->pb);

        // - Converts the click position into the buffer from the zoomed pos
        x /= zoom;
        y /= zoom;

        // - Process the address of the pixel at the right coordinates
        // - (Just like in a Matrix)
        pixel = pixels + y * rowstride + x * n_channels;
    }

    return pixel;
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
    ColorManagement topo;
    ColorManagement river;
    ColorManagement draw;

    // - Managing error
    builder = gtk_builder_new_from_file("ui.glade");

    // - Gets the widgets
    ui->window = GTK_WINDOW(gtk_builder_get_object(builder,"window"));
    ui->img_open_btn= GTK_BUTTON(gtk_builder_get_object(builder,
                "img_open_btn"));
    ui->treat_btn = GTK_BUTTON(gtk_builder_get_object(builder, "treat_btn"));
    ui->paint_btn = GTK_BUTTON(gtk_builder_get_object(builder, "paint_btn"));
    ui->dlg_file_chooser = GTK_FILE_CHOOSER_DIALOG(
            gtk_builder_get_object(builder, "dlg_file_chooser"));
    ui->rgb_entries[0] = GTK_ENTRY(gtk_builder_get_object(builder, "r_entry"));
    ui->rgb_entries[1] = GTK_ENTRY(gtk_builder_get_object(builder, "g_entry"));
    ui->rgb_entries[2] = GTK_ENTRY(gtk_builder_get_object(builder, "b_entry"));
    ui->size = GTK_ADJUSTMENT(gtk_builder_get_object(builder,
                "size_adjustment"));
    ui->threshold = GTK_ADJUSTMENT(gtk_builder_get_object(builder,
                "threshold"));

    draw_l.darea = GTK_DRAWING_AREA(gtk_builder_get_object(builder,
            "draw_area_1"));
    draw_l.pb = NULL;
    draw_l.ebox = GTK_EVENT_BOX(gtk_builder_get_object(builder, "event_box_1"));
    draw_l.scale = GTK_SCALE(gtk_builder_get_object(builder, "zoom_scale_1"));
    draw_l.zoom = GTK_ADJUSTMENT(gtk_builder_get_object(builder,
            "zoom_adjustment_1"));
    draw_l.color.r = 255;
    draw_l.color.g = 255;
    draw_l.color.b = 255;
    draw_l.pick_id = 0;
    draw_l.paint_id = 0;
    draw_l.paintable_id = 0;
    draw_l.release_id = 0;
    draw_l.ui = ui;

    draw_r.darea = GTK_DRAWING_AREA(gtk_builder_get_object(builder,
            "draw_area_2"));
    draw_r.pb = NULL;
    draw_r.ebox = GTK_EVENT_BOX(gtk_builder_get_object(builder, "event_box_2"));
    draw_r.scale = GTK_SCALE(gtk_builder_get_object(builder, "zoom_scale_2"));
    draw_r.zoom = GTK_ADJUSTMENT(gtk_builder_get_object(builder,
            "zoom_adjustment_2"));
    draw_r.color.r = 255;
    draw_r.color.g = 255;
    draw_r.color.b = 255;
    draw_r.pick_id = 0;
    draw_r.paint_id = 0;
    draw_r.paintable_id = 0;
    draw_r.release_id = 0;
    draw_r.ui = ui;

    topo.color = GTK_COLOR_CHOOSER(gtk_builder_get_object(builder,
                "topo_color"));
    river.color = GTK_COLOR_CHOOSER(gtk_builder_get_object(builder,
                "river_color"));
    draw.color = GTK_COLOR_CHOOSER(gtk_builder_get_object(builder,
                "draw_color"));

    topo.btn= GTK_BUTTON(gtk_builder_get_object(builder,
                "topo_btn"));
    river.btn= GTK_BUTTON(gtk_builder_get_object(builder,
                "river_btn"));
    draw.btn= GTK_BUTTON(gtk_builder_get_object(builder,
                "draw_btn"));

    topo.ui = ui;
    river.ui = ui;
    draw.ui = ui;

    topo.rgb_entry = 0;
    river.rgb_entry = 0;
    draw.rgb_entry = 1;

    ui->draw_left = draw_l;
    ui->draw_right = draw_r;

    ui->topo = topo;
    ui->river = river;
    ui->draw = draw;

    GdkDisplay* disp = gdk_display_get_default();
    ui->cursors.def = gdk_cursor_new_for_display(disp, GDK_LEFT_PTR);
    ui->cursors.pick = gdk_cursor_new_for_display(disp, GDK_CROSSHAIR);
    ui->cursors.paint = gdk_cursor_new_for_display(disp, GDK_PENCIL);

    // - Free bulder
    g_object_unref(builder);

    // - Connects signal handlers
    g_signal_connect(ui->window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(ui->img_open_btn, "clicked",
            G_CALLBACK(on_img_open_btn_clicked), ui);
    g_signal_connect(ui->topo.btn, "clicked",
            G_CALLBACK(on_color_picker_btn_clicked), &ui->topo);
    g_signal_connect(ui->river.btn, "clicked",
            G_CALLBACK(on_color_picker_btn_clicked), &ui->river);
    g_signal_connect(ui->draw.btn, "clicked",
            G_CALLBACK(on_color_picker_btn_clicked), &ui->draw);
    g_signal_connect(ui->draw_left.scale, "value-changed",
            G_CALLBACK(on_zoom), &ui->draw_left);
    g_signal_connect(ui->draw_right.scale, "value-changed",
            G_CALLBACK(on_zoom), &ui->draw_right);
    g_signal_connect(ui->draw_left.darea, "draw", G_CALLBACK(on_draw),
            &ui->draw_left);
    g_signal_connect(ui->draw_right.darea, "draw", G_CALLBACK(on_draw),
            &ui->draw_right);
    g_signal_connect(ui->treat_btn, "clicked", G_CALLBACK(on_treat_btn_clicked),
            ui);
    g_signal_connect(ui->paint_btn, "clicked", G_CALLBACK(on_paint_btn_clicked),
            ui);
    g_signal_connect(GTK_EDITABLE(ui->rgb_entries[0]), "changed",
            G_CALLBACK(update_rgb_value), ui);
    g_signal_connect(GTK_EDITABLE(ui->rgb_entries[1]), "changed",
            G_CALLBACK(update_rgb_value), ui);
    g_signal_connect(GTK_EDITABLE(ui->rgb_entries[2]), "changed",
            G_CALLBACK(update_rgb_value), ui);
    g_signal_connect(GTK_COLOR_BUTTON(ui->draw.color), "color-set",
            G_CALLBACK(update_color_wheel_value), ui);

    // - Sets initial cursor
    gdk_window_set_cursor(gtk_widget_get_window(GTK_WIDGET(ui->window)),
            ui->cursors.def);

    gtk_main();

    g_slice_free(UserInterface, ui);
}
