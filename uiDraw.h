#ifndef UIDRAW_H
#define UIDRAW_H

void on_paint_btn_clicked(GtkButton* b, gpointer user_data);
void paint(GtkEventBox* ebox, GdkEventButton* event, gpointer udata);
void on_draw(GtkDrawingArea* darea, cairo_t* cr, gpointer user_data);

#endif
