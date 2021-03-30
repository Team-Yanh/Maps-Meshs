#ifndef COLORPICKER_H
#define COLORPICKER_H

#if !defined(SPARSE) && defined(__GNUC__)
    #define unused __attribute__((unused))
#else
    #define unused
#endif

#include "imageFilter.h"

typedef struct UserInterface
{
    GtkWindow* window;
    GtkDialog* dlg_window;
    GtkButton* img_open_btn;
    GtkButton* treat_btn;
    GtkButton* color_picker_btn;
    GtkColorChooser* color_wheel_btn;
    GtkFileChooserDialog* dlg_file_chooser;
    GtkEventBox* img_event_box;
    GtkImage* img_main;
    GtkImage* treated_img;
    GtkScale* zoom_wheel;
    guint handler_id;
    GdkPixbuf* loaded_pixbuf;
    GdkPixbuf* displayed_pixbuf;
    GtkAdjustment* zoom;
    GtkEntry* rgb_entries[3];
} UserInterface;

void set_rgb_entry_value(GtkEntry* text_holder, int value);
double retrieve_color_value(GtkEntry* text_holder);
void on_img_open_btn_clicked(GtkButton* button, gpointer user_data);
void on_treat_btn_clicked(GtkButton* button, gpointer user_data);
RGB on_img_main_clicked(GtkEventBox* img_event_box, GdkEventButton* event, gpointer user_data);
void on_color_picker_btn_clicked(GtkButton* button, gpointer user_data);
void update_rgb_value(GtkEditable* label, gpointer user_data);
void update_color_wheel_value(GtkColorButton* btn, gpointer user_data);
void colorPicker();

#endif
