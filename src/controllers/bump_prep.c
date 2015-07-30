#include <pebble.h>
#include "main_view.h"

static Window *main_window;

static GBitmap *fistbump_bitmap;
static BitmapLayer *fistbump_bitmap_layer;
static TextLayer *title_text_layer;

static void bump_prep_window_load(Window *window) {
  static GColor bg_color;
  Layer *window_layer = window_get_root_layer(main_window);

  #ifdef PBL_COLOR
    bg_color = GColorJaegerGreen;
  #else
    bg_color = GColorBlack;
  #endif

  window_set_background_color(window, bg_color);

  fistbump_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_FISTBUMP_WAITING);
  fistbump_bitmap_layer = bitmap_layer_create(GRect(30, 60, 90, 45));
  //Add the image to the later and handle transparency
  bitmap_layer_set_bitmap(fistbump_bitmap_layer, fistbump_bitmap);
  layer_add_child(window_layer, bitmap_layer_get_layer(fistbump_bitmap_layer));

  #ifdef PBL_COLOR
    bitmap_layer_set_compositing_mode(fistbump_bitmap_layer, GCompOpSet);
  #else
    bitmap_layer_set_compositing_mode(fistbump_bitmap_layer, GCompOpAssign);
  #endif

  title_text_layer = text_layer_create(GRect(20, 93, 150, 50));
  text_layer_set_font(title_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_color(title_text_layer, GColorWhite);
  text_layer_set_background_color(title_text_layer, GColorClear);
  text_layer_set_text(title_text_layer, "Bump devices!");
  layer_add_child(window_layer, text_layer_get_layer(title_text_layer));
}

void bump_prep_window_unload(Window *window) {
  gbitmap_destroy(fistbump_bitmap);
}

void bump_prep_init() {
  main_window = window_create();

  window_set_window_handlers(main_window, (WindowHandlers) {
    .load = bump_prep_window_load,
    .unload = bump_prep_window_unload,
  });
}

void bump_prep_deinit() {
  window_destroy(main_window);
}

void bump_prep_show() {
  window_stack_push(main_window, true);
}