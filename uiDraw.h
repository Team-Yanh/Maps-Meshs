#ifndef UIDRAW_H
#define UIDRAW_H

typedef struct Pos
{
    int x;
    int y;
} Pos;

typedef struct Zone
{
    Pos init;
    Pos ind;
    Pos offSet;
    int size;
    int w;
    int h;
} Zone;

void add_paintable(DrawManagement* dm);
void remove_paintable(DrawManagement* dm);
void add_paint(DrawManagement* dm);
void remove_paint(DrawManagement* dm);
void add_release(DrawManagement* dm);
void remove_release(DrawManagement* dm);
void switch_paintable(DrawManagement* dm);
void begin_paint(GtkEventBox* ebox, GdkEventButton* event, gpointer user_data);
void end_paint(GtkEventBox* ebox, GdkEventButton* event, gpointer user_data);
void on_paint_btn_clicked(GtkButton* b, gpointer user_data);
void paint(GtkEventBox* ebox, GdkEventButton* event, gpointer user_data);
void on_draw(GtkDrawingArea* darea, cairo_t* cr, gpointer user_data);

#endif
