#include <gtk/gtk.h>
#include "uiTreatment.h"
#include "uiDraw.h"
#include "uiColorPick.h"
#include "imageFilter.h"

void color_pick(GtkEventBox* ebox, GdkEventButton *event, gpointer udata)
{
    // - Gets our variable ui
    ColorManagement* cm = udata;
    UserInterface* ui = cm->ui;
    GdkRGBA col = {255, 255, 255, 1};
    DrawManagement* dm;
    guchar* pixel = NULL;
    RGB rgb = {255, 255, 255};

    // - Guess which event box is clicked and sets the variables
    if (ebox == ui->draw_left.ebox)
        dm = &ui->draw_left;

    else
        dm = &ui->draw_right;

    // - Gets the pixel
    pixel = get_clicked_pixel(dm, event->x, event->y);

    // - If we found a pixel
    if (pixel != NULL)
    {
        // - Gets the rgb values from the pixel
        rgb.r = pixel[0];
        rgb.g = pixel[1];
        rgb.b = pixel[2];

        // - If the color is selected for drawing
        if (cm->rgb_entry)
        {
            ui->draw_left.color = rgb;
            ui->draw_right.color = rgb;
        }

        // - Sets the values to set in the color wheel btn
        col.red = (double)rgb.r/255.0;
        col.green = (double)rgb.g/255.0;
        col.blue = (double)rgb.b/255.0;
    }

    // - Removes the picks signals
    remove_pick_signal(&ui->draw_left);
    remove_pick_signal(&ui->draw_right);

    // - Sets the color of the color wheel
    gtk_color_chooser_set_rgba(cm->color, &col);

    // - Updates the values of the rgb_entries
    // - if the color is picked for drawing
    if (cm->rgb_entry)
    {
        set_rgb_entry_value(ui->rgb_entries[0], rgb.r);
        set_rgb_entry_value(ui->rgb_entries[1], rgb.g);
        set_rgb_entry_value(ui->rgb_entries[2], rgb.b);
    }
}

void remove_pick_signal(DrawManagement* dm)
{
    // - Disconnects the signals of the colorpicker
    if (dm->pick_id > 0)
    {
        g_signal_handler_disconnect(dm->ebox, dm->pick_id);

        // - Sets default cursor
        gdk_window_set_cursor(gtk_widget_get_window(GTK_WIDGET(dm->ui->window)),
                dm->ui->cursors.def);
    }

    dm->pick_id = 0;
}

void add_pick_signal(DrawManagement* dm, ColorManagement* cm)
{
    UserInterface* ui = cm->ui;

    // - Connects the signals if needed
    if (dm->pick_id == 0 && dm->pb != NULL)
    {
        dm->pick_id = g_signal_connect(dm->ebox, "button_press_event",
                G_CALLBACK(color_pick), cm);

        // - Removes paint signals
        remove_paintable(dm);

        // - Modifies the cursor
        gdk_window_set_cursor(gtk_widget_get_window(GTK_WIDGET(ui->window)),
                ui->cursors.pick);
    }
}

void on_color_picker_btn_clicked(unused GtkButton* button, gpointer user_data)
{
    ColorManagement* cm = user_data;
    UserInterface* ui = cm->ui;

    // - Connects the signals
    add_pick_signal(&ui->draw_left, cm);
    add_pick_signal(&ui->draw_right, cm);
}

void set_rgb_entry_value(GtkEntry* text_holder, int value)
{
    char s[] = "0  ";
    size_t i = 0;

    if (value >= 0 || value <= 255)
    {
        // - Determines the beginning of the char
        if (value < 10)
            i = 0;
        else if (value < 100)
            i = 1;
        else
            i = 2;

        // - Performs ITOA
        while (value > 0)
        {
            // - Retrieve actual unit
            char c = value % 10;

            // - Converts it to char and puts it in the string
            s[i] = c + 48;

            i--;
            value /= 10;
        }
    }

    // - Sets the value
    gtk_entry_set_text(text_holder, s);
}

double retrieve_color_value(GtkEntry* text_holder)
{
    // - Gets the text that is written in text_holder
    const gchar* s = gtk_entry_get_text(text_holder);

    // - Converts the string in int then in double
    double v = atoi(s);

    // - Readjusting value if needed
    // - First, if value is inferrior or equal to 0 and the string is not
    // - equal to null string then the string is containing a 0 or a letter
    if (v <= 0 && s[0] != 0)
    {
        v = 0;
        gtk_entry_set_text(text_holder, "0");
    }
    // - Then if the value is superior to 255 we clamp it to 255
    else if (v > 255)
    {
        v = 255;
        gtk_entry_set_text(text_holder, "255");
    }

    return v;
}

void update_rgb_value(unused GtkEditable* label, gpointer user_data)
{
    // - Gets the user interface
    UserInterface* ui = user_data;
    RGB rgb;
    GdkRGBA col = {0, 0, 0, 1};

    // - Gets the values of the different labels
    col.red = retrieve_color_value(ui->rgb_entries[0]);
    col.green = retrieve_color_value(ui->rgb_entries[1]);
    col.blue = retrieve_color_value(ui->rgb_entries[2]);

    // - Sets the color selected
    rgb.r = col.red;
    rgb.g = col.green;
    rgb.b = col.blue;

    ui->draw_left.color = rgb;
    ui->draw_right.color = rgb;

    // - Convertes these values between 0 and 1 to be able to set the color of
    // - the color wheel
    col.red /= 255;
    col.green /= 255;
    col.blue /= 255;

    // - Sets the color wheel color
    gtk_color_chooser_set_rgba(ui->draw.color, &col);
}

void update_color_wheel_value(unused GtkColorButton* btn, gpointer user_data)
{
    UserInterface* ui = user_data;
    GdkRGBA color;
    RGB rgb;

    // - Gets the color picked by the color wheel btn
    gtk_color_chooser_get_rgba(ui->draw.color, &color);

    // - Converts into rgb
    rgb.r = color.red * 255;
    rgb.g = color.green * 255;
    rgb.b = color.blue * 255;

    // - Sets the value into the fields rgb
    set_rgb_entry_value(ui->rgb_entries[0], rgb.r);
    set_rgb_entry_value(ui->rgb_entries[1], rgb.g);
    set_rgb_entry_value(ui->rgb_entries[2], rgb.b);

    // - Sets the color selected
    ui->draw_left.color = rgb;
    ui->draw_right.color = rgb;
}
