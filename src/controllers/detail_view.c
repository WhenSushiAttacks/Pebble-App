#include <pebble.h>
#include "main_view.h"

static Window *main_window;

static void detail_window_load(Window *window) {
  static GColor bg_color;
  Layer *window_layer = window_get_root_layer(main_window);

  #ifdef PBL_COLOR
    bg_color = GColorJaegerGreen;
  #else
    bg_color = GColorBlack;
  #endif

  window_set_background_color(window, bg_color);
}

void detail_window_unload(Window *window) {

}

void detail_view_init() {
  main_window = window_create();

  window_set_window_handlers(main_window, (WindowHandlers) {
    .load = detail_window_load,
    .unload = detail_window_unload,
  });
}

void detail_view_deinit() {
  window_destroy(main_window);
}

void detail_view_show() {
  window_stack_push(main_window, true);
}