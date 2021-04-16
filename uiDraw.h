#ifndef UIDRAW_H
#define UIDRAW_H

void remove_paint_signal(DrawManagement* dm);
void add_paint_signal(DrawManagement* dm);
void switch_paint_signal(DrawManagement* dm);
void on_paint_btn_clicked(GtkButton* b, gpointer user_data);
void paint(GtkEventBox* ebox, GdkEventButton* event, gpointer user_data);
void on_draw(GtkDrawingArea* darea, cairo_t* cr, gpointer user_data);

#endif
