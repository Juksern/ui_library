#include "library.h"

ui_context ui_ctx = {NULL, 0};

box_fn ui_box = NULL;
text_fn ui_text = NULL;
circle_fn ui_circle = NULL;

void ui_set_box_renderer(box_fn renderer) {
    ui_box = renderer;
}

void ui_set_text_renderer(text_fn renderer) {
    ui_text = renderer;
}

void ui_set_circle_renderer(circle_fn renderer) {
    ui_circle = renderer;
}

void ui_start() {
    ui_ctx.num_containers++;
    ui_ctx.containers = (ui_container*) realloc(ui_ctx.containers,
                                               sizeof(ui_container) * ui_ctx.num_containers);

    if (ui_ctx.containers == NULL)
        return;

    ui_container* cur = &(ui_ctx.containers[ui_ctx.num_containers - 1]);
    cur->head = NULL;
    cur->tail = NULL;

    cur->grid_x = 0;
    cur->grid_y = 0;
    cur->grid_spacing = 10;
    cur->grid_cols = 3;
}

void ui_stop() {
    for (size_t i = 0; i < ui_ctx.num_containers; ++i) {
        ui_container* cur = &(ui_ctx.containers[i]);
        ui_element* e = cur->head;

        while (e != NULL) {
            switch(e->type) {
                case UI_TYPE_BOX: {
                    box_data* t = &(e->data.boxData);
                    if (t->render)
                        t->render(t->x, t->y, t->w, t->h);
                    break;
                }
                case UI_TYPE_TEXT: {
                    text_data* t = &(e->data.textData);
                    if (t->render)
                        t->render(t->title, t->x, t->y);
                    break;
                }
                case UI_TYPE_CIRCLE: {
                    circle_data* t = &(e->data.circleData);
                    if (t->render)
                        t->render(t->x, t->y, t->r);
                    break;
                }
            }
            ui_element *next = e->next;
            free(e);
            e = next;
        }

        cur->head = NULL;
        cur->tail = NULL;
    }

    free(ui_ctx.containers);
    ui_ctx.num_containers = 0;
    ui_ctx.containers = NULL;
}

void ui_add(ui_type type, void* data) {
    ui_container* cur = &(ui_ctx.containers[ui_ctx.num_containers-1]);
    ui_element* e = (ui_element*) malloc(sizeof(ui_element));

    if (e == NULL)
        return;

    e->next = NULL;
    e->type = type;

    switch(type) {
        case UI_TYPE_BOX:
            e->data.boxData = *(box_data *)data;
            break;
        case UI_TYPE_TEXT:
            e->data.textData = *(text_data *)data;
            break;
        case UI_TYPE_CIRCLE:
            e->data.circleData = *(circle_data *)data;
            break;
        default:
            free(e);
            return;
    }

    ui_place(cur, e);
    ui_update_grid(cur);

    if (cur->head == NULL) {
        cur->head = e;
        cur->tail = e;
    } else {
        cur->tail->next = e;
        cur->tail = e;
    }
}

bool ui_button(string title) {
    box_data box = {0, 0, 100, 50, ui_box};
    text_data text = {title, 10, 20, ui_text};

    ui_add(UI_TYPE_BOX, &box);
    ui_add(UI_TYPE_TEXT, &text);

    return true;
}

void ui_label(string title) {
    if (!ui_text)
        return;

    text_data text = {
            title, 10, 10, ui_text
    };

    ui_add(UI_TYPE_TEXT, &text);
}

void ui_slider(string title) {
    if (!ui_text)
        return;

    text_data text = {
            title, 10, 10, ui_text
    };

    ui_add(UI_TYPE_TEXT, &text);
}

void ui_dropdown(string title) {
    if (!ui_text)
        return;

    text_data text = {
            title, 10, 10, ui_text
    };

    ui_add(UI_TYPE_TEXT, &text);
}

void ui_place(ui_container* container, ui_element* element) {
    switch(element->type) {
        case UI_TYPE_BOX:
            element->data.boxData.x = container->grid_x;
            element->data.boxData.y = container->grid_y;
            break;
        case UI_TYPE_TEXT:
            element->data.textData.x = container->grid_x;
            element->data.textData.y = container->grid_y;
            break;
        case UI_TYPE_CIRCLE:
            element->data.circleData.x = container->grid_x;
            element->data.circleData.y = container->grid_y;
            break;
    }
}

void ui_update_grid(ui_container* container) {
    container->grid_x += container->grid_spacing;

    if (container->grid_x >= container->grid_cols * container->grid_spacing) {
        container->grid_x = 0;
        container->grid_y += container->grid_spacing;
    }
}


