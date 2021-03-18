#ifndef COLORPICKER_H
#define COLORPICKER_H

#if !defined(SPARSE) && defined(__GNUC__)
    #define unused __attribute__((unused))
#else
    #define unused
#endif

#include "imageFilter.h"

typedef struct Image
{
    GdkPixbuf* pixbuf;
    guchar* pixels;
    int rowstride;
    int n_channels;
    int width;
    int height;
} Image;

typedef struct UserInterface
{
    GtkWindow* window;
    GtkButton* img_open_btn;
    GtkButton* color_picker_btn;
    GtkColorChooser* color_wheel_btn;
    GtkFileChooserDialog* dlg_file_chooser;
    GtkEventBox* img_event_box;
    GtkImage* img_main;
    GtkScale* zoom_wheel;
    Image* img_loaded;
    guint handler_id;
} UserInterface;

void on_img_open_btn_clicked(GtkButton* button, gpointer user_data);
RGB on_img_main_clicked(GtkEventBox* img_event_box, GdkEventButton* event, gpointer user_data);
void on_color_picker_btn_clicked(GtkButton* button, gpointer user_data);
void colorPicker();

#endif
