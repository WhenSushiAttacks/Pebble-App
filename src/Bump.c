#include <pebble.h>

#define kFilteringFactor 0.1

static Window *s_main_window;
static TextLayer *s_output_layer;

int accelFilter[3];

static void data_handler(AccelData *data, uint32_t num_samples) {
  // Long lived buffer
  static char s_buffer_1[128];
  snprintf(s_buffer_1, sizeof(s_buffer_1), "Bump YAY! :D %d,%d,%d", data[0].x, data[0].y, data[0].z - accelFilter[2]);
  // X makes sure the pebble is facing up, y is forward motion, z is height
  // && ((data[0].z > -800 && data[0].z < -1100) || (data[0].z > 800 && data[0].z < 1100))
  if(
    (data[0].x < -650 || (data[0].x > 1200 && data[0].x < 1350))
    &&
    ((data[0].y < 500 && data[0].y > 0) || (data[0].y < -200 && data[0].y > -1100))
    &&
    (data[0].z - accelFilter[2]) < -300 && (data[0].z - accelFilter[2]) > -1300
    ) {
    text_layer_set_text(s_output_layer,s_buffer_1);
      APP_LOG(APP_LOG_LEVEL_DEBUG,s_buffer_1);
  }

  if(
    (data[0].y < -650 || (data[0].y > 1200 && data[0].y < 1350))
    &&
    ((data[0].x < 500 && data[0].x > 0) || (data[0].x < -200 && data[0].x > -1100))
    &&
    (data[0].z - accelFilter[2]) < -300 && (data[0].z - accelFilter[2]) > -1300
    ) {
    text_layer_set_text(s_output_layer,s_buffer_1);
      APP_LOG(APP_LOG_LEVEL_DEBUG,s_buffer_1);
  }
  accelFilter[0] = data[0].x;
  accelFilter[1] = data[0].y;
  accelFilter[2] = data[0].z;



  // accelFilter[0] = data[0].x * kFilteringFactor + accelFilter[0] * (1.0f - kFilteringFactor);
  // accelFilter[1] = data[0].y * kFilteringFactor + accelFilter[1] * (1.0f - kFilteringFactor);
  // accelFilter[2] = data[0].z * kFilteringFactor + accelFilter[2] * (1.0f - kFilteringFactor);

  // if(abs(data[0].y - accelFilter[1]) > 100) {
  //   text_layer_set_text(s_output_layer,s_buffer);
  // }

  // is_second_handler = !is_second_handler;

  // if(is_second_handler) {
  //   snprintf(s_buffer, sizeof(s_buffer), "Yay! %d - %d - %d", (last_y - data[0].y), (last_x - data[0].x), (last_z - data[0].z));
  //   APP_LOG(APP_LOG_LEVEL_DEBUG,s_buffer);
  //   if((last_x != 0 && last_y != 0 && last_z != 0) && (last_y - data[0].y) > 250 && (last_x - data[0].x) < 65 && (last_z - data[0].z) < 5) {
  //     text_layer_set_text(s_output_layer,s_buffer);
  //   }
  //   last_x = data[0].x;
  //   last_y = data[0].y;
  //   last_z = data[0].z;
  // }
  // -510/800 y
  // -1140/950 z
  // x 0/200 x
}

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);

  // Create output TextLayer
  s_output_layer = text_layer_create(GRect(5, 0, window_bounds.size.w - 10, window_bounds.size.h));
  text_layer_set_font(s_output_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  text_layer_set_text(s_output_layer, "No data yet.");
  text_layer_set_overflow_mode(s_output_layer, GTextOverflowModeWordWrap);
  layer_add_child(window_layer, text_layer_get_layer(s_output_layer));
}

static void main_window_unload(Window *window) {
  // Destroy output TextLayer
  text_layer_destroy(s_output_layer);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  static char s_buffer[128];
  snprintf(s_buffer, sizeof(s_buffer), "Bump! %d %d %d", accelFilter[0], accelFilter[1], accelFilter[2]);
  text_layer_set_text(s_output_layer,s_buffer);
}
static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}

static void init() {
  // Create main Window
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  window_stack_push(s_main_window, true);

  // Subscribe to the accelerometer data service
  int num_samples = 1;
  accel_data_service_subscribe(num_samples, data_handler);
  accel_service_set_sampling_rate(ACCEL_SAMPLING_50HZ);

  window_set_click_config_provider(s_main_window, click_config_provider);
}

static void deinit() {
  // Destroy main Window
  window_destroy(s_main_window);
  accel_data_service_unsubscribe();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}