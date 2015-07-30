#include <pebble.h>
#include "main_view.h"
#include "handlers/vec.h"
#include "handlers/app_message_handler.h"
#include "controllers/bump_prep.h"

static Window *main_window;
static TextLayer *title_text_layer;
static MenuLayer *s_menu_layer;

static vec_t(struct Item) menu_items_general;
static vec_t(struct Item) menu_items_private;

struct Item
{
   char*  title;
   char*  type;
   char*  subtitle;
   int    id;
};

static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
  return 2;
}

static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  switch (section_index) {
    case 0:
      return menu_items_general.length;
    case 1:
      return menu_items_private.length;
    default:
      return 0;
  }
}

static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}

static void menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
  // Determine which section we're working with
  switch (section_index) {
    case 0:
      menu_cell_basic_header_draw(ctx, cell_layer, "General");
      break;
    case 1:
      menu_cell_basic_header_draw(ctx, cell_layer, "Private");
      break;
  }
}

static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
  // Determine which section we're going to draw in
  switch (cell_index->section) {
    case 0:
      menu_cell_basic_draw(ctx, cell_layer, menu_items_general.data[0].title, menu_items_general.data[0].subtitle, NULL);
      break;
    case 1:
      menu_cell_basic_draw(ctx, cell_layer, menu_items_private.data[0].title, menu_items_general.data[0].subtitle, NULL);
      break;
  }
}

static void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  // Use the row to specify which item will receive the select action
  APP_LOG(APP_LOG_LEVEL_INFO, "Message dropped! %i", cell_index->row);
}

static void select_long_click_handler(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  bump_prep_show();
}

static void main_window_load(Window *window) {

  static GColor bg_color;
  Layer *window_layer = window_get_root_layer(main_window);
  GRect bounds = layer_get_frame(window_layer);

  #ifdef PBL_COLOR
    bg_color = GColorJaegerGreen;
  #else
    bg_color = GColorBlack;
  #endif

  window_set_background_color(window, bg_color);

  title_text_layer = text_layer_create(GRect(10, 65, 150, 50));
  text_layer_set_font(title_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_color(title_text_layer, GColorWhite);
  text_layer_set_background_color(title_text_layer, GColorClear);
  text_layer_set_text(title_text_layer, "Waiting for data..");
  layer_add_child(window_layer, text_layer_get_layer(title_text_layer));

  // Create the menu layer
  s_menu_layer = menu_layer_create(bounds);
  menu_layer_set_callbacks(s_menu_layer, NULL, (MenuLayerCallbacks){
    .get_num_sections = menu_get_num_sections_callback,
    .get_num_rows = menu_get_num_rows_callback,
    .get_header_height = menu_get_header_height_callback,
    .draw_header = menu_draw_header_callback,
    .draw_row = menu_draw_row_callback,
    .select_click = menu_select_callback,
    .select_long_click = select_long_click_handler,
  });

  layer_set_hidden(menu_layer_get_layer(s_menu_layer), true);

  // Bind the menu layer's click config provider to the window for interactivity
  menu_layer_set_click_config_onto_window(s_menu_layer, window);

  layer_add_child(window_layer, menu_layer_get_layer(s_menu_layer));
}

void load_item_data(DictionaryIterator *iterator) {
  Tuple *type = dict_find(iterator, 3);

  struct Item newItem;

    newItem.title = dict_find(iterator, 0)->value->cstring;
    newItem.type = dict_find(iterator, 3)->value->cstring;
    newItem.subtitle = dict_find(iterator, 2)->value->cstring;
    newItem.id = dict_find(iterator, 1)->value->int32;

  if(strcmp(type->value->cstring, "general") == 0) {
    vec_push(&menu_items_general, newItem);
  } else if(strcmp(type->value->cstring, "private") == 0) {
    vec_push(&menu_items_private, newItem);
  }

  if(menu_items_private.length > 0 || menu_items_general.length > 0) {
    layer_set_hidden(text_layer_get_layer(title_text_layer), true);
    layer_set_hidden(menu_layer_get_layer(s_menu_layer), false);
  }
}

void main_window_unload(Window *window) {
  vec_deinit(&menu_items_general);
  vec_deinit(&menu_items_private);
  menu_layer_destroy(s_menu_layer);
}

void main_view_init() {
  main_window = window_create();
  vec_init(&menu_items_general);
  vec_init(&menu_items_private);

  window_set_window_handlers(main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
}

void main_view_deinit() {
  window_destroy(main_window);
}

void main_view_show() {
  send_app_message(KEY_GET_DATA, "true");
  window_stack_push(main_window, true);
}