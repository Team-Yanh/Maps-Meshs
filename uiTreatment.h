#ifndef UITREATMENT_H
#define UITREATMENT_H

#if !defined(SPARSE) && defined(__GNUC__)
    #define unused __attribute__((unused))
#else
    #define unused
#endif

#include "imageFilter.h"

typedef struct DrawManagement
{
    GtkDrawingArea* darea;
    GdkPixbuf* pb;
    GtkEventBox* ebox;
    GtkScale* scale;
    GtkAdjustment* zoom;
    guint pick_id;
    guint paint_id;
    int w;
    int h;
} DrawManagement;

typedef struct UserInterface
{
    GtkWindow* window;
    GtkButton* img_open_btn;
    GtkButton* treat_btn;
    GtkButton* color_picker_btn;
    GtkButton* paint_btn;
    DrawManagement draw_left;
    DrawManagement draw_right;
    GtkColorChooser* color_wheel_btn;
    GtkFileChooserDialog* dlg_file_chooser;
    GtkEntry* rgb_entries[3];
    RGB color;
} UserInterface;

void on_img_open_btn_clicked(GtkButton* button, gpointer user_data);
void on_treat_btn_clicked(unused GtkButton* button, gpointer user_data);
void load_image(DrawManagement* dm, char* filename);
void on_zoom(unused GtkScale* zoom_scale, gpointer user_data);
guchar* get_clicked_pixel(DrawManagement dm, int x, int y);
void uiTreatment();

#endif
