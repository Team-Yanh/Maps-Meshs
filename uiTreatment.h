#ifndef UITREATMENT_H
#define UITREATMENT_H

#if !defined(SPARSE) && defined(__GNUC__)
    #define unused __attribute__((unused))
#else
    #define unused
#endif

#include "imageFilter.h"

typedef struct Image
{
    char* name;
    SDL_Surface* surface;
    GdkRGBA rgba;
    RGB rgb;
    Color* color;
} Image;

typedef struct ColorManagement
{
    GtkColorChooser* color;
    GtkButton* btn;
    struct UserInterface* ui;
    int rgb_entry;
} ColorManagement;

typedef struct Cursors
{
    GdkCursor* def;
    GdkCursor* pick;
    GdkCursor* paint;
} Cursors;

typedef struct DrawManagement
{
    GtkDrawingArea* darea;
    GdkPixbuf* pb;
    GtkEventBox* ebox;
    GtkScale* scale;
    GtkAdjustment* zoom;
    RGB color;
    guint pick_id;
    guint paintable_id;
    guint paint_id;
    guint release_id;
    int w;
    int h;
    struct UserInterface* ui;
} DrawManagement;

typedef struct UserInterface
{
    GtkWindow* window;
    GtkButton* img_open_btn;
    GtkButton* treat_btn;
    GtkButton* color_picker_btn;
    GtkButton* paint_btn;
    GtkButton* restart_btn;
    GtkButton* topo_switch;
    GtkButton* river_switch;
    DrawManagement draw_left;
    DrawManagement draw_right;
    ColorManagement topo;
    ColorManagement river;
    ColorManagement draw;
    GtkFileChooserDialog* dlg_file_chooser;
    GtkEntry* rgb_entries[3];
    GtkAdjustment* size; GtkAdjustment* threshold;
    Cursors cursors;
    int step;
} UserInterface;

void remove_paint_pick_signals(UserInterface* ui);
void on_img_open_btn_clicked(GtkButton* button, gpointer user_data);
void on_treat_and_next(unused GtkButton* button, gpointer user_data);
void on_treat_and_repeat(unused GtkButton* button, gpointer user_data);
Image* initImage(GdkRGBA* rgba, char* filename);
void treat(unused GtkButton* button, gpointer user_data);
void on_restart_btn_clicked(unused GtkButton* button, gpointer user_data);
void load_image_from_file(DrawManagement* dm, char* filename);
void load_image_from_pixbuf(DrawManagement* dm, GdkPixbuf* pb);
void on_zoom(unused GtkScale* zoom_scale, gpointer user_data);
guchar* get_clicked_pixel(DrawManagement* dm, int x, int y);
void switch_dm(UserInterface* ui);
void uiTreatment();

#endif
