#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define PANEL_HEIGHT 30
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

void draw_panel(Display *display, Window window, GC gc) {
    // Рисуем панель задач
    XSetForeground(display, gc, WhitePixel(display, 0));
    XFillRectangle(display, window, gc, 0, 0, WINDOW_WIDTH, PANEL_HEIGHT);

    // Рисуем кнопку "Меню"
    XSetForeground(display, gc, BlackPixel(display, 0));
    XFillRectangle(display, window, gc, 10, 5, 60, 20);
    XSetForeground(display, gc, WhitePixel(display, 0));
    XDrawString(display, window, gc, 20, 20, "Menu", 4);
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
                                 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 1,
                                 BlackPixel(display, screen), WhitePixel(display, screen));

    XSelectInput(display, window, ExposureMask | ButtonPressMask);
    XMapWindow(display, window);

    gc = XCreateGC(display, window, 0, NULL);

    while (1) {
        XNextEvent(display, &event);
        if (event.type == Expose) {
            draw_panel(display, window, gc);
        } else if (event.type == ButtonPress) {
            if (event.xbutton.y < PANEL_HEIGHT) {
                if (event.xbutton.x >= 10 && event.xbutton.x <= 70) {
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
