#include <gtk/gtk.h>
#include "uiTreatment.h"
#include "uiColorPick.h"
#include "uiDraw.h"
#include "imageFilter.h"
#include "imageColoring.h"
#include "imageUtils.h"

void remove_signals(UserInterface* ui)
{
    remove_paintable(&ui->draw_left);
    remove_paintable(&ui->draw_right);

    remove_pick_signal(&ui->draw_right);
    remove_pick_signal(&ui->draw_left);

    remove_link(&ui->draw_left);
    remove_link(&ui->draw_right);

    reset_points(ui->points);
}

void on_img_open_btn_clicked(unused GtkButton* button, gpointer user_data)
{
    // - Gets our variable ui
    UserInterface* ui = user_data;
    gchar* filename = NULL;

    // - Disconnects all signals
    remove_signals(ui);

    // - Display the dialog box file chooser;
    gtk_widget_show(GTK_WIDGET(ui->dlg_file_chooser));

    // - Blocks in a main loop until dialog emits response signal
    if (gtk_dialog_run(GTK_DIALOG(ui->dlg_file_chooser)) == GTK_RESPONSE_OK)
    {
        // - Resets treatment step
        ui->step = -1;
        ui->type = TOPO;

        // - Gets the file name that were choosen
        filename = gtk_file_chooser_get_filename(
                GTK_FILE_CHOOSER(ui->dlg_file_chooser));

        // - If the filename exists
        if (filename != NULL)
        {
            load_image_from_file(&ui->draw_left, filename);
            g_free(filename);
        }
    }

    // - Hide dialog file chooser
    gtk_widget_hide(GTK_WIDGET(ui->dlg_file_chooser));
}

static RGB converts_gdkrgba_to_rgb(GdkRGBA* color)
{
    RGB rgb;

    rgb.r = color->red * 255;
    rgb.g = color->green * 255;
    rgb.b = color->blue * 255;

    return rgb;
}

void on_treat_and_next(unused GtkButton* button, gpointer user_data)
{
    UserInterface* ui = user_data;

    remove_signals(ui);

    if (ui->draw_left.pb != NULL)
    {
        ui->step++;

        if (ui->type == RIVER)
            load_image_from_file(&ui->draw_right, "topo.bmp");

        if (ui->step > 0)
            switch_dm(ui);

        treat(button, ui);
    }
}

void on_treat_and_repeat(unused GtkButton* button, gpointer user_data)
{
    UserInterface* ui = user_data;

    remove_signals(ui);

    if (ui->step > -1 && ui->draw_left.pb != NULL)
        treat(button, ui);
}

Image* initImage(GdkRGBA* rgba, char* filename)
{
    Image* img = calloc(sizeof(Image), 1);
    img->name = filename;

    img->surface = IMG_Load(filename);
    if (img->surface == NULL)
        errx(1, "Couldn't load image");

    img->rgba = *rgba;
    img->rgb = converts_gdkrgba_to_rgb(&img->rgba);
    img->color = initColor(img->surface->format);
    setRGB(img->color, img->rgb.r, img->rgb.g, img->rgb.b);

    return img;
}

void on_topo_switch_clicked(unused GtkButton* btn, gpointer user_data)
{
    UserInterface* ui = user_data;

    if (ui->draw_right.pb != NULL && ui->type == RIVER)
    {
        ui->type = TOPO;
        gdk_pixbuf_save(ui->draw_right.pb, "river.bmp", "bmp", NULL, NULL);
        load_image_from_file(&ui->draw_right, "topo.bmp");
    }
}

void on_river_switch_clicked(unused GtkButton* btn, gpointer user_data)
{
    UserInterface* ui = user_data;

    if (ui->draw_right.pb != NULL && ui->type == TOPO)
    {
        ui->type = RIVER;
        gdk_pixbuf_save(ui->draw_right.pb, "topo.bmp", "bmp", NULL, NULL);
        load_image_from_file(&ui->draw_right, "river.bmp");
    }
}

