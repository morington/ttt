#include <gtk/gtk.h>
#include <stdlib.h>

#define NUM_WORKSPACES 4

GtkWidget *window, *desktop, *panel, *clock_label, *menu_button;
GtkWidget *workspaces[NUM_WORKSPACES];
int current_workspace = 0;

// Объявления функций
void switch_workspace(GtkWidget *widget, gpointer data);
void on_menu_clicked(GtkWidget *widget, gpointer data);
void launch_application(GtkWidget *widget, gchar *app_name);
gboolean update_clock(gpointer data);

void hide_widget(GtkWidget *widget, gpointer data) {
    gtk_widget_hide(widget);
}

void switch_workspace(GtkWidget *widget, gpointer data) {
    gtk_widget_hide(workspaces[current_workspace]);
    current_workspace = GPOINTER_TO_INT(data);
    gtk_widget_show_all(workspaces[current_workspace]);
}

void on_menu_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *menu = gtk_menu_new();
    GtkWidget *item = gtk_menu_item_new_with_label("Gedit");
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
    g_signal_connect(item, "activate", G_CALLBACK(launch_application), "gedit");
    
    item = gtk_menu_item_new_with_label("Terminal");
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
    g_signal_connect(item, "activate", G_CALLBACK(launch_application), "gnome-terminal");
    
    gtk_widget_show_all(menu);
    gtk_menu_popup_at_pointer(GTK_MENU(menu), NULL);
}

void launch_application(GtkWidget *widget, gchar *app_name) {
    GdkDisplay *display = gdk_display_get_default();
    GdkMonitor *monitor = gdk_display_get_primary_monitor(display);
    GdkRectangle geometry;
    gdk_monitor_get_geometry(monitor, &geometry);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), geometry.width, geometry.height);
    gtk_window_set_decorated(GTK_WINDOW(window), FALSE);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    gtk_container_add(GTK_CONTAINER(window), gtk_label_new(app_name));
    
    gtk_widget_show_all(window);
    gtk_container_add(GTK_CONTAINER(workspaces[current_workspace]), window);
    current_workspace = (current_workspace + 1) % NUM_WORKSPACES;
    gtk_container_foreach(GTK_CONTAINER(workspaces[current_workspace]), (GtkCallback)hide_widget, NULL);
    gtk_widget_show_all(workspaces[current_workspace]);
}

gboolean update_clock(gpointer data) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char time_str[6];
    snprintf(time_str, sizeof(time_str), "%02d:%02d", tm.tm_hour, tm.tm_min);
    gtk_label_set_text(GTK_LABEL(clock_label), time_str);
    return TRUE;
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Simple Desktop");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    desktop = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window), desktop);

    for (int i = 0; i < NUM_WORKSPACES; ++i) {
        workspaces[i] = gtk_fixed_new();
        gtk_fixed_put(GTK_FIXED(desktop), workspaces[i], 0, 0);
    }
    gtk_widget_show_all(workspaces[0]);

    // Панель
    panel = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_size_request(panel, -1, 30);
    gtk_fixed_put(GTK_FIXED(desktop), panel, 0, 0);

    // Часы
    clock_label = gtk_label_new("00:00");
    gtk_box_pack_end(GTK_BOX(panel), clock_label, FALSE, FALSE, 10);
    g_timeout_add_seconds(1, (GSourceFunc)update_clock, NULL);

    // Кнопка меню
    menu_button = gtk_button_new_with_label("Menu");
    gtk_box_pack_start(GTK_BOX(panel), menu_button, FALSE, FALSE, 10);
    g_signal_connect(menu_button, "clicked", G_CALLBACK(on_menu_clicked), NULL);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
