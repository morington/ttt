#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PANEL_HEIGHT 30
#define WINDOW_WIDTH 800

void draw_panel(Display *display, Window window, GC gc) {
    // Рисуем панель задач
    XSetForeground(display, gc, WhitePixel(display, 0));
    XFillRectangle(display, window, gc, 0, 0, WINDOW_WIDTH, PANEL_HEIGHT);

    // Рисуем кнопку "Терминал"
    XSetForeground(display, gc, BlackPixel(display, 0));
    XFillRectangle(display, window, gc, 10, 5, 80, 20);
    XSetForeground(display, gc, WhitePixel(display, 0));
    XDrawString(display, window, gc, 20, 20, "Terminal", 8);
}

void set_always_on_top(Display *display, Window window) {
    Atom wm_state = XInternAtom(display, "_NET_WM_STATE", False);
    Atom wm_state_above = XInternAtom(display, "_NET_WM_STATE_ABOVE", False);
    Atom wm_dock = XInternAtom(display, "_NET_WM_WINDOW_TYPE_DOCK", False);
    Atom wm_window_type = XInternAtom(display, "_NET_WM_WINDOW_TYPE", False);

    // Установить окно как "док"
    XChangeProperty(display, window, wm_window_type, XA_ATOM, 32,
                    PropModeReplace, (unsigned char *)&wm_dock, 1);

    // Установить окно "всегда сверху"
    XChangeProperty(display, window, wm_state, XA_ATOM, 32,
                    PropModeReplace, (unsigned char *)&wm_state_above, 1);
}

int main() {
    Display *display;
    Window window;
    XEvent event;
    GC gc;
    int screen;

    display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Cannot open display\n");
        exit(1);
    }

    screen = DefaultScreen(display);
    window = XCreateSimpleWindow(display, RootWindow(display, screen),
                                 0, 0, WINDOW_WIDTH, PANEL_HEIGHT, 1,
                                 BlackPixel(display, screen), WhitePixel(display, screen));

    // Устанавливаем обработку событий и отображаем окно
    XSelectInput(display, window, ExposureMask | ButtonPressMask);
    XMapWindow(display, window);

    // Устанавливаем окно как "док" и "всегда сверху"
    set_always_on_top(display, window);

    gc = XCreateGC(display, window, 0, NULL);

    while (1) {
        XNextEvent(display, &event);
        if (event.type == Expose) {
            draw_panel(display, window, gc);
        } else if (event.type == ButtonPress) {
            if (event.xbutton.y < PANEL_HEIGHT) {
                if (event.xbutton.x >= 10 && event.xbutton.x <= 90) {
                    // Запуск терминала
                    if (fork() == 0) {
                        execlp("xterm", "xterm", NULL);
                    }
                }
            }
        }
    }

    XCloseDisplay(display);
    return 0;
}
