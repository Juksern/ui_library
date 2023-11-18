#include <stdio.h>
#include "library.h"

void draw_box(u64 a, u64 b, u64 c , u64 d) {
    printf("drawing box: %llu %llu %llu %llu\n", a, b, c, d);
}

void draw_text(string title, u64 x, u64 y) {
    printf("drawing text: %s %llu %llu\n", title, x, y);
}

void draw_circle(u64 a, u64 b, f64 r) {
    printf("drawing circle: %llu %llu %f\n", a, b, r);
}

int main() {
    ui_set_box_renderer(draw_box);
    ui_set_text_renderer(draw_text);
    ui_set_circle_renderer(draw_circle);

    for (int i = 0; i < 100; ++i) {
        ui_start();
        if (ui_button("button0"))
            printf("hello world\n");
        ui_label("label0");
            ui_start();
            if (ui_button("button0"))
                printf("hello world\n");

            ui_label("label0");
            ui_stop();
        ui_stop();
    }

    return 0;
}