#ifndef UITREATMENT_H
#define UITREATMENT_H

#if !defined(SPARSE) && defined(__GNUC__)
    #define unused __attribute__((unused))
#else
    #define unused
#endif

typedef struct UserInterface
{
    GtkWindow* window;
    GtkButton* img_open_btn;
    GtkButton* treat_btn;
    GtkButton* color_picker_btn;
    GtkColorChooser* color_wheel_btn;
    GtkFileChooserDialog* dlg_file_chooser;
    GtkEventBox* img_event_box;
    GtkImage* img_main;
    GtkDrawingArea* draw_area;
    cairo_surface_t *img_treated;
    GtkScale* zoom_scale;
    guint handler_id;
    GdkPixbuf* loaded_pixbuf;
    GdkPixbuf* displayed_pixbuf;
    GdkPixbuf* treated_pixbuf;
    GtkAdjustment* zoom;
    GtkEntry* rgb_entries[3];
} UserInterface;

void on_img_open_btn_clicked(GtkButton* button, gpointer user_data);
void on_treat_btn_clicked(unused GtkButton* button, gpointer user_data);
void on_zoom(unused GtkScale* zoom_scale, gpointer user_data);
void uiTreatment();

#endif
