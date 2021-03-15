#ifndef COLORPICKER_H
#define COLORPICKER_H

#if !defined(SPARSE) && defined(__GNUC__)
    #define unused __attribute__((unused))
#else
    #define unused
#endif

typedef struct UserInterface
{
    GtkWindow* window;
    GtkMenuItem* menuitm_open;
    GtkFileChooserDialog* dlg_file_chooser;
    GtkImage* img_main;
} UserInterface;

void on_menuitm_open_activate(GtkButton* button, gpointer user_data);
void colorPicker();

#endif
