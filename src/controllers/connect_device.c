#include <pebble.h>
#include "connect_device.h"

static Window *main_window;
static GBitmap *logo_bitmap;
static BitmapLayer *logo_bitmap_layer;

static TextLayer *title_text_layer;
static TextLayer *desc_text_layer;

static void connect_device_window_load(Window *window) {

  static GColor bg_color;
  Layer *window_layer = window_get_root_layer(main_window);
  logo_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_LOGO);

  #ifdef PBL_COLOR
    bg_color = GColorJaegerGreen;
  #else
    bg_color = GColorBlack;
  #endif

  window_set_background_color(window, bg_color);

  logo_bitmap_layer = bitmap_layer_create(GRect(30, 5, 80, 65));
  //Add the image to the later and handle transparency
  bitmap_layer_set_bitmap(logo_bitmap_layer, logo_bitmap);
  layer_add_child(window_layer, bitmap_layer_get_layer(logo_bitmap_layer));

  #ifdef PBL_COLOR
    bitmap_layer_set_compositing_mode(logo_bitmap_layer, GCompOpSet);
  #else
    bitmap_layer_set_compositing_mode(logo_bitmap_layer, GCompOpAssign);
  #endif

  title_text_layer = text_layer_create(GRect(12, 60, 125, 50));
  text_layer_set_font(title_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_color(title_text_layer, GColorWhite);
  text_layer_set_background_color(title_text_layer, GColorClear);
  text_layer_set_text(title_text_layer, "Please connect..");
  layer_add_child(window_layer, text_layer_get_layer(title_text_layer));

  desc_text_layer = text_layer_create(GRect(10, 90, 135, 100));
  text_layer_set_font(desc_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_text_color(desc_text_layer, GColorWhite);
  text_layer_set_background_color(desc_text_layer, GColorClear);
  text_layer_set_text(desc_text_layer, "Make sure your phone is connected, and the app is fully enabled.");
  layer_add_child(window_layer, text_layer_get_layer(desc_text_layer));
}

void connect_device_window_unload(Window *window) {
  gbitmap_destroy(logo_bitmap);
}

void connect_device_init() {
  main_window = window_create();

  window_set_window_handlers(main_window, (WindowHandlers) {
    .load = connect_device_window_load,
    .unload = connect_device_window_unload,
  });
}

void connect_device_deinit() {
  window_destroy(main_window);
}

void connect_device_show() {
  window_stack_push(main_window, true);
}