void treat(unused GtkButton* button, gpointer user_data)
{
    UserInterface* ui = user_data;
    GdkRGBA river_rgba;
    GdkRGBA topo_rgba;
    double threshold = 1.0; // [0.1 , 5]
    Color *black = NULL;
    Image* river = NULL;
    Image* topo = NULL;
    int end = 0;

    // - Gets river and topologic line colors
    gtk_color_chooser_get_rgba(ui->river.color, &river_rgba);
    gtk_color_chooser_get_rgba(ui->topo.color, &topo_rgba);

    // - Save the image form the left area
    if(ui->step == 0)
    {
        gdk_pixbuf_save(ui->draw_left.pb, "topo.bmp", "bmp", NULL, NULL);
        gdk_pixbuf_save(ui->draw_left.pb, "river.bmp", "bmp", NULL, NULL);
    }

    else if (ui->type == TOPO)
        gdk_pixbuf_save(ui->draw_left.pb, "topo.bmp", "bmp", NULL, NULL);

    else
        gdk_pixbuf_save(ui->draw_left.pb, "river.bmp", "bmp", NULL, NULL);

    // - Inits image structs
    river = initImage(&river_rgba, "river.bmp");
    topo = initImage(&topo_rgba, "topo.bmp");

        // - Gets threshold value
    threshold = gtk_adjustment_get_value(ui->threshold);

    // - Inits black color
    black = initColor(topo->surface->format);
    setRGB(black, 0, 0, 0);

    switch(ui->step)
    {
        case 0:
            printf("Filtering topologic lines...\n");
            keepTopoLineHSV(topo->surface, topo->color, threshold);
            setMonochromatic(topo->surface, black);

            printf("Filtering river...\n");
            keepTopoLineHSV(river->surface, river->color, threshold);
            setMonochromatic(river->surface, black);
            break;

        case 1:
            printf("Removing topologic lines isolated pixels...\n");
            removeIsolatedPixels(topo->surface);

            printf("Removing river isolated pixels...\n");
            removeIsolatedPixels(river->surface);
            break;

        case 2:
            printf("Finding all extremities...\n");
            FindAllExtremity(topo->surface);
            //FindAllExtremity(river->surface);
            setMonochromatic(topo->surface, black);
            //setMonochromatic(river->surface, black);
            break;

        case 3:
            printf("Thickenning topologic lines colors...\n");
            thickenColor(topo->surface, black);

            printf("Thickenning river colors...\n");
            thickenColor(river->surface, black);
            break;


        case 4:
            printf("Making height map from topologic lines...\n");
            makeHeightMap(topo->name, "height_map.bmp");
            end = 1;
            break;

        default:
            break;
    }

    SDL_SaveBMP(topo->surface, topo->name);
    SDL_SaveBMP(river->surface, river->name);

    // - Loads the treated img
    load_image_from_file(&ui->draw_right, topo->name);
    ui->type = TOPO;

    SDL_FreeSurface(topo->surface);
    SDL_FreeSurface(river->surface);
    freeColor(topo->color);
    freeColor(river->color);
    free(topo);
    free(river);
    freeColor(black);

    if (end)
        gtk_main_quit();
}

void load_image_from_file(DrawManagement* dm, char* filename)
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

void load_image_from_pixbuf(DrawManagement* dm, GdkPixbuf* pb)
{
    dm->pb = pb;
    if (pb != NULL)
    {
        dm->w = gdk_pixbuf_get_width(dm->pb);
        dm->h = gdk_pixbuf_get_height(dm->pb);

        // - Triggers scrollbars
        gtk_widget_set_size_request(GTK_WIDGET(dm->darea), dm->w, dm->h);
    }

    // - Send a draw signal to redraw the entire area
    gtk_widget_queue_draw(GTK_WIDGET(dm->darea));

    // - Sets the scale to 1
    gtk_adjustment_set_value(dm->zoom, 1.0);
}

void on_zoom(unused GtkScale* zoom_scale, gpointer user_data)
{
    DrawManagement* dm = user_data;
    UserInterface* ui = dm->ui;

    remove_signals(ui);

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

void switch_dm(UserInterface* ui)
{
    GdkPixbuf* temp;

    temp = ui->draw_right.pb;
    load_image_from_pixbuf(&ui->draw_right, ui->draw_left.pb);
    load_image_from_pixbuf(&ui->draw_left, temp);
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
    ui->restart_btn = GTK_BUTTON(gtk_builder_get_object(builder,
                "restart_btn"));
    ui->topo_switch = GTK_BUTTON(gtk_builder_get_object(builder,
                "topo_switch"));
    ui->river_switch = GTK_BUTTON(gtk_builder_get_object(builder,
                "river_switch"));
    ui->link_btn = GTK_BUTTON(gtk_builder_get_object(builder, "link_btn"));
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
    draw_l.link_id = 0;
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
    draw_r.link_id = 0;
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

    ui->points[0].x = -1;
    ui->points[1].x = -1;
    ui->points[0].y = -1;
    ui->points[1].y = -1;

    ui->step = -1;

    ui->type = TOPO;

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
    g_signal_connect(ui->topo_switch, "clicked",
            G_CALLBACK(on_topo_switch_clicked), ui);
    g_signal_connect(ui->river_switch, "clicked",
            G_CALLBACK(on_river_switch_clicked), ui);
    g_signal_connect(ui->link_btn, "clicked",
            G_CALLBACK(on_link_btn_clicked), ui);
    g_signal_connect(ui->draw_left.scale, "value-changed",
            G_CALLBACK(on_zoom), &ui->draw_left);
    g_signal_connect(ui->draw_right.scale, "value-changed",
            G_CALLBACK(on_zoom), &ui->draw_right);
    g_signal_connect(ui->draw_left.darea, "draw", G_CALLBACK(on_draw),
            &ui->draw_left);
    g_signal_connect(ui->draw_right.darea, "draw", G_CALLBACK(on_draw),
            &ui->draw_right);
    g_signal_connect(ui->treat_btn, "clicked",G_CALLBACK(on_treat_and_next),ui);
    g_signal_connect(ui->restart_btn, "clicked",G_CALLBACK(on_treat_and_repeat),
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
