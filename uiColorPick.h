#ifndef UICOLORPICK_H
#define UICOLORPICK_H

#if !defined(SPARSE) && defined(__GNUC__)
    #define unused __attribute__((unused))
#else
    #define unused
#endif

#include "imageFilter.h"

void set_rgb_entry_value(GtkEntry* text_holder, int value);
double retrieve_color_value(GtkEntry* text_holder);
void on_treat_btn_clicked(GtkButton* button, gpointer user_data);
RGB color_pick(GtkEventBox* ebox, GdkEventButton* event, gpointer udata);
void on_color_picker_btn_clicked(GtkButton* button, gpointer user_data);
void update_rgb_value(GtkEditable* label, gpointer user_data);
void update_color_wheel_value(GtkColorButton* btn, gpointer user_data);

#endif
