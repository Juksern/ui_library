#ifndef UI_LIBRARY_LIBRARY_H
#define UI_LIBRARY_LIBRARY_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/*
 * goals:
 * - 2D drawing
 * - responsive
 *
 *
 * */

/* types */

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float f64;
typedef const char* string;

/* function pointers */
typedef void (*box_fn)(u64 x, u64 y, u64 w, u64 h);
typedef void (*text_fn)(string text, u64 x, u64 y);
typedef void (*circle_fn)(u64 x, u64 y, f64 radius);

typedef enum ui_type_e{
    UI_TYPE_BOX,
    UI_TYPE_TEXT,
    UI_TYPE_CIRCLE
} ui_type;

typedef struct box_data_t{
    u64 x, y, w, h;
    box_fn render;
} box_data;

typedef struct text_data_t{
   string title;
    u64 x, y;
    text_fn render;
} text_data;

typedef struct circle_data_t{
    u64 x, y;
    f64 r;
    circle_fn render;
} circle_data;

typedef struct ui_element_t {
    struct ui_element_t* next;
    ui_type type;
    union {
        box_data boxData;
        text_data textData;
        circle_data circleData;
    } data;
} ui_element;

typedef struct ui_container_t {
    ui_element* head;
    ui_element* tail;

    u64 grid_x, grid_y, grid_spacing, grid_cols;
} ui_container;

typedef struct ui_context_t {
    ui_container* containers;
    size_t num_containers;
} ui_context;

/* drawing function pointers */
extern box_fn ui_box;
extern text_fn ui_text;
extern circle_fn ui_circle;

/* ui components */
void ui_start();
void ui_stop();
void ui_add(ui_type type, void* data);
bool ui_button();
void ui_label();
void ui_slider();
void ui_dropdown();

/* automatic grid functions */
void ui_place(ui_container* container, ui_element* element);
void ui_update_grid(ui_container* container);

/* init functions */
void ui_set_box_renderer(box_fn renderer);
void ui_set_text_renderer(text_fn renderer);
void ui_set_circle_renderer(circle_fn renderer);
#endif //UI_LIBRARY_LIBRARY_H
